#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"

using namespace std;

namespace argon_target {
double density(double temperature_K, double pressure_bar)
{
    const double R = 8.31;          // J/(mol K)
    const double M = 39.948e-3;     // kg/mol
    if (!isfinite(temperature_K) || !isfinite(pressure_bar)
        || temperature_K <= 0 || pressure_bar <= 0)
        throw runtime_error("Temperature [K] and absolute pressure [bar] must be finite and positive");

    // bar -> Pa; P M / (R T) gives kg/m^3; 0.001 converts to g/cm^3.
    const double rho = pressure_bar * 1e5 * M / (R * temperature_K) * 0.001;
    if (!isfinite(rho) || rho <= 0)
        throw runtime_error("Invalid ideal-gas Ar density");
    return rho;
}
} // namespace argon_target

void target_thickness(double temperature_K = 180.0, double pressure_bar = 1.5,
                      const char *srim_file = "",
                      double L_target = 0.5, double dx = 0.001,
                      double proton_energy_MeV = 3.5)
{
    const double E_min = 3.0;      // MeV
    const double E_max = 4.0;      // MeV
    const double E_step = 0.1;     // MeV
    if (!isfinite(proton_energy_MeV) || proton_energy_MeV <= 0)
        throw runtime_error("Incident proton energy [MeV] must be finite and positive");
    if (!isfinite(L_target) || L_target <= 0 || !isfinite(dx) || dx <= 0)
        throw runtime_error("Target length and integration step must be positive");
    const double rho = argon_target::density(temperature_K, pressure_bar);
    const TString macro_dir = gSystem->DirName(__FILE__);
    const TString input_path = srim_file[0] ? TString(srim_file)
        : macro_dir + "/../data/SRIM_Argon_proton.root";
    unique_ptr<TFile> input(TFile::Open(input_path, "READ"));
    if (!input || input->IsZombie())
        throw runtime_error(string("Cannot open SRIM ROOT file: ") + input_path.Data());
    auto tree = input->Get<TTree>("srim");
    if (!tree || tree->GetEntries() < 2)
        throw runtime_error("Expected TTree 'srim' with at least two entries");
    double energy_value, electronic, nuclear;
    if (tree->SetBranchAddress("Energy_MeV", &energy_value) < 0
        || tree->SetBranchAddress("dEdx_Elec", &electronic) < 0
        || tree->SetBranchAddress("dEdx_Nuclear", &nuclear) < 0)
        throw runtime_error("Missing or incompatible SRIM branches");
    vector<double> energy, stopping;
    for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
        if (tree->GetEntry(i) <= 0)
            throw runtime_error("Cannot read SRIM entry");
        if (!isfinite(energy_value) || !isfinite(electronic) || !isfinite(nuclear)
            || energy_value <= 0 || electronic < 0 || nuclear < 0
            || (!energy.empty() && energy_value <= energy.back()))
            throw runtime_error("SRIM energies must increase and stopping powers must be finite and nonnegative");
        energy.push_back(energy_value);
        // keV/(mg/cm^2) is numerically identical to MeV cm^2/g.
        stopping.push_back(electronic + nuclear);
    }
    input->Close();
    if (proton_energy_MeV < energy.front() || proton_energy_MeV > energy.back())
        throw runtime_error("Incident proton energy outside SRIM range");
    cout << "Read " << energy.size() << " SRIM points from " << input_path << endl;
    TGraph stopping_graph(energy.size(), energy.data(), stopping.data());

    // Shared integration for both the incident-energy scan and depth profile.
    // Do not silently extrapolate outside the supplied SRIM energy interval.
    auto propagate = [&](double Ein, vector<double> *positions,
                         vector<double> *profile) {
        double E = Ein;
        double x = 0;
        if (positions) positions->push_back(0);
        if (profile) profile->push_back(E);
        while (x < L_target) {
            if (E < energy.front() || E > energy.back())
                throw runtime_error("Energy outside SRIM range; supply a wider SRIM table");
            const double step = min(dx, L_target - x);
            const double next = E - stopping_graph.Eval(E) * rho * step;
            if (!isfinite(next) || next < energy.front())
                throw runtime_error("Energy falls below SRIM minimum before target exit; supply lower-energy SRIM data");
            if (x + step == x)
                throw runtime_error("Integration step too small to advance position");
            x += step;
            E = next;
            if (positions) positions->push_back(x * 10); // cm -> mm
            if (profile) profile->push_back(E);
        }
        return E;
    };

    // ============================================================
    // Target properties
    // ============================================================

    double mass_thickness = rho * L_target;

    cout << endl;
    cout << "==========================================" << endl;
    cout << " Ar target" << endl;
    cout << "==========================================" << endl;

    cout << "Temperature      = " << temperature_K << " K" << endl;
    cout << "Absolute pressure = " << pressure_bar << " bar" << endl;
    cout << "Density model    = ideal gas (Z = 1 assumed)" << endl;

    cout << "Density          = "
         << rho << " g/cm^3" << endl;

    cout << "Target length    = "
         << L_target << " cm" << endl;

    cout << "Mass thickness   = "
         << mass_thickness << " g/cm^2" << endl;

    cout << "                 = "
         << mass_thickness * 1000.
         << " mg/cm^2" << endl;

    cout << endl;

    // ============================================================
    // Calculate energy loss
    // ============================================================

    vector<double> Ein_vec;
    vector<double> Eout_vec;
    vector<double> dE_vec;

    cout << "Ein [MeV]   Eout [MeV]   DeltaE [MeV]"
         << endl;

    cout << "----------------------------------------"
         << endl;

    for (double Ein = E_min;
         Ein <= E_max + 1e-9;
         Ein += E_step)
    {
        const double E = propagate(Ein, nullptr, nullptr);

        double DeltaE = Ein - E;

        Ein_vec.push_back(Ein);
        Eout_vec.push_back(E);
        dE_vec.push_back(DeltaE);

        cout << Form("%7.3f     %7.3f       %7.3f",
                     Ein, E, DeltaE)
             << endl;
    }

    // Calculate the profile before writing any PDFs, so a failed integration
    // does not leave a partially updated set of plots.
    vector<double> xpos, Eprofile;
    const double profile_exit = propagate(proton_energy_MeV, &xpos, &Eprofile);
    // Energy-equivalent target thickness depends on the incident proton energy.
    // Use the integrated loss, including the change in stopping power with E.
    const double energy_thickness_MeV = proton_energy_MeV - profile_exit;
    cout << "\nSelected proton: Ein = " << proton_energy_MeV
         << " MeV, Eout = " << profile_exit
         << " MeV" << endl;
    cout << "Energy thickness = " << energy_thickness_MeV << " MeV"
         << " (" << energy_thickness_MeV * 1000 << " keV)" << endl;

    // ============================================================
    // Plot DeltaE vs incident energy
    // ============================================================

    TGraph *gLoss =
        new TGraph(Ein_vec.size(),
                   Ein_vec.data(),
                   dE_vec.data());

    gLoss->SetTitle(
        "Proton Energy Loss in Ar;"
        "Incident Proton Energy [MeV];"
        "#DeltaE [MeV]"
    );

    gLoss->SetMarkerStyle(20);
    gLoss->SetLineWidth(2);

    TCanvas *c1 =
        new TCanvas("c1",
                    "Energy Loss",
                    800, 600);

    gLoss->Draw("APL");

    c1->SaveAs(macro_dir + "/target_thickness.pdf");

    // ============================================================
    // Detailed energy profile for the selected incident proton energy
    // ============================================================

    TGraph *gProfile =
        new TGraph(xpos.size(),
                   xpos.data(),
                   Eprofile.data());

    gProfile->SetTitle(Form(
        "%.8g MeV Proton in Ar;"
        "Position in Ar [mm];"
        "Proton Energy [MeV]", proton_energy_MeV));

    gProfile->SetLineWidth(2);

    TCanvas *c2 =
        new TCanvas("c2",
                    "Energy Profile",
                    800, 600);

    gProfile->Draw("AL");

    TString energy_label = Form("%.8g", proton_energy_MeV);
    energy_label.ReplaceAll(".", "p");
    c2->SaveAs(macro_dir + "/energy_profile_" + energy_label + "MeV.pdf");
}
