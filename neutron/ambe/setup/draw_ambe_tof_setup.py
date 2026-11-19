#!/usr/bin/env python3
"""Flat 2D AmBe setup schematic. Requires matplotlib.
Run: python3 draw_ambe_tof_setup.py --output ambe_tof_setup.png
Positions and unspecified PMT sizes are schematic, not to scale.
Both neutron detector assemblies use BC408 and EMI 9954B PMTs.
Source-adjacent ND: 200 mm axis points out of the page; visible face 100 x 100 mm.
Near ND has one PMT. Two vertically stacked far NDs use the same end-on
view; each far ND has coaxial PMTs that
overlap in projection. Depth and PMT sizes are schematic.
LaBr3 nearly touches this ND, with the AmBe source in the small intervening gap.
"""
import argparse
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, Circle

DISTANCE_LABEL = '2 m'
DETECTOR_LABEL = 'Neutron detector'
SCINTILLATOR_TYPE = 'BC408'
PMT_TYPE = 'EMI 9954B'
LEFT_X, RIGHT_X = 3.2, 12.1
DETECTOR_WIDTH, DETECTOR_HEIGHT = 1.0, 1.7
DETECTOR_Y = 1.4
PMT_WIDTH, PMT_LENGTH = .43, .75  # illustrative sizes only
SOURCE_GAP = .22  # schematic gap between LaBr3 right face and near-ND left face
LABR_LENGTH, LABR_HEIGHT = .95, .80


def rect(ax,x,y,w,h,fill):
    ax.add_patch(Rectangle((x,y),w,h,facecolor=fill,edgecolor='black',lw=1))


def detector(ax,x,cy=None,show_specs=True):
    # Same end-on view as the source-adjacent ND: 100 x 100 projected face.
    # Far ND PMTs are coaxial in this view, so their projections overlap.
    if cy is None:
        cy=DETECTOR_Y+DETECTOR_HEIGHT/2
    size=DETECTOR_WIDTH
    cx=x+size/2
    rect(ax,x,cy-size/2,size,size,'0.84')
    ax.add_patch(Circle((cx,cy),PMT_WIDTH/2,facecolor='0.96',
                       edgecolor='black',lw=1,zorder=6))
    if show_specs:
        ax.text(cx,cy+.34,SCINTILLATOR_TYPE,ha='center',va='center',fontsize=12,zorder=7)
        ax.annotate(f'PMT {PMT_TYPE}',xy=(cx,cy-PMT_WIDTH/2),
                    xytext=(cx+1.20,cy-.40),fontsize=12,ha='center',
                    arrowprops=dict(arrowstyle='-',lw=.8),zorder=8)


def top_view(ax):
    # x = flight direction, y = detector's 200 mm axis (in/out of front view).
    # Vertically stacked far detectors have identical x-y projections.
    cy=DETECTOR_Y+DETECTOR_HEIGHT/2
    width=DETECTOR_WIDTH
    depth=2*width  # 200 mm / 100 mm
    pmt_cy=cy-depth/2
    for x,ends in [(LEFT_X,1),(RIGHT_X,2)]:
        rect(ax,x,cy-depth/2,width,depth,'0.84')
        rect(ax,x+(width-PMT_WIDTH)/2,pmt_cy-PMT_LENGTH,PMT_WIDTH,PMT_LENGTH,'0.96')
        if ends==2:
            rect(ax,x+(width-PMT_WIDTH)/2,cy+depth/2,PMT_WIDTH,PMT_LENGTH,'0.96')
    ax.text(LEFT_X+width/2,cy+depth/2+.33,'Neutron detector',
            fontsize=16,ha='center',va='bottom')
    ax.text(RIGHT_X+width/2,cy+depth/2+PMT_LENGTH+.20,'Neutron detectors',
            fontsize=16,ha='center',va='bottom')
    ax.annotate('2 detectors (stacked)',xy=(RIGHT_X+width,cy+.5),
                xytext=(RIGHT_X+1.2,cy+.85),fontsize=11,ha='left',va='center',
                arrowprops=dict(arrowstyle='-',lw=.7))
    source_x=LEFT_X-SOURCE_GAP/2
    rect(ax,source_x-.08,cy-.18,.16,.36,'0.35')
    labr_x=LEFT_X-SOURCE_GAP-LABR_LENGTH
    rect(ax,labr_x,cy-LABR_HEIGHT/2,LABR_LENGTH,LABR_HEIGHT,'0.94')
    ax.annotate(r'LaBr$_3$',xy=(labr_x,cy),xytext=(.25,cy),ha='left',va='center',
                fontsize=17,arrowprops=dict(arrowstyle='-',lw=.8))
    ax.annotate(r'$^{241}$AmBe',xy=(source_x,cy+.18),xytext=(1.8,cy+1.5),
                ha='center',va='center',fontsize=17,arrowprops=dict(arrowstyle='-',lw=.8))
    ax.annotate('',xy=(RIGHT_X,cy),xytext=(source_x+.10,cy),
                arrowprops=dict(arrowstyle='-|>',linestyle=(0,(4,3)),lw=1.1,color='black'))
    ax.text(8,cy+.28,r'$n,\gamma$',ha='center',fontsize=17)
    ax.set(xlim=(0,15.3),ylim=(-.3,4.65));ax.set_aspect('equal');ax.axis('off')
    ax.text(.12,4.35,'(b) Top view',fontsize=15,va='top')


