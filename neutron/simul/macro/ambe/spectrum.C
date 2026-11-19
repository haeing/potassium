#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

void spectrum(const char* filename="ambe.root",
              const char* reference="reference_measured.dat") {
  TFile file(filename, "READ");
  auto tree=file.Get<TTree>("neutrons");
  if(file.IsZombie() || !tree) throw std::runtime_error("Cannot read neutron tree");
  double energy=0;
  tree->SetBranchAddress("energy_MeV", &energy);
  TH1D histogram("neutron_energy", ";Neutron energy [MeV];Normalized density [MeV^{-1}]", 120, 0, 12);
  histogram.SetDirectory(nullptr);
  histogram.Sumw2();
  for(Long64_t i=0; i<tree->GetEntries(); ++i) {
    tree->GetEntry(i);
    histogram.Fill(energy);
  }
  // Reference values are densities extracted from the measured red curve in
  // Fig. B.5, printed p.117 of ALARM-CF-CU-SHIELD-001 (2022).
  // It covers 1--11 MeV only. Normalize BOTH distributions over that interval;
  // do not treat the missing reference below 1 MeV as a measured zero.
  TH1D measured("measured_ambe", "", 100, 1, 11);
  measured.SetDirectory(nullptr);
  std::ifstream input(reference);
  if(!input) throw std::runtime_error("Cannot open measured spectrum data");
  std::string line;
  int rows=0;
  while(std::getline(input,line)) {
    if(line.empty() || line[0]=='#') continue;
    double low,high,density,error;
    std::istringstream row(line);
    if(!(row>>low>>high>>density>>error) || rows>=100 ||
       std::abs(low-(1+rows*0.1))>1e-6 || std::abs(high-low-0.1)>1e-6 ||
       density<0 || error<0) throw std::runtime_error("Invalid measured spectrum bin");
    measured.SetBinContent(rows+1,density);
    measured.SetBinError(rows+1,error);
    ++rows;
  }
  if(rows!=100) throw std::runtime_error("Expected 100 measured bins from 1 to 11 MeV");
  const auto commonCount=histogram.Integral(11,110);
  const auto referenceArea=measured.Integral("width");
  if(commonCount<=0 || referenceArea<=0) throw std::runtime_error("Empty spectrum");
  histogram.Scale(1.0/commonCount,"width");
  measured.Scale(1.0/referenceArea);
  gStyle->SetOptStat(0);
  TCanvas canvas("spectrum", "AmBe neutron energy spectrum", 1000, 650);
  canvas.SetLeftMargin(0.12);
  canvas.SetBottomMargin(0.13);
  histogram.SetMinimum(0);
  histogram.SetMaximum(1.25*std::max(histogram.GetMaximum(),measured.GetMaximum()));
  histogram.SetLineColor(kBlue+1);
  histogram.SetMarkerColor(kBlue+1);
  histogram.SetMarkerStyle(20);
  histogram.SetMarkerSize(0.45);
  histogram.SetLineWidth(2);
  histogram.Draw("E1");
  measured.SetLineColor(kRed+1);
  measured.SetLineWidth(2);
  measured.Draw("HIST SAME");
  TLegend legend(0.48,0.75,0.88,0.88);
  legend.SetBorderSize(0);
  legend.SetFillStyle(0);
  legend.SetTextSize(0.03);
  legend.AddEntry(&histogram,"Geant4 simulation (50 million events)","lep");
  legend.AddEntry(&measured,"Measured AmBe (Kostal et al.; digitized)","l");
  legend.Draw();
  TLatex text;
  text.SetNDC();
  text.SetTextSize(0.028);
  text.DrawLatex(0.13,0.94,"Both spectra normalized to unit area over 1-11 MeV");
  text.SetTextSize(0.024);
  text.DrawLatex(0.12,0.025,"Reference: ALARM-CF-CU-SHIELD-001 (2022), Fig. B.5; measured curve extracted from PDF");
  std::cout << "Simulation neutrons: " << tree->GetEntries()
            << "; 1-11 MeV: " << commonCount << std::endl;
  canvas.SaveAs("spectrum.pdf");
}
