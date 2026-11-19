#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TLinearFitter.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TNamed.h>
#include <TParameter.h>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace Slew118 {
struct Event {Long64_t entry; std::array<int,32> q; std::array<int,16> t; std::array<double,6> b; double dt; bool train=false;};
double median(std::vector<double> x){if(x.empty())throw std::runtime_error("Empty sample");std::sort(x.begin(),x.end());return (x[(x.size()-1)/2]+x[x.size()/2])/2;}
double robustWidth(const std::vector<double>& x){double m=median(x);std::vector<double> d;for(double v:x)d.push_back(std::abs(v-m));return std::max(0.025,1.4826*median(d));}
double rms(const std::vector<double>& x){double m=std::accumulate(x.begin(),x.end(),0.)/x.size(),s=0;for(double v:x)s+=(v-m)*(v-m);return std::sqrt(s/x.size());}
struct Core {std::unique_ptr<TH1D> hist;std::unique_ptr<TF1> fit;int status,cov;double lo,hi;};
Core core(const std::vector<double>& values,const char* name,const char* title) {
 double m=median(values),s=robustWidth(values);
 Core r;r.lo=m-2.5*s;r.hi=m+2.5*s;
 r.hist.reset(new TH1D(name,title,160,m-8*s,m+8*s));r.hist->SetDirectory(nullptr);
 for(double v:values)r.hist->Fill(v);
 r.fit.reset(new TF1(Form("%s_gaussian",name),"gaus",r.lo,r.hi));
 r.fit->SetParameters(r.hist->GetMaximum(),m,s);r.fit->SetParLimits(2,0.005,10*s);
 auto result=r.hist->Fit(r.fit.get(),"LQRSN");r.status=int(result);r.cov=result->CovMatrixStatus();return r;
}
}

