#include "constants.hh"
#include "plot_style.hh"
#include "gas_target.hh"
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TPad.h>
#include <TString.h>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <iostream>
#include <stdexcept>

// Run in kinematics/: root -l -b -q 'neutron_tof_90.cc()'
// Arguments: proton LAB energy [MeV], path [m], timing sigma [ns],
// enable SRIM loss, temperature [K], pressure [bar], gas length [cm], SRIM path.
// TOF = detector time - ideal reaction time. Uniform reaction depth, constant
// equal LAB differential cross sections for the selected levels above threshold.
// Energy loss from SRIM; fixed LAB 90 deg and fixed neutron path at every depth.
// No straggling, beam spread, proton transit-time/start jitter, finite geometry,
// efficiency variation, backgrounds, or additional levels are included.
void neutron_tof_90(double beam_mev = kinematics::default_proton_beam_mev,
                    double path_m = kinematics::default_flight_path_m,
                    double sigma_ns = kinematics::default_time_sigma_ns,
                    bool energy_loss = true,
                    double temperature_k = kinematics::gas_temperature_k,
                    double pressure_bar = kinematics::gas_pressure_bar,
                    double length_cm = kinematics::gas_length_cm,
                    const char* srim_file = "") {
    using namespace kinematics;
    apply_plot_style();
    if (!std::isfinite(beam_mev) || beam_mev<=0 || !std::isfinite(path_m) || path_m<=0 ||
        !std::isfinite(sigma_ns) || sigma_ns<=0)
        throw std::invalid_argument("Beam energy, path, and timing sigma must be finite and positive.");
    struct Level { double ex, energy, tof, onset, end_tof; int color; };
    std::vector<Level> levels;
    const int colors[] = {kBlack, kBlue+1, kRed+1, kGreen+2};
    const double mp = hydrogen1_u*atomic_mass_mev;
    const double mt = argon40_u*atomic_mass_mev;
    const double mn = neutron_mass_mev;
    const double base = mp+mt-mn;
    GasProfile profile;
    if (energy_loss) {
        const TString macro_dir = gSystem->DirName(__FILE__);
        const TString source = srim_file[0] ? TString(srim_file) : macro_dir+"/../data/SRIM_Argon_proton.root";
        profile = gas_profile(beam_mev,source.Data(),temperature_k,pressure_bar,length_cm);
        std::cout << "Ar density=" << profile.density_g_cm3 << " g/cm3, proton Ein="
                  << beam_mev << " MeV, Eout=" << profile.energy_mev.back()
                  << " MeV, loss=" << beam_mev-profile.energy_mev.back() << " MeV\n";
    }
    auto tof_at = [&](double beam, double onset) {
        const double energy=(mt-mn)*(beam-onset)/(mp+mt+beam);
        const double beta=std::sqrt(energy*(energy+2*mn))/(energy+mn);
        return path_m/(speed_of_light_m_ns*beta);
    };
    for (unsigned j=0; j<potassium40_excitation_mev.size(); ++j) {
        const double ex = potassium40_excitation_mev[j];
        const double mr = potassium40_mass_mev+ex;
        const double onset = (mr-base)*(mr+base)/(2*(mt-mn));
        if (beam_mev<=onset) continue;
        const double energy = (mt-mn)*(beam_mev-onset)/(mp+mt+beam_mev);
        const double beta = std::sqrt(energy*(energy+2*mn))/(energy+mn);
        // Scan cell midpoints to estimate the full histogram range, including
        // threshold tails. Events outside this finite range go to ROOT overflow.
        double end_tof=path_m/(speed_of_light_m_ns*beta);
        if (energy_loss) for (std::size_t i=1; i<profile.depth_cm.size(); ++i) {
            const double local=profile.energy(0.5*(profile.depth_cm[i-1]+profile.depth_cm[i]));
            if (local>onset) end_tof=std::max(end_tof,tof_at(local,onset));
        }
        if (energy_loss && profile.energy_mev.back()>onset)
            end_tof=std::max(end_tof,tof_at(profile.energy_mev.back(),onset));
        levels.push_back({ex,energy,path_m/(speed_of_light_m_ns*beta),onset,end_tof,colors[j]});
    }
    if (levels.empty()) throw std::runtime_error("No selected channels accessible at LAB 90 deg.");
    const double low = levels.front().tof-6*sigma_ns;
    double high = 0;
    for (const auto& level:levels) high=std::max(high,level.end_tof+6*sigma_ns);
    const int bins = static_cast<int>(std::ceil((high-low)/tof_bin_width_ns));
    if (bins>1000000) throw std::runtime_error("TOF range too large; reduce beam proximity to threshold.");
    TFile output("neutron_tof_90.root","RECREATE");
    if (output.IsZombie()) throw std::runtime_error("Cannot create ROOT output.");
    auto* total = new TH1D("tof_total",Form("^{40}Ar(p,n)^{40}K, E_{p}=%.3f MeV, LAB 90#circ, L=%.2f m;TOF [ns];Counts / bin",beam_mev,path_m),bins,low,high);
    total->SetLineColor(kGray+2);
    total->SetLineWidth(2);
    std::vector<TH1D*> histograms;
    TRandom3 rng(tof_random_seed);
    auto* legend = new TLegend(0.32,0.58,0.73,0.88);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetHeader(Form("Equal cross sections, #sigma_{t} = %.0f ps",sigma_ns*1000));
    for (unsigned j=0; j<levels.size(); ++j) {
        const auto& level=levels[j];
        auto* h=new TH1D(Form("tof_level_%u",j),"",bins,low,high);
        h->SetLineColor(level.color);
        h->SetLineWidth(2);
        for (int i=0; i<tof_events_per_level; ++i) {
            const double local=energy_loss ? profile.energy(rng.Uniform(0,length_cm)) : beam_mev;
            if (local<=level.onset) continue;
            h->Fill(rng.Gaus(tof_at(local,level.onset),sigma_ns));
        }
        total->Add(h);
        histograms.push_back(h);
        legend->AddEntry(h,Form("E_{x}=%.6f MeV",level.ex),"l");
        std::cout << std::fixed << std::setprecision(6) << "Ex=" << level.ex
                  << " MeV, Tn=" << level.energy << " MeV, TOF=" << level.tof
                  << " ns, delay=" << level.tof-levels.front().tof << " ns\n";
        std::cout << "  Simulated mean=" << h->GetMean() << " ns, RMS=" << h->GetStdDev()
                  << " ns, accepted=" << h->GetEntries() << ", overflow="
                  << h->GetBinContent(bins+1) << "\n";
        if (!energy_loss && (std::abs(h->GetMean()-level.tof)>6*sigma_ns/std::sqrt(tof_events_per_level) ||
            std::abs(h->GetStdDev()-sigma_ns)>0.02*sigma_ns))
            throw std::runtime_error("Simulated timing moments failed validation.");
    }
    gStyle->SetOptStat(0);
    auto* canvas=new TCanvas("tof_90","Neutron TOF at LAB 90 deg",plot_canvas_size,plot_canvas_size);
    canvas->SetLeftMargin(0.16);
    canvas->SetBottomMargin(0.15);
    canvas->SetGrid();
    total->SetMinimum(0);
    double peak = 0;
    for (auto* h:histograms) peak = std::max(peak,h->GetMaximum());
    total->SetMaximum(peak*1.25);
    total->Draw("AXIS");
    for (auto* h:histograms) h->Draw("HIST SAME");
    legend->Draw();
    canvas->SaveAs("neutron_tof_90.png");
    canvas->SaveAs("neutron_tof_90.pdf");
    auto* zoom_canvas = new TCanvas("tof_90_zoom", "Earliest TOF levels",plot_canvas_size,plot_canvas_size);
    gPad->SetLeftMargin(0.16);
    gPad->SetBottomMargin(0.15);
    gPad->SetGrid();
    auto* zoom = static_cast<TH1D*>(total->DrawCopy("AXIS"));
    zoom->SetTitle("Zoom: earliest selected levels;TOF [ns];Counts / bin");
    const double zoom_end = levels.size()>1 ? levels[1].tof : levels[0].tof;
    zoom->GetXaxis()->SetRangeUser(levels.front().tof-6*sigma_ns, (energy_loss ? levels[std::min(std::size_t(1),levels.size()-1)].end_tof : zoom_end)+6*sigma_ns);
    for (auto* h:histograms) h->DrawCopy("HIST SAME");
    zoom_canvas->SaveAs("neutron_tof_90_zoom.png");
    zoom_canvas->SaveAs("neutron_tof_90_zoom.pdf");
    output.Write();
    canvas->Write();
    zoom_canvas->Write();
    // Keep histograms alive for interactive ROOT use after closing the file.
    total->SetDirectory(nullptr);
    for (auto* h:histograms) h->SetDirectory(nullptr);
    output.Close();
}
