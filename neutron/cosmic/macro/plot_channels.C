#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLeaf.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

// From macro/: root -l -b -q 'plot_channels.C()'
// All runs >= firstRun are discovered, including cosmic_beta_run*.root.
// lastRun=-1 selects the last available run. Default TDC is raw hardware counts.
// referenceTDC=true instead plots tdc[15]-tdc[channel], requiring both valid.
void plot_channels(int firstRun=70, int lastRun=-1, bool referenceTDC=false,
                   const char* dataDir="", const char* outputPdf="") {
    gROOT->SetBatch(kTRUE);
    gStyle->SetOptStat(1110);
    gStyle->SetTitleFontSize(0.045);
    TString base=gSystem->DirName(__FILE__);
    TString input = TString(dataDir).IsNull() ? base+"/../data" : TString(dataDir);
    TString output = TString(outputPdf).IsNull()
        ? base+"/result/cosmic_channels_run70_to_last.pdf" : TString(outputPdf);
    struct Run { int number; TString file; };
    std::vector<Run> runs;
    void* dir=gSystem->OpenDirectory(input);
    if (!dir) throw std::runtime_error("Cannot open data directory");
    while (const char* entry=gSystem->GetDirEntry(dir)) {
        TString name(entry);
        if (!name.EndsWith(".root")) continue;
        TString prefix;
        if (name.BeginsWith("cosmic_run")) prefix="cosmic_run";
        else if (name.BeginsWith("cosmic_beta_run")) prefix="cosmic_beta_run";
        else continue;
        TString digits=name(prefix.Length(),name.Length()-prefix.Length()-5);
        if (!digits.IsDigit()) continue;
        int n=digits.Atoi();
        if (n>=firstRun && (lastRun<0 || n<=lastRun)) runs.push_back({n,name});
    }
    gSystem->FreeDirectory(dir);
    std::sort(runs.begin(),runs.end(),[](const Run& a,const Run& b){
        return a.number==b.number ? a.file<b.file : a.number<b.number;
    });
    if (runs.empty()) throw std::runtime_error("No matching ROOT files");
    if (gSystem->mkdir(gSystem->DirName(output),kTRUE)!=0 &&
        gSystem->AccessPathName(gSystem->DirName(output)))
        throw std::runtime_error("Cannot create output directory");
    const std::array<int,6> channels={4,5,6,7,10,11};
    const std::array<const char*,6> labels={"Neutron 1-1","Neutron 1-2",
        "Neutron 2-1","Neutron 2-2","Trig 1","Trig 2"};
    TCanvas canvas("channel_summary","Cosmic channel summary",1500,1000);
    canvas.Print(output+"[");
    int completed=0, pages=0, failed=0;
    for (const auto& run:runs) {
        std::unique_ptr<TFile> file(TFile::Open(input+"/"+run.file));
        TTree* tree=nullptr;
        if (file && !file->IsZombie()) file->GetObject("tree",tree);
        if (!tree || !tree->GetLeaf("qdc") || !tree->GetLeaf("tdc") ||
            tree->GetLeaf("qdc")->GetLenStatic()!=32 ||
            tree->GetLeaf("tdc")->GetLenStatic()!=16) {
            std::cerr<<"SKIP: invalid tree/branch layout: "<<run.file<<"\n";
            ++failed; continue;
        }
        int qdc[32],tdc[16];
        tree->SetBranchAddress("qdc",qdc);
        tree->SetBranchAddress("tdc",tdc);
        tree->SetBranchStatus("*",0);
        tree->SetBranchStatus("qdc",1); tree->SetBranchStatus("tdc",1);
        std::array<double,6> lo,hi;
        lo.fill(std::numeric_limits<double>::infinity());
        hi.fill(-std::numeric_limits<double>::infinity());
        const Long64_t entries=tree->GetEntries();
        auto validT=[&](int c){return tdc[c]!=-9999 && (!referenceTDC || tdc[15]!=-9999);};
        auto timeValue=[&](int c){return referenceTDC ? double(tdc[15])-tdc[c] : double(tdc[c]);};
        for (Long64_t e=0;e<entries;++e) {
            if(tree->GetEntry(e)<=0) throw std::runtime_error("Failed reading event");
            for(int i=0;i<6;++i) if(validT(channels[i])) {
                double t=timeValue(channels[i]); lo[i]=std::min(lo[i],t); hi[i]=std::max(hi[i],t);
            }
        }
        std::array<std::unique_ptr<TH1D>,6> adc,time;
        std::array<std::unique_ptr<TH2D>,6> joint;
        for(int i=0;i<6;++i) {
            if(!std::isfinite(lo[i])) {lo[i]=0;hi[i]=1;}
            double pad=std::max(1.,(hi[i]-lo[i])*0.05);
            lo[i]-=pad;hi[i]+=pad;
            TString title=Form("%s (CH%02d)",labels[i],channels[i]);
            TString tAxis=referenceTDC ? "TDC[15] - TDC[channel] [counts]" : "Raw TDC [counts]";
            adc[i].reset(new TH1D(Form("adc_%d",i),title+";ADC (QDC) [counts];Events",256,0,4096));
            time[i].reset(new TH1D(Form("tdc_%d",i),title+";"+tAxis+";Events",256,lo[i],hi[i]));
            joint[i].reset(new TH2D(Form("joint_%d",i),title+";ADC (QDC) [counts];"+tAxis,256,0,4096,256,lo[i],hi[i]));
            adc[i]->SetDirectory(nullptr);time[i]->SetDirectory(nullptr);joint[i]->SetDirectory(nullptr);
            joint[i]->SetStats(false);
        }
        for(Long64_t e=0;e<entries;++e) {
            if(tree->GetEntry(e)<=0) throw std::runtime_error("Failed reading event");
            for(int i=0;i<6;++i) {
                int c=channels[i];
                if(qdc[c]!=-9999) adc[i]->Fill(qdc[c]);
                if(validT(c)) {
                    time[i]->Fill(timeValue(c));
                    if(qdc[c]!=-9999) joint[i]->Fill(qdc[c],timeValue(c));
                }
            }
        }
        for(int kind=0;kind<3;++kind) {
            canvas.Clear();
            canvas.Divide(3,2,0.005,0.025);
            for(int i=0;i<6;++i) {
                canvas.cd(i+1);gPad->SetBottomMargin(0.13);gPad->SetLeftMargin(0.14);
                gPad->SetRightMargin(kind==2 ? 0.17 : 0.06);
                if(kind==0) adc[i]->Draw("HIST");
                else if(kind==1) time[i]->Draw("HIST");
                else joint[i]->Draw("COLZ");
            }
            canvas.cd();
            TPaveText heading(0.05,0.965,0.95,0.998,"NDC");
            heading.SetFillStyle(0);heading.SetBorderSize(0);heading.SetTextSize(0.019);
            heading.AddText(Form("%s | %s | %lld events | no physics cuts | missing (-9999) excluded",
                run.file.Data(),kind==0 ? "ADC" : kind==1 ? "TDC" : "ADC vs TDC",entries));
            heading.Draw();canvas.Print(output);++pages;
        }
        canvas.Clear();tree->ResetBranchAddresses();++completed;
        std::cout<<"DONE "<<run.file<<": "<<entries<<" events\n";
    }
    canvas.Print(output+"]");
    std::cout<<"Saved "<<output<<" ("<<completed<<" files, "<<pages<<" pages, "<<failed<<" skipped).\n";
}
