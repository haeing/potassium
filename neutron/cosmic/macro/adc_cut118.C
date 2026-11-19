#include <TFile.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TMath.h>
#include <cmath>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TSystem.h>
#include <array>
#include <memory>
#include <iostream>
#include <stdexcept>

// Raw ADC: neutron signal = Landau + constant background, no pedestal subtraction.
// Negative neutron cut values select auto cut = fitted mode - 2*Landau width.
// Landau width is a scale parameter, not a Gaussian sigma. No CSV output.
void adc_cut118(const char* inputFile="", const char* outputPdf="",
 double cut4=-1,double cut5=-1,double cut6=-1,double cut7=-1,
 double cut10=350,double cut11=260) {
 gStyle->SetOptStat(0);
 TString base=gSystem->DirName(__FILE__);
 TString input=TString(inputFile).IsNull()?base+"/../data/cosmic_run118.root":TString(inputFile);
 TString pdf=TString(outputPdf).IsNull()?base+"/result/run118_adc_cuts.pdf":TString(outputPdf);
 TFile f(input);TTree* t=nullptr;f.GetObject("tree",t);
 if(!t || !t->GetLeaf("qdc") || t->GetLeaf("qdc")->GetLenStatic()!=32) throw std::runtime_error("Missing qdc[32] input");
 int qdc[32];t->SetBranchAddress("qdc",qdc);t->SetBranchStatus("*",0);t->SetBranchStatus("qdc",1);
 const int ch[6]={4,5,6,7,10,11};
 const char* label[6]={"Neutron 1-1","Neutron 1-2","Neutron 2-1","Neutron 2-2","Trig 1","Trig 2"};
 double cut[6]={cut4,cut5,cut6,cut7,cut10,cut11};
 std::array<std::unique_ptr<TH1D>,6> raw,pass;
 Long64_t valid[6]={},kept[6]={};
 for(int i=0;i<6;i++) {
  raw[i].reset(new TH1D(Form("cut118_raw_%d",i),Form("Run 118 - %s (CH%02d);Raw ADC [counts];Events / 5 counts",label[i],ch[i]),820,0,4100));
  pass[i].reset((TH1D*)raw[i]->Clone(Form("cut118_pass_%d",i)));
  raw[i]->SetDirectory(nullptr);pass[i]->SetDirectory(nullptr);
  raw[i]->SetLineColor(kGray+2);pass[i]->SetLineColor(kBlue+1);
 }
 for(Long64_t e=0;e<t->GetEntries();e++) {
  if(t->GetEntry(e)<=0)throw std::runtime_error("Event read failed");
  for(int i=0;i<6;i++) if(qdc[ch[i]]!=-9999) {
   ++valid[i];raw[i]->Fill(qdc[ch[i]]);

  }
 }
 std::array<std::unique_ptr<TF1>,4> signal;
 const double fitLow[4]={950,900,850,850};
 for(int i=0;i<4;i++) {
  signal[i].reset(new TF1(Form("signal118_%d",i),
   "[0]*TMath::Landau(x,[1]+0.22278298*[2],[2],true)+[3]",fitLow[i],2300));
  signal[i]->SetParNames("Area","Mode","Landau width","Background");
  signal[i]->SetParameters(30000, i<2?1250:1150,90,10);
  signal[i]->SetParLimits(0,1,1e8);signal[i]->SetParLimits(1,1000,1600);
  signal[i]->SetParLimits(2,10,300);signal[i]->SetParLimits(3,0,1000);
  auto fit=raw[i]->Fit(signal[i].get(),"LQRSN");
  if(int(fit)!=0 || fit->CovMatrixStatus()!=3)throw std::runtime_error("Landau fit failed; inspect before applying cut");
  double candidate=signal[i]->GetParameter(1)-2*signal[i]->GetParameter(2);
  if(cut[i]<0)cut[i]=10*std::round(candidate/10.);
  signal[i]->SetLineColor(kGreen+2);signal[i]->SetLineWidth(2);
  std::cout<<"FIT CH"<<ch[i]<<": mode="<<signal[i]->GetParameter(1)<<" width="<<signal[i]->GetParameter(2)
   <<" chi2/ndf="<<signal[i]->GetChisquare()<<"/"<<signal[i]->GetNDF()<<" cut="<<cut[i]<<'\n';
 }
 for(Long64_t e=0;e<t->GetEntries();e++) {
  t->GetEntry(e);
  for(int i=0;i<6;i++)if(qdc[ch[i]]!=-9999 && qdc[ch[i]]>cut[i]){++kept[i];pass[i]->Fill(qdc[ch[i]]);}
 }
 gSystem->mkdir(gSystem->DirName(pdf),kTRUE);
 TCanvas c("adc_cut118_canvas","Run 118 raw ADC cuts",1600,1000);
 c.Print(pdf+"[");
 for(int page=0;page<2;page++) {
  c.Clear();c.Divide(3,2);
  // Objects must remain alive until the complete canvas has been printed.
  std::array<std::unique_ptr<TLine>,6> lines;
  std::array<std::unique_ptr<TLegend>,6> legends;
  for(int i=0;i<6;i++) {
   c.cd(i+1);gPad->SetLeftMargin(0.13);gPad->SetBottomMargin(0.12);gPad->SetLogy();
   double xmax=page==0?4100:(i<4?2300:800);
   double xmin=page==1 && i<4?700:0;
   gPad->SetLogy(page==0 || i>=4);
   raw[i]->GetXaxis()->SetRangeUser(xmin,xmax);raw[i]->SetMinimum(0.5);raw[i]->SetMaximum(-1111);raw[i]->SetMaximum(raw[i]->GetMaximum()*(page==0?8:1.8));
   raw[i]->Draw("HIST");pass[i]->GetXaxis()->SetRangeUser(xmin,xmax);pass[i]->Draw("HIST SAME");
   if(i<4)signal[i]->Draw("SAME");
   lines[i].reset(new TLine(cut[i],0.5,cut[i],raw[i]->GetMaximum()));
   lines[i]->SetLineColor(kRed+1);lines[i]->SetLineStyle(2);lines[i]->SetLineWidth(2);lines[i]->Draw();
   legends[i].reset(new TLegend(0.56,0.65,0.91,0.81));legends[i]->SetBorderSize(0);legends[i]->SetFillStyle(0);legends[i]->SetTextSize(0.031);
   legends[i]->AddEntry(raw[i].get(),"Before cut","l");legends[i]->AddEntry(pass[i].get(),"After channel cut","l");legends[i]->AddEntry(lines[i].get(),"ADC cut","l");if(i<4)legends[i]->AddEntry(signal[i].get(),"Landau + background","l");legends[i]->Draw();
   TLatex text;text.SetNDC();text.SetTextSize(0.032);
   text.DrawLatex(0.16,0.87,Form("Keep ADC > %.0f: %lld / %lld (%.1f%%)",cut[i],kept[i],valid[i],valid[i]?100.*kept[i]/valid[i]:0));
   text.SetTextSize(0.028);text.DrawLatex(0.16,0.82,i<4?"Cut: mode - 2 x Landau width (rounded to 10)":"Provisional valley cut; signal loss possible");
  }
  c.Print(pdf);c.Clear();
 }
 c.Print(pdf+"]");t->ResetBranchAddresses();
 for(int i=0;i<6;i++)std::cout<<"CH"<<ch[i]<<": ADC > "<<cut[i]<<", retained "<<kept[i]<<"/"<<valid[i]<<'\n';
 std::cout<<"Saved "<<pdf<<" (2 pages; independent channel cuts, no coincidence requirement)\n";
}