// Run 118, raw ADC cuts, independent reference-TDC time-walk calibration and held-out evaluation.
// No pedestal subtraction. No CSV. tdcNs=0.025 follows the existing analysis.
// Four-detector, zero-covariance variance separation; no equal-ND assumption.
void slewing118(const char* inputFile="",const char* outputDir="",double tdcNs=0.025,
                double adcUpper=3800) {
 using namespace Slew118;
 if(tdcNs<=0 || adcUpper<=1130)throw std::runtime_error("Invalid scale or ADC upper cut");
 gStyle->SetOptStat(0);gStyle->SetPalette(kRainBow);gStyle->SetNumberContours(255);
 TString base=gSystem->DirName(__FILE__);
 TString input=TString(inputFile).IsNull()?base+"/../data/cosmic_run118.root":TString(inputFile);
 TString out=TString(outputDir).IsNull()?base+"/result":TString(outputDir);
 gSystem->mkdir(out,kTRUE);
 TFile source(input);TTree* tree=nullptr;source.GetObject("tree",tree);
 if(!tree || !tree->GetLeaf("qdc") || !tree->GetLeaf("tdc") || tree->GetLeaf("qdc")->GetLenStatic()!=32 || tree->GetLeaf("tdc")->GetLenStatic()!=16)throw std::runtime_error("Expected tree with qdc[32], tdc[16]");
 int q[32],t[16];tree->SetBranchAddress("qdc",q);tree->SetBranchAddress("tdc",t);
 tree->SetBranchStatus("*",0);tree->SetBranchStatus("qdc",1);tree->SetBranchStatus("tdc",1);
 const double cuts[6]={1130,1040,1000,1000,350,260};const int channels[6]={4,5,6,7,10,11};
 const char* detectorNames[6]={"Neutron 1-1","Neutron 1-2","Neutron 2-1","Neutron 2-2","Trig 1","Trig 2"};
 const double sign[4]={-1,-1,1,1};
 std::vector<Event> ev;Long64_t adcCount=0;
 for(Long64_t e=0;e<tree->GetEntries();e++) {
  if(tree->GetEntry(e)<=0)throw std::runtime_error("Input read failure");
  bool pass=true;for(int i=0;i<6;i++)if(q[channels[i]]<=cuts[i] || q[channels[i]]>=adcUpper)pass=false;
  if(!pass)continue;++adcCount;
  for(int i=0;i<6;i++)if(t[channels[i]]==-9999)pass=false;
  if(t[15]==-9999)pass=false;
  if(!pass)continue;
  Event v;v.entry=e;std::copy(q,q+32,v.q.begin());std::copy(t,t+16,v.t.begin());
  for(int i=0;i<6;i++)v.b[i]=std::sqrt(1500./q[channels[i]])-1.;
  v.dt=(double(t[6])+t[7]-t[4]-t[5])*0.5*tdcNs;ev.push_back(v);
 }
 tree->ResetBranchAddresses();
 if(ev.size()<400)throw std::runtime_error("Too few selected events (need >=400)");
 std::vector<size_t> order(ev.size());std::iota(order.begin(),order.end(),0);
 std::mt19937 random(118);std::shuffle(order.begin(),order.end(),random);
 size_t ntrain=ev.size()/2;for(size_t i=0;i<ntrain;i++)ev[order[i]].train=true;
 // cosmic_analysis.cc convention: fit each TDC[i]-TDC[15] versus its ADC.
 // c+s*(sqrt(1500/Q)-1) == a/sqrt(Q)-b, with a=s*sqrt(1500), b=s-c.
 std::array<std::array<double,2>,6> coeff{};
 for(int channel=0;channel<6;channel++) {
  std::vector<double> timing;for(auto& v:ev)if(v.train)timing.push_back(double(v.t[channels[channel]])-v.t[15]);
  coeff[channel][0]=median(timing);double scale=robustWidth(timing);
  for(int iteration=0;iteration<15;iteration++) {
   TLinearFitter regression(1,"pol1");
   for(auto& v:ev)if(v.train) {
    double x=v.b[channel],res=double(v.t[channels[channel]])-v.t[15]-coeff[channel][0]-coeff[channel][1]*x;
    double z=std::abs(res)/scale,w=z>1.5?1.5/z:1.;
    regression.AddPoint(&x,double(v.t[channels[channel]])-v.t[15],1./std::sqrt(w));
   }
   if(regression.Eval()!=0)throw std::runtime_error("Individual reference-TDC fit failed");
   for(int p=0;p<2;p++){coeff[channel][p]=regression.GetParameter(p);if(!std::isfinite(coeff[channel][p]))throw std::runtime_error("Nonfinite coefficient");}
   std::vector<double> residual;for(auto& v:ev)if(v.train)residual.push_back(double(v.t[channels[channel]])-v.t[15]-coeff[channel][0]-coeff[channel][1]*v.b[channel]);
   scale=robustWidth(residual);
  }
  std::cout<<detectorNames[channel]<<": TDC-TDC15 fit intercept="<<coeff[channel][0]<<" slope="<<coeff[channel][1]<<" counts\n";
 }
 auto correctedTime=[&](const Event& v,int i){return (double(v.t[channels[i]])-v.t[15]-coeff[i][0]-coeff[i][1]*v.b[i])*tdcNs;};
 auto correction=[&](const Event& v){double shift=0;for(int i=0;i<4;i++)shift+=0.5*sign[i]*(coeff[i][0]+coeff[i][1]*v.b[i])*tdcNs;return shift;};
 std::vector<double> rawDt,corrDt;
 for(auto& v:ev)if(!v.train){rawDt.push_back(v.dt);corrDt.push_back(v.dt-correction(v));}
 auto before=core(rawDt,"dt_before","Held-out: before slewing;T_{Neutron 2} - T_{Neutron 1} [ns];Events");
 auto after=core(corrDt,"dt_after","Held-out: after slewing;T_{Neutron 2} - T_{Neutron 1} [ns];Events");
 bool reliable=before.status==0 && before.cov==3 && after.status==0 && after.cov==3;
 double sigma=after.fit->GetParameter(2),sigmaError=after.fit->GetParError(2);
 const char* timeNames[4]={"Neutron 1","Neutron 2","Trig 1","Trig 2"};
 const int pairA[6]={0,0,0,1,1,2},pairB[6]={1,2,3,2,3,3};
 auto times=[&](const Event& v,bool corrected){std::array<double,6> p;for(int i=0;i<6;i++)p[i]=corrected?correctedTime(v,i):(double(v.t[channels[i]])-v.t[15])*tdcNs;return std::array<double,4>{(p[0]+p[1])/2,(p[2]+p[3])/2,p[4],p[5]};};
 std::array<std::vector<Core>,2> pairFits;
 for(int stage=0;stage<2;stage++)for(int k=0;k<6;k++) {
  std::vector<double> d;for(auto& v:ev)if(!v.train){auto tt=times(v,stage);d.push_back(tt[pairB[k]]-tt[pairA[k]]);}
  pairFits[stage].push_back(core(d,Form("pair_%d_%d",stage,k),Form("%s: %s - %s;Time difference [ns];Events",stage?"Corrected":"Before",timeNames[pairB[k]],timeNames[pairA[k]])));
 }
 std::array<double,6> pairVariance;double sumVariance=0;std::array<double,4> rowSum{},separated{};
 for(int k=0;k<6;k++){auto& f=pairFits[1][k];if(f.status || f.cov!=3)throw std::runtime_error("Pair Gaussian fit failed; cannot separate variances");double w=f.fit->GetParameter(2);pairVariance[k]=w*w;sumVariance+=w*w;rowSum[pairA[k]]+=w*w;rowSum[pairB[k]]+=w*w;}
 // Equal-weight least squares of six equations V_ij = v_i+v_j, four unknowns.
 // No positivity clipping: a negative variance is an explicit model inconsistency.
 for(int i=0;i<4;i++)separated[i]=rowSum[i]/2-sumVariance/6;
 auto variance=[&](int a,int b){for(int k=0;k<6;k++)if((pairA[k]==a&&pairB[k]==b)||(pairA[k]==b&&pairB[k]==a))return pairVariance[k];throw std::runtime_error("Unknown pair");};
 auto tripletVariance=[&](int i,int j,int k){return (variance(i,j)+variance(i,k)-variance(j,k))/2;};
 // Paired bootstrap of held-out events preserves statistical dependence of pair fits.
 // Calibration coefficients are held fixed; this estimates evaluation statistics only.
 std::vector<std::array<double,4>> testTimes;for(auto& v:ev)if(!v.train)testTimes.push_back(times(v,true));
 std::mt19937 bootRandom(1182026);std::uniform_int_distribution<size_t> pick(0,testTimes.size()-1);
 std::vector<std::array<double,14>> bootValues;
 for(int b=0;b<200;b++) {
  std::array<std::vector<double>,6> sample;
  for(size_t n=0;n<testTimes.size();n++){auto& tt=testTimes[pick(bootRandom)];for(int k=0;k<6;k++)sample[k].push_back(tt[pairB[k]]-tt[pairA[k]]);}
  std::array<double,6> vv;bool good=true;
  for(int k=0;k<6;k++){auto fit=core(sample[k],Form("boot_%d_%d",b,k),"");if(fit.status || fit.cov!=3){good=false;break;}double w=fit.fit->GetParameter(2);vv[k]=w*w;}
  if(!good)continue;
  std::array<double,4> row{},vdet{};double total=0;
  for(int k=0;k<6;k++){total+=vv[k];row[pairA[k]]+=vv[k];row[pairB[k]]+=vv[k];}
  std::array<double,14> record{};
  for(int i=0;i<4;i++){vdet[i]=row[i]/2-total/6;record[i]=vdet[i];}
  record[4]=(vv[0]+vv[1]-vv[3])/2;record[5]=(vv[0]+vv[3]-vv[1])/2;
  record[6]=(vv[0]+vv[2]-vv[4])/2;record[7]=(vv[0]+vv[4]-vv[2])/2;
  for(int k=0;k<6;k++)record[8+k]=vv[k]-vdet[pairA[k]]-vdet[pairB[k]];
  bootValues.push_back(record);
 }
 if(bootValues.size()<160)throw std::runtime_error("Too few successful paired-bootstrap samples");
 std::array<double,14> varianceError{};
 for(int j=0;j<14;j++){double m=0;for(auto& r:bootValues)m+=r[j];m/=bootValues.size();for(auto& r:bootValues)varianceError[j]+=(r[j]-m)*(r[j]-m);varianceError[j]=std::sqrt(varianceError[j]/(bootValues.size()-1));}
 TString pdf=out+"/run118_slewing_resolution.pdf";
 TFile output(out+"/run118_slewing.root","RECREATE");
 TTree selected("selected","ADC-selected events, no timing window; split and correction stored");
 Long64_t originalEntry;bool training;int qout[32],tout[16];double tc[6],detectorTime[4],rawDelta,correctedDelta;
 selected.Branch("source_entry",&originalEntry);selected.Branch("training",&training);
 selected.Branch("qdc",qout,"qdc[32]/I");selected.Branch("tdc",tout,"tdc[16]/I");
 selected.Branch("pmt_time_corrected_ns",tc,"pmt_time_corrected_ns[6]/D");
 selected.Branch("detector_time_corrected_ns",detectorTime,"detector_time_corrected_ns[4]/D");
 selected.Branch("delta_raw_ns",&rawDelta);selected.Branch("delta_corrected_ns",&correctedDelta);
 for(auto& v:ev){originalEntry=v.entry;training=v.train;std::copy(v.q.begin(),v.q.end(),qout);std::copy(v.t.begin(),v.t.end(),tout);
  for(int i=0;i<6;i++)tc[i]=correctedTime(v,i);
  detectorTime[0]=(tc[0]+tc[1])/2;detectorTime[1]=(tc[2]+tc[3])/2;detectorTime[2]=tc[4];detectorTime[3]=tc[5];
  rawDelta=v.dt;correctedDelta=v.dt-correction(v);selected.Fill();}
 selected.Write();
 for(int i=0;i<6;i++){
  TParameter<double>(Form("reference_tdc_intercept_%d_counts",i),coeff[i][0]).Write();
  TParameter<double>(Form("reference_tdc_slope_%d_counts",i),coeff[i][1]).Write();
  TParameter<double>(Form("slewing_a_%d",i),coeff[i][1]*std::sqrt(1500.)).Write();
  TParameter<double>(Form("slewing_b_%d",i),coeff[i][1]-coeff[i][0]).Write();
 }
 TParameter<double>("tdc_ns_per_count",tdcNs).Write();
 TParameter<double>("heldout_sigma_dt_ns",sigma).Write();
 TParameter<double>("heldout_sigma_dt_error_ns",sigmaError).Write();
 TParameter<double>("heldout_fwhm_dt_ns",2.354820045*sigma).Write();

 TNamed("interpretation","Each TDC[i]-TDC[15] fitted independently versus own ADC with a/sqrt(ADC)-b; full fitted function subtracted. Absolute TOF offsets are removed. All pair variances are separated assuming zero detector covariance and negligible path-length fluctuations; no equal-resolution assumption.").Write();
 before.hist->Write();after.hist->Write();before.fit->Write();after.fit->Write();
 TCanvas canvas("slew118_canvas","Run 118 slewing",1500,1000);canvas.Print(pdf+"[");
 canvas.Clear();TLatex text;text.SetNDC();text.SetTextSize(0.025);
 double y=0.94;auto line=[&](TString message){text.DrawLatex(0.07,y,message);y-=0.052;};
 line("Run 118: neutron slewing and timing resolution");
 line(Form("Input events: %lld; six-channel ADC pass: %lld; Six PMT + reference TDC valid: %zu",tree->GetEntries(),adcCount,ev.size()));
 line(Form("Training: %zu; held-out evaluation: %zu; fixed shuffle seed 118",ntrain,rawDt.size()));
 line(Form("Raw ADC lower cuts: 1130, 1040, 1000, 1000, 350, 260; upper < %.0f",adcUpper));
 line(Form("Time scale: %.5f ns/count (from existing macro; not independently calibrated)",tdcNs));
 line("Neutron 1: mean of Neutron 1-1 / 1-2; Neutron 2: mean of Neutron 2-1 / 2-2");
 line("Each PMT: #Deltat_{i}=TDC_{i}-TDC_{15}; fit f_{i}(ADC)=a_{i}/#sqrt{ADC}-b_{i}");
 line("#Deltat_{i,corr}=#Deltat_{i}-f_{i}(ADC); full fit subtracted (cosmic_analysis.cc convention)");
 line(Form("Held-out Gaussian core #sigma: %.4f #rightarrow %.4f #pm %.4f ns",before.fit->GetParameter(2),sigma,sigmaError));
 line(Form("Held-out full-sample RMS: %.4f #rightarrow %.4f ns (all selected events)",rms(rawDt),rms(corrDt)));
 line(Form("Gaussian fit status/covariance: before %d/%d; after %d/%d",before.status,before.cov,after.status,after.cov));
 line("Variance separation uses six detector pairs, zero covariance, negligible flight-path spread.");
 line("Not a unique intrinsic measurement: no geometry/path or reference-resolution subtraction.");
 line("Cosmic charged-particle timing; neutron-interaction timing is not measured here.");
 line("No pedestal subtraction; valid TDC15 required; no timing-window event rejection.");
 line("Gaussian windows: median #pm 2.5 x 1.4826 MAD; core widths exclude tail modeling.");
 canvas.Print(pdf);
 // PMT reference-relative timing versus its own ADC, before/after on held-out events.
 for(int stage=0;stage<2;stage++) {
  canvas.Clear();canvas.Divide(3,2);
  std::array<std::unique_ptr<TH2D>,6> hist;
  std::array<std::unique_ptr<TProfile>,6> profile;
  std::array<std::unique_ptr<TF1>,6> curve;
  for(int i=0;i<6;i++) {
   canvas.cd(i+1);gPad->SetRightMargin(0.16);gPad->SetLeftMargin(0.15);gPad->SetBottomMargin(0.13);
   std::vector<double> values;
   for(auto& v:ev)if(!v.train)values.push_back(stage?correctedTime(v,i)/tdcNs:double(v.t[channels[i]])-v.t[15]);
   double m=median(values),width=robustWidth(values);
   hist[i].reset(new TH2D(Form("adc_tdc_%d_%d",stage,i),Form("HELD-OUT %s: %s;Raw ADC [counts];%s [counts]",detectorNames[i],stage?"after slewing":"before slewing",stage?"Corrected (TDC - TDC15)":"TDC - TDC15"),40,cuts[i],adcUpper,100,m-6*width,m+6*width));hist[i]->SetDirectory(nullptr);
   profile[i].reset(new TProfile(Form("adc_tdc_profile_%d_%d",stage,i),"",20,cuts[i],adcUpper));profile[i]->SetDirectory(nullptr);
   for(auto& v:ev)if(!v.train){double time=stage?correctedTime(v,i)/tdcNs:double(v.t[channels[i]])-v.t[15];hist[i]->Fill(v.q[channels[i]],time);profile[i]->Fill(v.q[channels[i]],time);}
   hist[i]->Draw("COLZ");profile[i]->SetLineColor(kBlack);profile[i]->SetMarkerStyle(20);profile[i]->SetMarkerSize(0.5);profile[i]->Draw("SAME");
   curve[i].reset(new TF1(Form("slew_curve_%d_%d",stage,i),stage?"[0]":"[0]+[1]*(sqrt(1500/x)-1)",cuts[i],adcUpper));
   curve[i]->SetParameter(0,stage?0:coeff[i][0]);if(!stage)curve[i]->SetParameter(1,coeff[i][1]);curve[i]->SetLineColor(kRed);curve[i]->Draw("SAME");
   hist[i]->Write();profile[i]->Write();curve[i]->Write();
  }
  canvas.Print(pdf);canvas.Clear();
 }
 canvas.Clear();canvas.Divide(2,1);
 Core* cores[2]={&before,&after};
 for(int i=0;i<2;i++){canvas.cd(i+1);gPad->SetLeftMargin(0.14);auto& r=*cores[i];r.hist->Draw("HIST");r.fit->SetLineColor(kRed);r.fit->Draw("SAME");
  TLatex note;note.SetNDC();note.SetTextSize(0.032);note.DrawLatex(0.17,0.86,Form("#sigma = %.4f #pm %.4f ns",r.fit->GetParameter(2),r.fit->GetParError(2)));
  note.DrawLatex(0.17,0.80,Form("#chi^{2}/ndf = %.1f/%d",r.fit->GetChisquare(),r.fit->GetNDF()));
  note.DrawLatex(0.17,0.74,Form("Fit window: [%.3f, %.3f] ns",r.lo,r.hi));}
 canvas.Print(pdf);canvas.Clear();
 canvas.Divide(2,2);std::array<std::unique_ptr<TH2D>,4> position;
 for(int det=0;det<2;det++)for(int stage=0;stage<2;stage++) {
  int k=2*det+stage;canvas.cd(k+1);gPad->SetRightMargin(0.16);gPad->SetLeftMargin(0.14);
  std::vector<double> x;for(auto& v:ev)if(!v.train){int a=2*det;double d=(v.t[a+4]-v.t[a+5])*tdcNs;
   if(stage)d=correctedTime(v,a)-correctedTime(v,a+1);x.push_back(d);}
  double xm=median(x),xs=robustWidth(x),ym=median(stage?corrDt:rawDt),ys=robustWidth(stage?corrDt:rawDt);
  position[k].reset(new TH2D(Form("position_%d",k),Form("HELD-OUT Neutron detector %d: %s;End time difference [ns];Detector mean time difference [ns]",det+1,stage?"corrected":"raw"),60,xm-5*xs,xm+5*xs,80,ym-6*ys,ym+6*ys));position[k]->SetDirectory(nullptr);
  size_t n=0;for(auto& v:ev)if(!v.train){position[k]->Fill(x[n++],stage?v.dt-correction(v):v.dt);}position[k]->Draw("COLZ");position[k]->Write();
 }
 canvas.Print(pdf);canvas.Clear();
 // Every time-difference Gaussian is displayed, both before and after correction.
 for(int stage=0;stage<2;stage++) {
  canvas.Clear();canvas.Divide(3,2);
  for(int k=0;k<6;k++){canvas.cd(k+1);gPad->SetLeftMargin(0.14);auto& r=pairFits[stage][k];r.hist->Draw("HIST");r.fit->SetLineColor(kRed);r.fit->Draw("SAME");
   TLatex label;label.SetNDC();label.SetTextSize(0.032);label.DrawLatex(0.17,0.86,Form("#sigma = %.1f #pm %.1f ps",1000*r.fit->GetParameter(2),1000*r.fit->GetParError(2)));
   label.DrawLatex(0.17,0.80,Form("#chi^{2}/ndf = %.1f/%d",r.fit->GetChisquare(),r.fit->GetNDF()));
   label.DrawLatex(0.17,0.74,Form("Fit [%.3f, %.3f] ns",r.lo,r.hi));r.hist->Write();r.fit->Write();
  }
  canvas.Print(pdf);canvas.Clear();
 }
 TLatex resolutionText;resolutionText.SetNDC();resolutionText.SetTextSize(0.027);
 double ry=0.94;auto resolutionLine=[&](TString value){resolutionText.DrawLatex(0.06,ry,value);ry-=0.055;};
 resolutionLine("Run 118 - detector variance separation (no equal-ND assumption)");
 resolutionLine("Zero covariance: #sigma_{ij}^{2}=v_{i}+v_{j}, v_{i}=#sigma_{i}^{2}");
 resolutionLine("Three-detector relation: v_{i}=(#sigma_{ij}^{2}+#sigma_{ik}^{2}-#sigma_{jk}^{2})/2");
 resolutionLine("Six-pair consistency estimate (check closure on the next page)");
 for(int i=0;i<4;i++) {
  if(separated[i]>=0)resolutionLine(Form("%s: #sigma = %.1f #pm %.1f ps (six-pair solution)",timeNames[i],1000*std::sqrt(separated[i]),500*varianceError[i]/std::sqrt(separated[i])));
  else resolutionLine(Form("%s: NEGATIVE variance %.6f ns^{2}; no physical resolution",timeNames[i],separated[i]));
  TParameter<double>(Form("separated_variance_%d_ns2",i),separated[i]).Write();
  TParameter<double>(Form("separated_variance_error_%d_ns2",i),varianceError[i]).Write();
 }
 for(int ref=2;ref<4;ref++)for(int nd=0;nd<2;nd++) {
  double vv=tripletVariance(nd,1-nd,ref);
  if(vv>=0)resolutionLine(Form("%s with %s: #sigma = %.1f #pm %.1f ps (three-detector relation)",timeNames[nd],timeNames[ref],1000*std::sqrt(vv),500*varianceError[4+2*(ref-2)+nd]/std::sqrt(vv)));
  else resolutionLine(Form("%s with %s: NEGATIVE variance %.6f ns^{2}",timeNames[nd],timeNames[ref],vv));
 }
 resolutionLine("Reference-specific three-detector results are shown separately; no equal-ND constraint.");
 resolutionLine("Gaussian core widths are approximate variance inputs; geometry not subtracted.");
 resolutionLine("Errors: paired bootstrap, fixed calibration; Gaussian core and geometry systematics omitted.");
 resolutionLine("No clipping of negative variances and no #sigma_{ND1}=#sigma_{ND2} constraint.");
 canvas.Print(pdf);canvas.Clear();
 TLatex closureText;closureText.SetNDC();closureText.SetTextSize(0.026);
 double cy=0.93;auto closureLine=[&](TString value){closureText.DrawLatex(0.06,cy,value);cy-=0.065;};
 closureLine("Zero-covariance consistency check: measured variance minus (v_{i}+v_{j})");
 closureLine("Six-pair solution is unweighted least squares, not a constraint on ND equality.");
 double maxPull=0;
 for(int k=0;k<6;k++) {
  double residual=pairVariance[k]-separated[pairA[k]]-separated[pairB[k]];
  maxPull=std::max(maxPull,std::abs(residual)/varianceError[8+k]);
  closureLine(Form("%s - %s: %.5f #pm %.5f ns^{2}",timeNames[pairB[k]],timeNames[pairA[k]],residual,varianceError[8+k]));
 }
 closureLine(Form("Largest individual |residual / bootstrap error| = %.2f (not a global significance)",maxPull));
 closureLine("Shared pair samples are bootstrapped together; calibration is fixed.");
 closureLine("Nonzero closure can reflect covariance, flight-path spread, or Gaussian core modeling.");
 closureLine("Treat the six-pair detector widths as a model-dependent consistency estimate.");
 closureLine("Use the reference-specific three-detector results to inspect reference dependence.");
 canvas.Print(pdf);canvas.Clear();canvas.Print(pdf+"]");output.Write();
 std::ofstream report((out+"/run118_slewing_summary.txt").Data());
 report<<"Run 118 independent PMT reference-TDC slewing calibration; held-out timing analysis\nInput: "<<input<<"\nTDC ns/count: "<<tdcNs<<"\nSelected/train/test: "<<ev.size()<<"/"<<ntrain<<"/"<<rawDt.size()<<"\n";
 report<<"ADC cuts Neutron 1-1/1-2/2-1/2-2/Trig 1/Trig 2 > 1130/1040/1000/1000/350/260 and < "<<adcUpper<<"\n";
 for(int i=0;i<6;i++)report<<detectorNames[i]<<": intercept="<<coeff[i][0]<<", slope="<<coeff[i][1]<<" counts; a="<<coeff[i][1]*std::sqrt(1500.)<<", b="<<coeff[i][1]-coeff[i][0]<<"; f(Q)=a/sqrt(Q)-b; time=(TDC-TDC15)-f(Q)\n";
 report<<"Before sigma +/- fit statistical error [ns]: "<<before.fit->GetParameter(2)<<" +/- "<<before.fit->GetParError(2)<<"\nAfter sigma +/- fit statistical error [ns]: "<<sigma<<" +/- "<<sigmaError<<"\n";
 report<<"Before/after full RMS [ns]: "<<rms(rawDt)<<" / "<<rms(corrDt)<<"\n";
 report<<"Before/after fit chi2/ndf: "<<before.fit->GetChisquare()<<"/"<<before.fit->GetNDF()<<" ; "<<after.fit->GetChisquare()<<"/"<<after.fit->GetNDF()<<"\n";
 report<<"Before/after fit status/cov: "<<before.status<<"/"<<before.cov<<" ; "<<after.status<<"/"<<after.cov<<"\n";
 report<<"Zero covariance assumed; no equal-detector resolution assumption. Negligible path-length spread assumed. No geometric or external reference subtraction. Errors omit calibration/model/geometry systematics. Gaussian core fits use median +/- 2.5 robust sigma. No time-window selection; tails contribute to RMS. Plots are each PMT TDC-TDC15 and its corrected value versus own ADC. Full fitted offsets are subtracted, so the corrected mean difference is not absolute flight time. Reference jitter and hit-position effects are not independently calibrated.\n";
 report<<"After delta FWHM [ps]: "<<2354.820045*sigma<<" +/- "<<2354.820045*sigmaError<<"\n";
 for(int k=0;k<6;k++) {auto& f=pairFits[1][k];report<<timeNames[pairB[k]]<<" - "<<timeNames[pairA[k]]<<": sigma="<<1000*f.fit->GetParameter(2)<<" +/- "<<1000*f.fit->GetParError(2)<<" ps, variance="<<pairVariance[k]<<" ns2; six-pair closure residual="<<pairVariance[k]-separated[pairA[k]]-separated[pairB[k]]<<" ns2\n";}
 for(int i=0;i<4;i++)report<<timeNames[i]<<": six-pair variance="<<separated[i]<<" ns2, sigma="<<(separated[i]>=0?1000*std::sqrt(separated[i]):-1)<<" ps (-1 means nonphysical)\n";
 for(int ref=2;ref<4;ref++)for(int nd=0;nd<2;nd++){double vv=tripletVariance(nd,1-nd,ref);report<<timeNames[nd]<<" with "<<timeNames[ref]<<": triplet variance="<<vv<<" ns2, sigma="<<(vv>=0?1000*std::sqrt(vv):-1)<<" ps\n";}
 report<<"Paired bootstrap accepted "<<bootValues.size()<<"/200; fixed calibration; sigma errors by delta method from variance bootstrap.\n";
 for(int i=0;i<4;i++)report<<timeNames[i]<<": six-pair variance error="<<varianceError[i]<<" ns2; sigma error="<<(separated[i]>0?500*varianceError[i]/std::sqrt(separated[i]):-1)<<" ps\n";
 for(int ref=2;ref<4;ref++)for(int nd=0;nd<2;nd++){int j=4+2*(ref-2)+nd;double v=tripletVariance(nd,1-nd,ref);report<<timeNames[nd]<<" with "<<timeNames[ref]<<": triplet variance error="<<varianceError[j]<<" ns2, sigma error="<<(v>0?500*varianceError[j]/std::sqrt(v):-1)<<" ps\n";}
 for(int k=0;k<6;k++)report<<"Pair "<<k<<" closure residual error="<<varianceError[8+k]<<" ns2\n";
 report<<"The six-pair result is a consistency fit; reference-specific three-detector solutions should be inspected alongside its closure residuals. No equal-ND constraint. Negative variances are not clipped.\n";
 report.close();std::cout<<"Selected "<<ev.size()<<", train "<<ntrain<<", held-out "<<rawDt.size()<<"\n";
 std::cout<<"Held-out sigma before/after [ns]: "<<before.fit->GetParameter(2)<<" / "<<sigma<<" +/- "<<sigmaError<<"\nSaved "<<pdf<<"\n";
}
