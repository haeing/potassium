//Type of Gas
/*
  0 : 4He
  1 : 40Ar
  Properties are based on "National Bureau of Standards" [Temperature : K, Pressure : Pa]
*/

const int num_type = 2;
const int Gas_type = 1;



//Lee-Kesler Constants
const double R = 8.31; //[J/(mol K)]

//Simple constants
const double b1_s = 0.1181193;
const double b2_s = 0.265728;
const double b3_s = 0.154790;
const double b4_s = 0.030323;
const double c1_s = 0.0236744;
const double c2_s = 0.0186984;
const double c3_s = 0.0;
const double c4_s = 0.042724;
const double d1_s = 0.155488*pow(10,-4);
const double d2_s = 0.623689*pow(10,-4);
const double beta_s = 0.65392;
const double gamma_s = 0.060167;

//Reference constants
const double b1_r = 0.2026579;
const double b2_r = 0.331511;
const double b3_r = 0.027655;
const double b4_r = 0.203488;
const double c1_r = 0.0313385;
const double c2_r = 0.0503618;
const double c3_r = 0.016901;
const double c4_r = 0.041577;
const double d1_r = 0.48736*pow(10,-4);
const double d2_r = 0.0740366*pow(10,-4);
const double beta_r = 1.226;
const double gamma_r = 0.03754;

const double w_r = 0.3978; //Pitzer's acentric factor (reference value)


double f_Vr(int n, double Vr, double Pr, double Tr, double B, double C, double D, double c4, double beta, double gamma){
  double f2 = -1*Pr/Tr;
  double f3 = -1*B/(Vr*Vr);
  double f4 = -2*C/pow(Vr,3);
  double f5 = -5*D/pow(Vr,6);
  double f6 = (2*c4*beta/(pow(Tr,3)*pow(Vr,3)))*(gamma/(Vr*Vr)-1)*TMath::Exp(-1*gamma/(Vr*Vr));
  double f7 = (2*c4*gamma/(pow(Tr,3)*pow(Vr,5)))*(gamma/(Vr*Vr)-2)*TMath::Exp(-1*gamma/(Vr*Vr));
  switch(n)
    {
    case 0 : 
      return 1 - Pr*Vr/Tr + B/Vr + C/(Vr*Vr) + D/pow(Vr,5) + c4*beta/(pow(Tr,3)*Vr*Vr)*TMath::Exp(-1*gamma/(Vr*Vr)) + c4*gamma/(pow(Tr,3)*pow(Vr,4))*TMath::Exp(-1*gamma/(Vr*Vr));
      break;
    case 1 :
      return f2 + f3 + f4 + f5 + f6 + f7;
      break;
    default :
      return -9999;
      break;
    }
}


