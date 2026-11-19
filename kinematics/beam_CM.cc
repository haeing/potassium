#include "constants.hh"
#include "plot_style.hh"
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TStyle.h>
#include <array>
#include <stdexcept>

// Run from kinematics/: root -l -b -q 'beam_CM.cc(10.0)'
// E_beam is the total LAB kinetic energy, not energy per nucleon.
// E_CM is entrance-channel kinetic energy, not 41K excitation energy.
void beam_CM(double max_beam_mev = kinematics::default_max_beam_mev) {
    using namespace kinematics;
    apply_plot_style();
    if (!std::isfinite(max_beam_mev) || max_beam_mev <= 0.0)
        throw std::invalid_argument("Maximum beam energy must be finite and positive.");

    struct Channel { double projectile, target; const char* label; int color; };
    const std::array<Channel, 4> channels = {{
        {hydrogen1_u, argon40_u, "p beam + ^{40}Ar target", kBlue + 1},
        {argon40_u, hydrogen1_u, "^{40}Ar beam + p target", kAzure + 7},
        {helium4_u, chlorine37_u, "#alpha beam + ^{37}Cl target", kRed + 1},
        {chlorine37_u, helium4_u, "^{37}Cl beam + #alpha target", kOrange + 7}
    }};
    gStyle->SetOptStat(0);
    auto* canvas = new TCanvas("beam_cm", "Beam and CM energy", plot_canvas_size, plot_canvas_size);
    canvas->SetLeftMargin(0.16);
    canvas->SetBottomMargin(0.15);
    canvas->SetGrid();
    auto* curves = new TMultiGraph();
    curves->SetTitle(";E_{beam, LAB} [MeV];E_{CM} [MeV]");
    auto* legend = new TLegend(0.16, 0.66, 0.60, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    for (const auto& channel : channels) {
        auto* graph = new TGraph(graph_intervals + 1);
        for (int i = 0; i <= graph_intervals; ++i) {
            const double beam = max_beam_mev * i / graph_intervals;
            graph->SetPoint(i, beam, cm_energy(beam, channel.projectile, channel.target));
        }
        graph->SetLineColor(channel.color);
        graph->SetLineWidth(3);
        curves->Add(graph, "L");
        legend->AddEntry(graph, channel.label, "l");
    }
    curves->Draw("A");
    curves->GetXaxis()->SetLimits(0.0, max_beam_mev);
    curves->SetMinimum(0.0);
    curves->SetMaximum(max_beam_mev);
    legend->Draw();
    canvas->SaveAs("beam_CM.png");
    canvas->SaveAs("beam_CM.pdf");
}
