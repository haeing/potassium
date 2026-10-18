void kin_n1(){

  //AMU methode (atomic mass)
  double amu = 931.494; //MeV/c2
  double em = 0.511; //MeV/c2
  double c  = 299792458;

  double Arm = 39.96238312*amu+em; //Z = 18, N = 22 e = -1
  double pm = 938.272;
  double nm = 939.565;
  double Clm = 36.9659026*amu+em; //Z = 17, N = 20 
  double alpham = 4.002603*amu;
  double K41m = 40.961825*amu;
  double K40m = 39.963998*amu;



  double K40_state[4];
  K40_state[0] = 0;
  K40_state[1] = 0.0298;
  K40_state[2] = 0.8001;
  K40_state[3] = 0.8914;
  
  //double m1 = pm;
  //double m2 = Arm;
  double m1 = alpham;
  double m2 = Clm;
  double m3 = nm;
  double m4 = K40m;

  double Q = (m1+m2-m3-m4);
  
  //double Et = E_beam+Q;
  
  

  
  

  TGraph *kinematics[6];
  

  double E_beam_min = 2.3; //degree
  double E_beam_max = 6;

  double theta[6];
  theta[0] = 0;
  theta[1] = 30;
  theta[2] = 60;
  theta[3] = 90;
  theta[4] = 120;
  theta[5] = 180;

  int bin = 2000;
  double step = (E_beam_max-E_beam_min)/bin;
  TMultiGraph *mg = new TMultiGraph();

  TLegend *l1 = new TLegend(0.8,0.5,0.48,0.6);
  int count = 0;
  for(int i=0;i<6;i++){
    kinematics[i] = new TGraph();
    for(int n=0;n<bin;n++){

      double E_beam = E_beam_min+n*step; //MeV
      //double theta = theta_min+n*step;
      //double s = (E_beam*(m4-m1)+m4*Q)/(m3+m4);
      double s = (E_beam*(m4-m1)+m4*Q-m4*K40_state[3])/(m3+m4);
      double r = TMath::Sqrt(m1*m3*E_beam)/(m3+m4)*TMath::Cos(theta[i]*TMath::Pi()/180);

      if(r+TMath::Sqrt(r*r+s)>0){
	kinematics[i]->AddPoint(E_beam,pow(r+TMath::Sqrt(r*r+s),2));
	count++;
      }
      if(r-TMath::Sqrt(r*r+s)>0){
	kinematics[i]->AddPoint(E_beam,pow(r-TMath::Sqrt(r*r+s),2));
	count++;
      }


    }
    kinematics[i]->SetLineColor(i+1);
    kinematics[i]->SetMarkerColor(i+1);
    kinematics[i]->SetMarkerStyle(7);
    kinematics[i]->SetMarkerSize(2);

    //kinematics[i]->SetMarkerStyle(8);
    //kinematics[i]->SetMarkerSize(0.5);

    mg->Add(kinematics[i]);
    //l1->AddEntry(kinematics[i],Form("^{40}K state : %.1f keV",K40_state[i]*1000));
    l1->AddEntry(kinematics[i],Form("#theta_{Lab} : %.0f #circ",theta[i]));
  }
  //mg->SetTitle("Kinematics - neutron;#theta_{Lab} [degree];E_{n} [MeV]");
  mg->SetTitle("Kinematics - neutron;E_{beam} [MeV];E_{n} [MeV]");
  
  
  TCanvas *c1 = new TCanvas("c1","c1");
  mg->Draw("ap");
  l1->Draw();
}