void cal_density(){

  
  double Tc[2];
  double Pc[2];
  double w[2]; //Acentric factor
  double MM[2]; //molecular mass [kg/mole]
  string Gas_name[2];

  Gas_name[0] = "^{4}He";
  Tc[0] = 5.202; //[K]
  Pc[0] = 227458.09299; //[Pa]
  w[0] = -0.387;
  MM[0] = 4.003 * pow(10,-3);   

  Gas_name[1] = "^{40}Ar";
  Tc[1] = 151.111;
  Pc[1] = 4905619.87669;
  w[1] = -0.004;
  MM[1] = 39.948 * pow(10,-3);  

  const int num_T = 9;
  const int num_P = 10000;
  
  double Tr[num_T];
  double Pr[num_P];

  double Tr_init = 1.2;
  double Pr_init = 0.01;

  double Tr_step[num_type];
  Tr_step[0] = 2;
  Tr_step[1] = 0.05;
  
  double Pr_step[num_type];
  Pr_step[0] = 0.1;
  Pr_step[1] = 0.01;
  
  TGraph *Comp_fact[num_type][num_T];
  for(int type = 0; type <num_type; type++){
    for(int nt=0;nt<num_T;nt++){
      Comp_fact[type][nt] = new TGraph();
      Comp_fact[type][nt] ->SetMarkerColor(nt+1);
      Comp_fact[type][nt] ->SetMarkerStyle(20);
      Comp_fact[type][nt] ->SetLineColor(nt+1);
      Comp_fact[type][nt] ->SetLineWidth(2);
    
      for(int np = 0; np < num_P;np++){

	Tr[nt] = Tr_init + Tr_step[type] * nt;
	Pr[np] = Pr_init + Pr_step[type] * np;

  
	//Redlich-kwong eqaution
	double A_RK = 0.08664*Pr[np]/Tr[nt];
	double B_RK = 4.9340/pow(Tr[nt],3.0/2.0);
	double C_RK = A_RK*A_RK+A_RK-A_RK*B_RK;
	double D_RK = -1*C_RK/3-1.0/9.0;
	double E_RK = C_RK/6+(A_RK*A_RK*B_RK)/2+1.0/27.0;
	double Z_RK;
	if(pow(D_RK,3)+pow(E_RK,2)>=0){
	  Z_RK = cbrt(E_RK+TMath::Sqrt(pow(D_RK,3)+pow(E_RK,2)))+cbrt(E_RK-TMath::Sqrt(pow(D_RK,3)+pow(E_RK,2)))+1.0/3.0;
	}
	else{
	  Z_RK = 2*TMath::Sqrt(-1*D_RK)*TMath::Cos(-1.0/3.0*TMath::ACos(E_RK/TMath::Sqrt(-1*pow(D_RK,3))))+1.0/3.0;
	}
      
  


	//Lee-Kesler equation

	double B_s = b1_s-b2_s/Tr[nt]-b3_s/pow(Tr[nt],2)-b4_s/pow(Tr[nt],3);
	double C_s = c1_s-c2_s/Tr[nt]+c3_s/pow(Tr[nt],3);
	double D_s = d1_s+d2_s/Tr[nt];


	double B_r = b1_r-b2_r/Tr[nt]-b3_r/pow(Tr[nt],2)-b4_r/pow(Tr[nt],3);
	double C_r = c1_r-c2_r/Tr[nt]+c3_r/pow(Tr[nt],3);
	double D_r = d1_r+d2_r/Tr[nt];

	//Newton-Raphson Method
	double Vr0 = Z_RK*Tr[nt]/Pr[np];
	int n_0 = 0;
	while(TMath::Abs(f_Vr(0,Vr0,Pr[np],Tr[nt],B_s,C_s,D_s,c4_s,beta_s,gamma_s))>0.001 && n_0 < 1000){
	  n_0++;
	  double Vr0_diff = -1*f_Vr(0,Vr0,Pr[np],Tr[nt],B_s,C_s,D_s,c4_s,beta_s,gamma_s) / f_Vr(1,Vr0,Pr[np],Tr[nt],B_s,C_s,D_s,c4_s,beta_s,gamma_s);
	  Vr0 += Vr0_diff;

	}

	double Vrr = Z_RK*Tr[nt]/Pr[np];
	int n_r = 0;
	while(TMath::Abs(f_Vr(0,Vrr,Pr[np],Tr[nt],B_r,C_r,D_r,c4_r,beta_r,gamma_r))>0.001 && n_r < 1000){
	  n_r++;
	  double Vrr_diff = -1*f_Vr(0,Vrr,Pr[np],Tr[nt],B_r,C_r,D_r,c4_r,beta_r,gamma_r) / f_Vr(1,Vrr,Pr[np],Tr[nt],B_r,C_r,D_r,c4_r,beta_r,gamma_r);
	  Vrr += Vrr_diff;
	}

  
  
	double Z_0 = Pr[np]*Vr0/Tr[nt];
	double Z_r = Pr[np]*Vrr/Tr[nt];

	double Z_cal = Z_0+(w[type]/w_r)*(Z_r-Z_0);
	
	double rho_cal = Pr[np]*Pc[type]*MM[type]/(R*Tr[nt]*Tc[type]*Z_cal) * 0.001;
	Comp_fact[type][nt]->SetPoint(Comp_fact[type][nt]->GetN(),Pr[np]*Pc[type]/133.3,rho_cal);
      }

    
    
    }
  }

  TLegend *le[num_type];
  TMultiGraph* mg[num_type];
  
  TCanvas *c1 = new TCanvas("c1","c1");
  
  c1->Divide(2);
  for(int n=0;n<num_type;n++){
    c1->cd(n+1);
    gPad->SetLogx();
    gPad->SetGrid();
    mg[n] = new TMultiGraph();
    

    le[n] = new TLegend(0.8,0.5,0.48,0.6);
    mg[n]->SetTitle(Form("%s;Pressure [Torr];Density [g/cm^{3}]",Gas_name[n].c_str()));
    for(int i=0;i<num_T;i++){
      mg[n]->Add(Comp_fact[n][i]);
      le[n]->AddEntry(Comp_fact[n][i],Form("%.1f K",(Tr_init + Tr_step[n] *i)*Tc[n]));
    }
    mg[n]->GetXaxis()->SetLimits(100, 60000);
    mg[n]->GetHistogram()->GetYaxis()->SetRangeUser(0, 200 * 0.001);
    mg[n]->Draw("AL");
    le[n]->Draw();
  }


  
  
}
