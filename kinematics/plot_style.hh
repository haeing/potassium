#ifndef POTASSIUM_KINEMATICS_PLOT_STYLE_HH
#define POTASSIUM_KINEMATICS_PLOT_STYLE_HH
#include <TStyle.h>
namespace kinematics {
constexpr int plot_canvas_size = 1000;
inline void apply_plot_style() {
    gStyle->SetOptStat(0);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.09);
    gStyle->SetLabelFont(43,"XYZ");
    gStyle->SetLabelSize(30,"XYZ");
    gStyle->SetTitleFont(43,"XYZ");
    gStyle->SetTitleSize(36,"XYZ");
    gStyle->SetTitleOffset(1.3,"X");
    gStyle->SetTitleOffset(1.5,"Y");
    gStyle->SetTitleFont(42,"");
    gStyle->SetTitleFontSize(0.032);
    gStyle->SetLegendFont(42);
    gStyle->SetLegendTextSize(0.03);
}
}
#endif
