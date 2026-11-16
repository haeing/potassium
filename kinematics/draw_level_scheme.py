#!/usr/bin/env python3
"""Recreate the supplied nuclear level diagram using editable Matplotlib shapes.

Positions are schematic, NOT a linear energy scale. Selected levels only.
Evaluated energies/spins/gamma multipolarities: NNDC ENSDF Adopted Levels,
40K (Jun Chen, NDS 140, 1, 2017), 41K (Nesaraja & McCutchan,
NDS 133, 1, 2016); evaluations September 2015, accessed 2026-11-17.
https://www.nndc.bnl.gov/ensnds/40/K/adopted.pdf
https://www.nndc.bnl.gov/ensnds/41/K/adopted.pdf
Atomic mass excesses: AME2020 (keV), used to derive ground-state Q values.
https://www-nds.iaea.org/amdc/ame2020/mass_1.mas20.txt
See level_scheme_sources.md for uncertainties and limitations.
"""
import argparse
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch

# (energy label in MeV, schematic y, evaluated J^pi)
# Parentheses and missing parity on a candidate follow ENSDF literally.
# 8.041 MeV has conflicting reaction-based assignments; see annotation.
K41_LEVELS = [
    ('6.76977', 2.20, r'$(3/2^{-},5/2,7/2^{-})$'),
    ('6.980', 2.48, r'$1/2^{-},3/2^{-}$'),
    ('7.232', 2.84, r'$1/2^{-},3/2^{-}$'),
    ('7.471', 3.21, r'$5/2^{-},7/2^{-}$'),
    ('7.836', 3.72, r'$(1/2^{-},3/2,5/2^{+})$'),
    ('8.041', 4.04, r'$\mathrm{see\ note}$'),
    ('8.268', 4.69, r'$1/2^{-},3/2^{-}$'),
    ('8.464', 5.00, r'$1/2^{-},3/2^{-}$'),
    ('8.660', 5.29, r'$1/2^{-},3/2^{-}$'),
    ('8.873', 5.67, r'$1/2^{-},3/2^{-}$'),
    ('9.116', 6.04, r'$(5/2^{-},7/2^{-})$'),
    ('9.376', 6.42, r'$(3/2^{+},5/2^{+})$'),
    ('9.626', 6.73, r'$1/2^{-},3/2^{-}$'),
    ('9.857', 7.11, r'$1/2^{-},3/2^{-}$'),
    ('10.093', 7.57, r'$1/2^{-},3/2^{-}$'),
    ('12.220', 8.42, r'$3/2^{+},5/2^{+}$'),
]
K40_LEVELS = [
    ('g.s.', r'$4^{-}$', 3.40),
    ('0.0298299', r'$3^{-}$', 3.82),
    ('0.8001431', r'$2^{-}$', 7.34),
    ('0.891394', r'$5^{-}$', 8.08),
]
# AME2020 central mass excesses [keV], not nuclear binding energies.
MASS_EXCESS = {'n': 8071.31806, 'H1': 7288.971064,
               'He4': 2424.91587, 'Cl37': -31761.552,
               'Ar40': -35039.89997, 'K40': -33535.497,
               'K41': -35559.54880}
ATOMIC_MASS_U = {'H1': 1.007825031898, 'He4': 4.00260325413,
                 'Ar40': 39.96238312204, 'Cl37': 36.965902573}

def reaction_values(projectile, target):
    """Atomic-mass Q; nonrelativistic LAB threshold using atomic mass ratio.

    Threshold ignores electron binding corrections/nuclear mass conversion and
    relativistic Q^2/(2*M_target*c^2). Display only to 0.001 MeV.
    """
    q = (MASS_EXCESS[projectile] + MASS_EXCESS[target]
         - MASS_EXCESS['n'] - MASS_EXCESS['K40']) / 1000
    threshold = -q * (1 + ATOMIC_MASS_U[projectile]/ATOMIC_MASS_U[target])
    return q, threshold

Q_P, THRESHOLD_P = reaction_values('H1', 'Ar40')
Q_ALPHA, THRESHOLD_ALPHA = reaction_values('He4', 'Cl37')
BLUE = '#80b3d9'


