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

// root -l -b -q 'neutron_energy_beam_90.cc(10.0)'
// p + stationary 40Ar -> n + 40K*, at exactly 90 degrees in LAB.
// Same neutral-atom mass approximation as neutron_energy_angle.cc.
// Only the four selected levels in constants.hh are included, not all open levels.
void neutron_energy_beam_90(double max_beam_mev = kinematics::default_max_beam_mev) {
    using namespace kinematics;
    apply_plot_style();
    if (!std::isfinite(max_beam_mev) || max_beam_mev <= 0)
        throw std::invalid_argument("Maximum beam energy must be finite and positive.");
    const double mp = hydrogen1_u * atomic_mass_mev;
    const double mt = argon40_u * atomic_mass_mev;
    const double mn = neutron_mass_mev;
    const double base = mp + mt - mn;
    const int colors[] = {kBlack, kBlue + 1, kRed + 1, kGreen + 2};
    auto* curves = new TMultiGraph();
    curves->SetTitle("^{40}Ar(p,n)^{40}K, #theta_{n,LAB} = 90#circ;E_{p,LAB} [MeV];T_{n,LAB} [MeV]");
    auto* legend = new TLegend(0.15, 0.65, 0.52, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    unsigned count = 0;
    for (unsigned j = 0; j < potassium40_excitation_mev.size(); ++j) {
        const double ex = potassium40_excitation_mev[j];
        const double mr = potassium40_mass_mev + ex;
        // At 90 deg p_n dot p_beam = 0. The residual mass shell gives
        // T_n = [(base^2-mr^2)+2(mt-mn)T_beam]/[2(mp+mt+T_beam)].
        // The 90-degree onset is slightly above the total reaction threshold.
        const double onset = (mr-base)*(mr+base)/(2*(mt-mn));
        std::cout << "Ex = " << ex << " MeV: LAB 90 deg onset = " << onset << " MeV\n";
        if (max_beam_mev <= onset) continue;
        auto* graph = new TGraph(graph_intervals + 1);
        for (int i = 0; i <= graph_intervals; ++i) {
            const double beam = onset + (max_beam_mev-onset)*i/graph_intervals;
            const double tn = (mt-mn)*(beam-onset)/(mp+mt+beam);
            graph->SetPoint(i, beam, tn);
            const double pn2 = tn*(tn+2*mn);
            const double pb2 = beam*(beam+2*mp);
            const double recoil_energy = std::sqrt(mr*mr+pb2+pn2);
            if (std::abs(mp+mt+beam-mn-tn-recoil_energy) > 1e-7)
                throw std::runtime_error("Energy conservation check failed.");
        }
        graph->SetLineColor(colors[j]);
        graph->SetLineWidth(3);
        curves->Add(graph, "L");
        legend->AddEntry(graph, Form("^{40}K E_{x} = %.6f MeV", ex), "l");
        ++count;
    }
    if (!count) { std::cout << "No selected channels accessible at 90 deg.\n"; return; }
    auto* canvas = new TCanvas("neutron_beam_90", "Neutron energy at LAB 90 deg", plot_canvas_size, plot_canvas_size);
    canvas->SetLeftMargin(0.16);
    canvas->SetBottomMargin(0.15);
    canvas->SetGrid();
    curves->Draw("A");
    curves->GetXaxis()->SetLimits(0, max_beam_mev);
    curves->SetMinimum(0);
    legend->Draw();
    canvas->SaveAs("neutron_energy_beam_90.png");
    canvas->SaveAs("neutron_energy_beam_90.pdf");
}
