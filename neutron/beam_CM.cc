void beam_CM(){

  gStyle->SetOptStat(0);


  //AMU methode (atomic mass)
  double amu = 0.931494; //GeV/c2
  double em = 0.000511; //GeV/c2
  double c  = 299792458;

  double Arm = 39.96238312*amu+em; //Z = 18, N = 22 e = -1
  double pm = 0.938272;
  double nm = 0.939565;
  double Clm = 36.9659026*amu+em; //Z = 17, N = 20 
  double alpham = 4.002603*amu;
  double K41m = 40.961825*amu;
  double K40m = 39.963998*amu;

  double Ekin_cm = 0; //MeV
  cout<<"K40 ex Arp : "<<Ekin_cm+(Arm+pm-K40m-nm)*1000<<" MeV"<<endl;
  cout<<"K40 ex Cla : "<<Ekin_cm+(Clm+alpham-K40m-nm)*1000<<" MeV"<<endl;

  cout<<"K41 ex Arp : "<<Ekin_cm+(Arm+pm-K41m)*1000<<" MeV"<<endl;
  cout<<"K41 ex Cla : "<<Ekin_cm+(Clm+alpham-K41m)*1000<<" MeV"<<endl;

  cout<<"K40+n and K41 : "<<(K41m-K40m-nm)*1000<<" MeV"<<endl;

  

  TGraph *ar_beamp = new TGraph();
  TGraph *p_beamar = new TGraph();
  TGraph *cl_beama = new TGraph();
  TGraph *a_beamcl = new TGraph();


  double kin_min = 0.00001; //GeV
  double kin_max = 0.1;
  //double kin_max = 0.001;

  int bin = 500;
  double step = (kin_max-kin_min)/bin;

  double kin_beam;
  double kin_ar_beamp_cm;
  double kin_cl_beama_cm;
  double kin_p_beamar_cm;
  double kin_a_beamcl_cm;

  for(int i=0;i<bin;i++){
    kin_beam = kin_min+step*i;

    //kin_ar_beamp_cm = kin_beam*Arm/(Arm+pm);
    kin_ar_beamp_cm = kin_beam*K40m/(K40m+nm);
    kin_cl_beama_cm = kin_beam*Clm/(Clm+alpham);
    kin_p_beamar_cm = kin_beam*pm/(Arm+pm);
    kin_a_beamcl_cm = kin_beam*alpham/(Clm+alpham);

    ar_beamp->AddPoint(kin_beam*1000,kin_ar_beamp_cm*1000);
    cl_beama->AddPoint(kin_beam*1000,kin_cl_beama_cm*1000);

    p_beamar->AddPoint(kin_beam*1000,kin_p_beamar_cm*1000);
    a_beamcl->AddPoint(kin_beam*1000,kin_a_beamcl_cm*1000);
    
    
  }
  TCanvas *c1 = new TCanvas("c1","c1",200,10,700,500);

  cout<<"Arp thre : "<<2.287*(pm+Arm)/(Arm)<<endl;
  cout<<"Cla thre : "<<3.872*(alpham+Clm)/(Clm)<<endl;


  

  
  ar_beamp->SetTitle(";E_{beam} [MeV];E_{cm} [MeV]");
  
  double y_min = 0; //MeV
  double y_max = kin_max*1000; //MeV
  

  double x_min = 0;
  double x_max = kin_max*1000;
  ar_beamp->GetYaxis()->SetRangeUser(y_min,y_max);
  ar_beamp->GetXaxis()->SetRangeUser(x_min,x_max);

  cl_beama->SetLineColor(kRed);
  cl_beama->SetLineWidth(3);
  
  ar_beamp->SetLineWidth(3);
  ar_beamp->SetLineColor(kBlue);
  
  p_beamar->SetLineWidth(3);
  p_beamar->SetLineColor(kGreen);
  
  a_beamcl->SetLineWidth(3);

  
  ar_beamp->Draw();

  
  cl_beama->Draw("same");
  p_beamar->Draw("same");
  a_beamcl->Draw("same");

  

    
  TLegend *l1 = new TLegend(0.8,0.5,0.48,0.6);
  l1->AddEntry(ar_beamp,"Target : ^{40}Ar, Beam : p");
  l1->AddEntry(p_beamar,"Target : p,   Beam : ^{40}Ar");
  l1->AddEntry(cl_beama,"Target : ^{37}Cl, Beam : #alpha");
  l1->AddEntry(a_beamcl,"Target : #alpha,   Beam : ^{37}Cl");
  l1->SetBorderSize(0);
  l1->Draw();

  


  double y_min_T = y_min*1E-3/(8.6173281E-5);
  double y_max_T = y_max*1E-3/(8.6173281E-5);

  //double y_min_T = y_min*1E+6/11604;
  //double y_max_T = y_max*1E+6/11604;
  TGaxis *axis = new TGaxis(x_max,y_min,x_max,y_max,y_min_T,y_max_T,510,"L+");
  axis->SetTitle("T [GK]");
  axis->SetLineColor(kRed);
  axis->SetLineWidth(2);
  axis->SetLabelColor(kRed);
  axis->SetTextColor(kRed);
  axis->SetTitleColor(kRed);
  //axis->SetLabelSize(1);
  //axis->Draw("sames");



  
}
