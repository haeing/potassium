#include "constants.hh"
#include "plot_style.hh"
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TString.h>
#include <TAxis.h>
#include <iostream>
#include <stdexcept>

// Run in kinematics/: root -l -b -q 'neutron_energy_angle.cc(3.5)'
// p + stationary 40Ar -> n + 40K*, relativistic two-body kinematics.
// Atomic mass convention matches beam_CM.cc; electron corrections neglected.
// Curves represent allowed energies, not angular distributions or yields.
void neutron_energy_angle(double beam_mev = kinematics::default_proton_beam_mev) {
    using namespace kinematics;
    apply_plot_style();
    if (!std::isfinite(beam_mev) || beam_mev <= 0)
        throw std::invalid_argument("Beam energy must be finite and positive.");
    const double mp = hydrogen1_u * atomic_mass_mev;
    const double mt = argon40_u * atomic_mass_mev;
    const double mn = neutron_mass_mev;
    const double s = (mp + mt) * (mp + mt) + 2 * mt * beam_mev;
    const double w = std::sqrt(s);
    const double beta = std::sqrt(beam_mev * (beam_mev + 2 * mp)) / (mp + mt + beam_mev);
    const double gamma = (mp + mt + beam_mev) / w;
    const double pi = std::acos(-1.0);
    const int colors[] = {kBlack, kBlue + 1, kRed + 1, kGreen + 2};
    auto* curves = new TMultiGraph();
    curves->SetTitle(Form("^{40}Ar(p,n)^{40}K, E_{p,LAB} = %.3f MeV;#theta_{n,LAB} [deg];T_{n,LAB} [MeV]", beam_mev));
    auto* legend = new TLegend(0.48, 0.42, 0.88, 0.65);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    int open_channels = 0;
    for (unsigned j = 0; j < potassium40_excitation_mev.size(); ++j) {
        const double ex = potassium40_excitation_mev[j];
        const double residual = potassium40_mass_mev + ex;
        if (w <= mn + residual) {
            std::cout << "Closed channel: Ex = " << ex << " MeV\n";
            continue;
        }
        // Factored Kallen function avoids subtraction of large squared masses.
        const double pstar = std::sqrt((w-mn-residual)*(w+mn+residual)*
                                      (w-mn+residual)*(w+mn-residual)) / (2*w);
        const double estar = std::hypot(mn, pstar);
        const double tstar = pstar*pstar / (estar+mn);
        auto* graph = new TGraph(graph_intervals + 1);
        for (int i = 0; i <= graph_intervals; ++i) {
            const double theta_cm = pi*i/graph_intervals;
            const double pz = gamma*(pstar*std::cos(theta_cm)+beta*estar);
            const double pt = pstar*std::sin(theta_cm);
            const double theta_lab = std::atan2(pt, pz)*180/pi;
            const double energy = tstar + gamma*gamma*beta*beta/(gamma+1)*estar
                                  + gamma*beta*pstar*std::cos(theta_cm);
            graph->SetPoint(i, theta_lab, energy);
            // Check energy conservation including the recoiling residual nucleus.
            const double residual_lab_energy = gamma*(std::hypot(residual,pstar)-beta*pstar*std::cos(theta_cm));
            if (std::abs((mn+energy+residual_lab_energy)-(mp+mt+beam_mev)) > 1e-7)
                throw std::runtime_error("Energy conservation check failed.");
        }
        graph->SetLineWidth(3);
        graph->SetLineColor(colors[j]);
        curves->Add(graph, "L");
        legend->AddEntry(graph, Form("^{40}K E_{x} = %.6f MeV", ex), "l");
        ++open_channels;
    }
    if (!open_channels) { std::cout << "No open selected channels.\n"; return; }
    auto* canvas = new TCanvas("neutron_angle", "Neutron energy vs LAB angle", plot_canvas_size, plot_canvas_size);
    canvas->SetLeftMargin(0.16);
    canvas->SetBottomMargin(0.15);
    canvas->SetGrid();
    curves->Draw("A");
    curves->GetXaxis()->SetLimits(0,180);
    curves->SetMinimum(0);
    legend->Draw();
    canvas->SaveAs("neutron_energy_angle.png");
    canvas->SaveAs("neutron_energy_angle.pdf");
}
