double ToF_n_cal(double E_n){
  double length = 5; //m
  double nm = 939.565; //MeV/c2
  double c = 299792458;


  return TMath::Sqrt(nm/(2*E_n))*length/c*pow(10,9); //ns
}

void kin_n2(){

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
  
  double m1 = Clm;
  double m2 = alpham;
  double m3 = nm;
  double m4 = K40m;

  double Q = (m1+m2-m3-m4);
  
  //double Et = E_beam+Q;
  

  //Ar(p,n)K
  //double E_beam_min = 3.3; //degree
  //double E_beam_max = 6;

  //Cl(alpha,n)K
  double E_beam_min = 55; //degree
  double E_beam_max = 80;

  
  int theta_min = 0;
  int theta_max = 180;
  const int num_theta = 10;
  double theta[num_theta+1];
  
  int bin = 2000;
  double step = (E_beam_max-E_beam_min)/bin;
  TMultiGraph *mg = new TMultiGraph();
  double E_n[4][num_theta+1][bin];
  double E_n_range[4][bin];
  double E_n_center[4][bin];
  double E_beam[bin];
  double E_beam_err[bin];
  TLegend *l1 = new TLegend(0.8,0.5,0.48,0.6);
  int count = 0;
  TGraphErrors *graph[4];
  for(int j=0;j<4;j++){
    for(int i=0;i<num_theta+1;i++){
      for(int n=0;n<bin;n++){

	E_beam[n] = E_beam_min+n*step; //MeV
	E_beam_err[n] = 0;
	double theta_step = (theta_max - theta_min)/num_theta;
	
	theta[i] = theta_min + theta_step * i;
	
	//double theta = theta_min+n*step;
	//double s = (E_beam*(m4-m1)+m4*Q)/(m3+m4);
	double s = (E_beam[n]*(m4-m1)+m4*Q-m4*K40_state[j])/(m3+m4);
	double r = TMath::Sqrt(m1*m3*E_beam[n])/(m3+m4)*TMath::Cos(theta[i]*TMath::Pi()/180);

	if(r+TMath::Sqrt(r*r+s)>=0){
	  E_n[j][i][n] = pow(r+TMath::Sqrt(r*r+s),2);
	  count++;
	}
	else if(r-TMath::Sqrt(r*r+s)>0){
	  E_n[j][i][n] = pow(r-TMath::Sqrt(r*r+s),2);
	  count++;
	}

	if(i==num_theta){
	  E_n_range[j][n] = (E_n[j][num_theta][n]-E_n[j][0][n]);
	  E_n_center[j][n] = (E_n[j][num_theta][n]+E_n[j][0][n])/2;
	}

      }


    }
    
    graph[j] = new TGraphErrors(bin,E_beam,E_n_center[j],E_beam_err,E_n_range[j]);
    if(j%2==1)graph[j]->SetFillStyle(3006);
    else if(j%2==0)graph[j]->SetFillStyle(3007);
    graph[j]->SetFillColorAlpha(j+1,0.35);
    graph[j]->SetLineColorAlpha(j+1,0);
    graph[j]->SetMarkerColor(j+1);
    
    mg->Add(graph[j]);
    
    

    
  }
  l1->AddEntry(graph[0],"^{40}K Ground state");
  l1->AddEntry(graph[1],"^{40}K  29.82 keV Excited state");
  l1->AddEntry(graph[2],"^{40}K  800.14 keV Excited state");
  l1->AddEntry(graph[3],"^{40}K  891.39 keV Excited state");
  
  mg->SetTitle("Kinematics - neutron;E_{beam} [MeV];E_{n} [MeV]");
  mg->GetXaxis()->SetRangeUser(E_beam_min,E_beam_max);
  //mg->GetYaxis()->SetRangeUser(0.1,4);
  mg->GetYaxis()->SetRangeUser(0.1,30);


  cout<<ToF_n_cal(1)<<endl;
  cout<<ToF_n_cal(7)<<endl;
  

  
  
  
  
  TCanvas *c1 = new TCanvas("c1","c1");
  mg->Draw("a3");
  l1->Draw();

}
