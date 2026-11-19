#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TLatex.h>
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <cmath>

// Gaussian core fits for run 118. Trig ranges must be supplied explicitly.
// Example: fit_pedestal118("", "", trig1Low, trig1High, trig2Low, trig2High)
// Unfitted channels retain their raw values and are flagged as uncorrected.
void fit_pedestal118(const char* inputFile="",const char* outputDir="",
                     double trig1Low=0,double trig1High=0,
                     double trig2Low=0,double trig2High=0) {
 gStyle->SetOptStat(0);
 TString base=gSystem->DirName(__FILE__);
 TString input=TString(inputFile).IsNull()?base+"/../data/cosmic_run118.root":TString(inputFile);
 TString out=TString(outputDir).IsNull()?base+"/result":TString(outputDir);
 gSystem->mkdir(out,kTRUE);
 TFile in(input);TTree* t=nullptr;in.GetObject("tree",t);
 if(!t || !t->GetBranch("qdc") || !t->GetBranch("tdc")) throw std::runtime_error("Missing input tree/branches");
 int qdc[32],tdc[16];t->SetBranchAddress("qdc",qdc);t->SetBranchAddress("tdc",tdc);
 const int ch[6]={4,5,6,7,10,11};
 const char* names[6]={"Neutron 1-1","Neutron 1-2","Neutron 2-1","Neutron 2-2","Trig 1","Trig 2"};
 // Fixed, reviewable ranges around the visible narrow cores; no automatic signal-peak selection.
 double low[6]={177,179,201,172,trig1Low,trig2Low};
 double high[6]={190,189,214,182,trig1High,trig2High};
 double mean[6]={},sigma[6]={};bool ok[6]={};
 std::array<std::unique_ptr<TH1D>,6> raw,corrected;
 std::array<std::unique_ptr<TF1>,6> fits;
 for(int i=0;i<6;i++) {
  raw[i].reset(new TH1D(Form("ped_raw_%d",i),Form("%s (CH%02d);Raw ADC [counts];Events / count",names[i],ch[i]),4096,0,4096));raw[i]->SetDirectory(nullptr);
 }
 for(Long64_t e=0;e<t->GetEntries();e++){t->GetEntry(e);for(int i=0;i<6;i++)if(qdc[ch[i]]!=-9999)raw[i]->Fill(qdc[ch[i]]);}
 for(int i=0;i<6;i++) {
  int status=-1,cov=-1;
  if(high[i]>low[i]) {
   fits[i].reset(new TF1(Form("ped_gauss_%d",i),"gaus",low[i],high[i]));
   raw[i]->GetXaxis()->SetRangeUser(low[i],high[i]);
   fits[i]->SetParameters(raw[i]->GetMaximum(),raw[i]->GetBinCenter(raw[i]->GetMaximumBin()),2.);
   raw[i]->GetXaxis()->SetRange(0,0);fits[i]->SetParLimits(2,0.2,20);
   TFitResultPtr result=raw[i]->Fit(fits[i].get(),"LQRSN");status=int(result);cov=result->CovMatrixStatus();
   mean[i]=fits[i]->GetParameter(1);sigma[i]=fits[i]->GetParameter(2);
   ok[i]=status==0 && cov==3 && mean[i]>low[i] && mean[i]<high[i] && sigma[i]>0.2 && sigma[i]<20;
  }
  std::cout<<names[i]<<": mean="<<mean[i]<<" sigma="<<sigma[i]<<" status="<<status<<" applied="<<ok[i]<<'\n';
  corrected[i].reset(new TH1D(Form("ped_sub_%d",i),Form("%s (CH%02d);%s;Events / count",names[i],ch[i],ok[i]?"ADC - pedestal mean [counts]":"Raw ADC [counts] (UNCORRECTED)"),4352,-256,4096));corrected[i]->SetDirectory(nullptr);
 }
 TFile result(out+"/run118_pedestal_subtracted.root","RECREATE");
 TTree correctedTree("tree","Run 118; per-channel Gaussian pedestal mean subtraction");
 double adc[32];bool applied[32]={};double pedestal[32]={};
 for(int i=0;i<6;i++){applied[ch[i]]=ok[i];pedestal[ch[i]]=ok[i]?mean[i]:0;}
 correctedTree.Branch("qdc",qdc,"qdc[32]/I");correctedTree.Branch("tdc",tdc,"tdc[16]/I");
 correctedTree.Branch("adc_pedsub",adc,"adc_pedsub[32]/D");
 correctedTree.Branch("pedestal_applied",applied,"pedestal_applied[32]/O");
 correctedTree.Branch("pedestal_mean",pedestal,"pedestal_mean[32]/D");
 for(Long64_t e=0;e<t->GetEntries();e++) {
  t->GetEntry(e);for(int c=0;c<32;c++)adc[c]=qdc[c]==-9999?-9999:qdc[c]-pedestal[c];
  for(int i=0;i<6;i++)if(qdc[ch[i]]!=-9999)corrected[i]->Fill(adc[ch[i]]);
  correctedTree.Fill();
 }
 correctedTree.Write();
 for(int i=0;i<6;i++){raw[i]->Write();corrected[i]->Write();if(fits[i])fits[i]->Write();}
 TCanvas canvas("ped118","Run 118 pedestal",1500,950);
 TString pdf=out+"/run118_pedestal.pdf";canvas.Print(pdf+"[");
 for(int page=0;page<3;page++) {
  canvas.Clear();canvas.Divide(3,2);
  for(int i=0;i<6;i++) {
   canvas.cd(i+1);gPad->SetLeftMargin(0.14);gPad->SetBottomMargin(0.13);
   if(page==0) {
    raw[i]->GetXaxis()->SetRangeUser(i<4?140:140,i<4?250:600);raw[i]->Draw("HIST");
    if(fits[i]){fits[i]->SetLineColor(kRed);fits[i]->Draw("SAME");}
   } else {
    corrected[i]->GetXaxis()->SetRangeUser(page==1?(ok[i]?-30:140):-256,page==1?(ok[i]?100:600):4096);
    if(page==2)gPad->SetLogy();corrected[i]->Draw("HIST");
   }
   TLatex text;text.SetNDC();text.SetTextSize(0.032);
   if(ok[i])text.DrawLatex(0.17,0.84,Form("Pedestal: %.2f #pm %.2f; #sigma = %.2f",mean[i],fits[i]->GetParError(1),sigma[i]));
   else text.DrawLatex(0.17,0.84,"UNCORRECTED: pedestal not established");
  }
  canvas.Print(pdf);
 }
 canvas.Print(pdf+"]");canvas.Clear();t->ResetBranchAddresses();
 std::cout<<"Saved PDF and ROOT file in "<<out<<'\n';
}
