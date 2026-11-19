#ifndef POTASSIUM_KINEMATICS_GAS_TARGET_HH
#define POTASSIUM_KINEMATICS_GAS_TARGET_HH
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>

namespace kinematics {
// Same defaults and ideal-gas density convention as Target-Thickness.md.
constexpr double gas_temperature_k = 180.0;
constexpr double gas_pressure_bar = 1.5;
constexpr double gas_length_cm = 0.5;
constexpr double gas_step_cm = 0.001;
constexpr double gas_constant_j_mol_k = 8.31;
constexpr double argon_molar_mass_kg_mol = 0.039948;

struct GasProfile {
    std::vector<double> depth_cm, energy_mev;
    double density_g_cm3;
    double energy(double depth) const {
        const auto upper = std::upper_bound(depth_cm.begin(), depth_cm.end(), depth);
        if (upper == depth_cm.begin()) return energy_mev.front();
        if (upper == depth_cm.end()) return energy_mev.back();
        const auto i = static_cast<std::size_t>(upper-depth_cm.begin()-1);
        const double f = (depth-depth_cm[i])/(depth_cm[i+1]-depth_cm[i]);
        return energy_mev[i]+f*(energy_mev[i+1]-energy_mev[i]);
    }
};

inline GasProfile gas_profile(double incident, const char* srim_path,
                              double temperature, double pressure,
                              double length, double step = gas_step_cm) {
    for (double value : {incident, temperature, pressure, length, step})
        if (!std::isfinite(value) || value<=0) throw std::runtime_error("Invalid gas profile input.");
    GasProfile result;
    result.density_g_cm3 = pressure*1e5*argon_molar_mass_kg_mol /
                          (gas_constant_j_mol_k*temperature)*0.001;
    std::unique_ptr<TFile> input(TFile::Open(srim_path,"READ"));
    if (!input || input->IsZombie()) throw std::runtime_error("Cannot open SRIM ROOT file.");
    auto* tree = input->Get<TTree>("srim");
    if (!tree || tree->GetEntries()<2) throw std::runtime_error("Missing SRIM tree.");
    double e, electronic, nuclear;
    if (tree->SetBranchAddress("Energy_MeV",&e)<0 ||
        tree->SetBranchAddress("dEdx_Elec",&electronic)<0 ||
        tree->SetBranchAddress("dEdx_Nuclear",&nuclear)<0)
        throw std::runtime_error("Missing SRIM branches.");
    std::vector<double> energies, stopping;
    for (Long64_t i=0; i<tree->GetEntries(); ++i) {
        if (tree->GetEntry(i)<=0 || !std::isfinite(e) || !std::isfinite(electronic) ||
            !std::isfinite(nuclear) || e<=0 || electronic<0 || nuclear<0 ||
            (!energies.empty() && e<=energies.back())) throw std::runtime_error("Invalid SRIM table.");
        energies.push_back(e);
        stopping.push_back(electronic+nuclear); // keV/(mg/cm2) = MeV cm2/g.
    }
    TGraph graph(energies.size(),energies.data(),stopping.data());
    double x=0, current=incident;
    result.depth_cm.push_back(x);
    result.energy_mev.push_back(current);
    while (x<length) {
        if (current<energies.front() || current>energies.back())
            throw std::runtime_error("Energy outside SRIM table.");
        const double h=std::min(step,length-x);
        const double next=current-graph.Eval(current)*result.density_g_cm3*h;
        if (!std::isfinite(next) || next<energies.front() || x+h==x)
            throw std::runtime_error("Cannot advance SRIM integration within table range.");
        x+=h; current=next;
        result.depth_cm.push_back(x);
        result.energy_mev.push_back(current);
    }
    return result;
}
}
#endif