def draw(output):
    plt.rcParams.update({'font.family':'serif','font.serif':['Times New Roman','DejaVu Serif'],
                         'mathtext.fontset':'stix','svg.fonttype':'none'})
    fig,(ax,ax_top)=plt.subplots(2,1,figsize=(13,8.3),facecolor='white')
    fig.subplots_adjust(left=.015,right=.99,bottom=.02,top=.98,hspace=.10)
    beam_y=DETECTOR_Y+DETECTOR_HEIGHT/2
    # Two far NDs in the same end-on view, stacked vertically.
    far_gap=0.0  # schematic separation between the two square faces
    far_offset=(DETECTOR_WIDTH+far_gap)/2
    detector(ax,RIGHT_X,beam_y+far_offset)
    detector(ax,RIGHT_X,beam_y-far_offset,show_specs=False)
    ax.text(RIGHT_X+DETECTOR_WIDTH/2,beam_y+far_offset+DETECTOR_WIDTH/2+.30,
            'Neutron detectors',ha='center',va='bottom',fontsize=16)
    # The near ND is viewed along its 200 mm axis: square 100 x 100 mm face.
    near_width=DETECTOR_WIDTH
    near_height=DETECTOR_WIDTH
    rect(ax,LEFT_X,beam_y-near_height/2,near_width,near_height,'0.84')
    pmt_cx=LEFT_X+near_width/2
    # A single PMT along the viewing axis appears as a circular end face.
    ax.add_patch(Circle((pmt_cx,beam_y),PMT_WIDTH/2,
                       facecolor='0.96',edgecolor='black',lw=1,zorder=6))
    ax.text(pmt_cx,beam_y+.34,SCINTILLATOR_TYPE,
            ha='center',va='center',fontsize=12,zorder=7)
    ax.text(pmt_cx,beam_y+near_height/2+.34,DETECTOR_LABEL,
            ha='center',va='bottom',fontsize=16)
    ax.annotate(f'PMT {PMT_TYPE}',xy=(pmt_cx,beam_y-PMT_WIDTH/2),
                xytext=(pmt_cx+.9,beam_y-.92),fontsize=12,ha='center',
                arrowprops=dict(arrowstyle='-',lw=.8),zorder=8)
    source_x=LEFT_X-SOURCE_GAP/2
    rect(ax,source_x-.08,beam_y-.18,.16,.36,'0.35')
    ax.annotate(r'$^{241}$AmBe',xy=(source_x,beam_y+.18),xytext=(2.10,3.66),
                ha='center',va='center',fontsize=17,arrowprops=dict(arrowstyle='-',lw=.8))
    # LaBr3 remains a separate source-adjacent detector, drawn as a flat rectangle.
    labr_x=LEFT_X-SOURCE_GAP-LABR_LENGTH
    rect(ax,labr_x,beam_y-LABR_HEIGHT/2,LABR_LENGTH,LABR_HEIGHT,'0.94')
    ax.annotate(r'LaBr$_3$',xy=(labr_x,beam_y),xytext=(.25,beam_y),
                ha='left',va='center',fontsize=17,arrowprops=dict(arrowstyle='-',lw=.8))
    ax.annotate('',xy=(labr_x+LABR_LENGTH*.45,beam_y),xytext=(source_x-.1,beam_y),
                arrowprops=dict(arrowstyle='-|>',linestyle='--',lw=1))
    # Long flight path, from source to the near face of the second detector.
    ax.annotate('',xy=(RIGHT_X,beam_y),xytext=(source_x+.1,beam_y),
                arrowprops=dict(arrowstyle='-|>',linestyle=(0,(4,3)),lw=1.1),zorder=5)
    ax.text(8,beam_y+.28,r'$n,\gamma$',ha='center',fontsize=17)
    yy=.20
    ax.annotate('',xy=(RIGHT_X,yy),xytext=(source_x,yy),
                arrowprops=dict(arrowstyle='<->',lw=.8,shrinkA=0,shrinkB=0))
    for x in [source_x,RIGHT_X]:ax.plot([x,x],[yy-.1,yy+.16],color='black',lw=.7)
    ax.text((source_x+RIGHT_X)/2,yy+.10,DISTANCE_LABEL,ha='center',va='bottom',fontsize=18)
    ax.set(xlim=(0,15.3),ylim=(-.05,4.65));ax.set_aspect('equal');ax.axis('off')
    ax.text(.12,4.35,'(a) Front view',fontsize=15,va='top')
    top_view(ax_top)
    output=Path(output);output.parent.mkdir(parents=True,exist_ok=True)
    for ext in ['.png','.svg']:
        fig.savefig(output.with_suffix(ext),dpi=220,bbox_inches='tight',facecolor='white')
    plt.close(fig)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',default='ambe_tof_setup.png')
    draw(p.parse_args().output)