def draw(output):
    # Use Times for ordinary text and mathematical labels alike.
    from matplotlib import font_manager
    font_dir = Path('/System/Library/Fonts/Supplemental')
    for name in ['Times New Roman.ttf', 'Times New Roman Italic.ttf',
                 'Times New Roman Bold.ttf', 'Times New Roman Bold Italic.ttf']:
        if (font_dir / name).exists():
            font_manager.fontManager.addfont(str(font_dir / name))
    plt.rcParams.update({
        'font.family': 'Times New Roman',
        'mathtext.fontset': 'custom',
        'mathtext.rm': 'Times New Roman',
        'mathtext.it': 'Times New Roman:italic',
        'mathtext.bf': 'Times New Roman:bold',
        'mathtext.cal': 'Times New Roman:italic',
        'mathtext.sf': 'Times New Roman',
        'mathtext.tt': 'Times New Roman',
    })
    fig, ax = plt.subplots(figsize=(17.5, 10))
    fig.subplots_adjust(left=0.015, right=0.99, bottom=0.025, top=0.985)
    ax.set(xlim=(0, 17.5), ylim=(0, 9.8))
    ax.axis('off')

    def line(xs, ys, **kw):
        ax.plot(xs, ys, color=kw.pop('color', 'black'),
                lw=kw.pop('lw', 3.5), solid_capstyle='butt', **kw)

    def text(x, y, label, size=23, **kw):
        return ax.text(x, y, label, fontsize=size, va=kw.pop('va', 'center'), **kw)

    def arrow(start, end, color='black', width=2.5, scale=24, linestyle='-'):
        ax.add_patch(FancyArrowPatch(start, end, arrowstyle='-|>',
                     mutation_scale=scale, linewidth=width, color=color, linestyle=linestyle,
                     shrinkA=0, shrinkB=0, joinstyle='miter'))

    # Entrance channels and source-image annotations.
    text(0.5, 3.15, rf'$Q={Q_P:.3f}\ \mathrm{{MeV}}$', 29)
    text(0.5, 1.46, rf'$Q={Q_ALPHA:.3f}\ \mathrm{{MeV}}$', 29)
    line([3.8, 6.4], [3.63, 3.63])
    line([3.8, 6.4], [1.98, 1.98])
    text(4.15, 3.15, r'$^{40}\mathrm{Ar}+\mathrm{p}$', 33)
    text(4.15, 1.46, r'$^{37}\mathrm{Cl}+\alpha$', 33)
    arrow((6.4, 3.63), (7.25, 8.23), width=3, scale=29)
    arrow((6.4, 1.98), (7.25, 8.23), width=3, scale=29)

    # Compound-nucleus level scheme. Separate positions preserve readability.
    xl, xr = 7.25, 9.98
    line([xl, xl], [0.76, 9.72])
    line([xr, xr], [0.76, 9.72])
    line([xl, xr], [0.76, 0.76])
    for label, y, spin in K41_LEVELS:
        line([xl, xr], [y, y])
        text(xr-0.06, y+0.025, label, 14, ha='right', va='bottom')
    text(xl+0.08, 0.98, r'$3/2^{+}$', 25)
    text(xr-0.08, 0.98, 'g.s.', 22, ha='right')
    text((xl+xr)/2, 1.57, r'$\vdots$', 30, ha='center')
    text(xr-0.15, 9.54, '[MeV]', 29, ha='right')
    text((xl+xr)/2, 0.24, r'$^{41}\mathrm{K}$', 36, ha='center')


    # Neutron exit channel and dotted enlargement guides.
    arrow((xr, 8.19), (10.83, 8.04), scale=25)
    for y, col, width in [(7.86, 'black', 3), (7.91, '#bdbdbd', 1.2),
                          (8.04, '#c6c6c6', 1.2), (8.08, '#dddddd', 1.2)]:
        line([10.83, 13.46], [y, y], color=col, lw=width)
    text(12.14, 7.20, r'$^{40}\mathrm{K}+\mathrm{n}$', 35, ha='center')
    line([13.46, 14.05], [8.08, 8.70], lw=2.5, linestyle=':')
    line([13.46, 14.05], [7.86, 3.40], lw=2.5, linestyle=':')

    # Residual nucleus, with exaggerated first-excited-state spacing.
    left, right = 14.05, 16.82
    line([left, left], [3.40, 8.70])
    line([right, right], [3.40, 8.70])
    for energy, spin, y in K40_LEVELS:
        line([left, right], [y, y])
        text(left+0.08, y+0.18, spin, 25)
        text(right-0.08, y+0.10, energy, 15, ha='right', va='bottom',
             bbox=dict(facecolor='white', edgecolor='none', pad=0.5))
    text((left+right)/2, 2.85, r'$^{40}\mathrm{K}$', 36, ha='center')

    # Gamma transitions: (x, initial y, final y, multipolarity, label y).
    transitions = [
        (14.96, 3.80, 3.43, 'M1', 4.80),
        (15.09, 7.27, 3.84, 'M1(+E2)', 6.50),
        (15.34, 7.31, 3.43, '[E2]', 5.97),
        (15.61, 8.08, 3.84, '[E2]', 5.46),
        (15.85, 8.08, 3.43, 'M1+E2', 6.98),
    ]
    for x, yi, yf, label, label_y in transitions:
        arrow((x, yi), (x, yf), color=BLUE, width=3.0, scale=27,
              linestyle='--' if x == 15.61 else '-')
        if label:
            text(x+0.03, label_y, label, 12)
    text(16.18, 4.90, r'$\gamma$', 38, color=BLUE)

    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(output, dpi=180, facecolor='white')
    svg = output.with_suffix('.svg')
    #fig.savefig(svg, facecolor='white')
    plt.close(fig)
    print(f'Saved: {output}\nSaved: {svg}')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', default='level_scheme.png')
    args = parser.parse_args()
    draw(args.output)
