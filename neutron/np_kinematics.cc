void np_kinematics()
{
  // --- constants (MeV, c=1)
  const double m_n = 939.5654133;
  const double m_p = 938.2720813;
  const double T0  = 3.0;                    

  // incident neutron in LAB (along +z)
  const double E_n0 = m_n + T0;
  const double p_n0 = std::sqrt(E_n0*E_n0 - m_n*m_n);

  TLorentzVector pn_lab(0., 0., p_n0, E_n0);
  TLorentzVector pp_lab(0., 0., 0.,   m_p);

  // total 4-momentum and CM boost
  TLorentzVector Ptot = pn_lab + pp_lab;
  const TVector3 beta_cm = Ptot.BoostVector();    // CM velocity vector (β⃗)
  const double   sqs     = std::sqrt(Ptot.M2());  // √s

  TLorentzVector pn_cm = pn_lab;
  pn_cm.Boost(-beta_cm);
  const double p_star  = pn_cm.P();
  const double E_star  = pn_cm.E();

  TH2F* h2 = new TH2F("h2", "n + p (elastic), T_{0}=3 MeV;#theta_{lab} [deg];T_{lab} [MeV]",
                      180, 0.0, 180.0,
                      200, 0.0, 1.2*T0);

  TRandom3 rng(0);
  for (Long64_t i = 0; i < 200000; ++i) {
    const double u   = rng.Uniform(-1.0, 1.0);     // cosθ*
    const double ths = std::acos(u);               // θ*
    const double phi = rng.Uniform(0.0, 2.0*TMath::Pi());

    // neutron momentum in CM after scattering
    const double pxs = p_star * std::sin(ths) * std::cos(phi);
    const double pys = p_star * std::sin(ths) * std::sin(phi);
    const double pzs = p_star * std::cos(ths);

    TLorentzVector pn_cm_scatt(pxs, pys, pzs, E_star);

    // boost back to LAB
    TLorentzVector pn_lab_scatt = pn_cm_scatt;
    pn_lab_scatt.Boost(beta_cm);

    const double p_lab = pn_lab_scatt.P();
    const double ez    = pn_lab_scatt.Pz();
    const double E_lab = pn_lab_scatt.E();
    const double T_lab = E_lab - m_n;

    // lab scattering angle (relative to +z beam axis)
    double theta_lab_deg = 0.0;
    if (p_lab > 0.0) {
      double cos_th_lab = ez / p_lab;
      if (cos_th_lab >  1.0) cos_th_lab =  1.0;
      if (cos_th_lab < -1.0) cos_th_lab = -1.0;
      theta_lab_deg = std::acos(cos_th_lab) * 180.0 / TMath::Pi();
    }

    h2->Fill(theta_lab_deg, T_lab);
  }

  gStyle->SetOptStat(0);
  TCanvas* c1 = new TCanvas("c1","n–p elastic kinematics", 900, 700);
  h2->SetContour(100);
  h2->Draw("COLZ");
  c1->SetRightMargin(0.15);
  c1->Update();

}
