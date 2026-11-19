#!/usr/bin/env python3
"""Black-and-white 2D setup drawing; all dimensions in mm.

Front: x-z, top: x-y. T1 and T2 are centered above/between ND1/ND2.
ND 200(x) x 100(y) x 100(z); T1 100(x) x 5(y) x 10(z).
T2 is T1 rotated -90 deg about z: 5(x) x 100(y) x 10(z).
PMT and light-guide dimensions are provisional and not annotated.
150 is interpreted as upper T1 top to lower T2 bottom.
Run: python3 draw_detector_setup_2d.py --output detector_setup_2d.png
Requires: matplotlib.
"""
import argparse
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, Polygon, Circle

ND_SCINTILLATOR_TYPE = 'BC408'
ND_PMT_TYPE = 'EMI 9954B'
ND_LENGTH, ND_DEPTH, ND_HEIGHT = 200, 100, 100
# Finger trigger dimensions [mm] -- EDIT THESE THREE VALUES LATER.
# T1: x=length, y=depth, z=height. T2: 90-degree rotation about z.
# These dimensions update BOTH views, PMT attachment points and dimension labels.
T_LENGTH = 100  # provisional length
T_DEPTH = 5     # provisional horizontal thickness
T_HEIGHT = 10   # provisional vertical thickness
ND_PMT_LENGTH, ND_PMT_DIAMETER = 120, 50  # provisional
T_PMT_LENGTH, T_PMT_DIAMETER = 80, 25     # provisional
GUIDE_LENGTH = 15                       # provisional
ND2_BOTTOM, ND1_BOTTOM = 0, 135
# Fixed bottom elevations [mm]. Changing finger height changes its top,
# total height, and clearance above it; bottom elevations remain fixed.
T2_BOTTOM, T1_BOTTOM = 115, 255


def rect(ax,x,y,w,h,fill='0.82',lw=.9):
    ax.add_patch(Rectangle((x,y),w,h,facecolor=fill,edgecolor='black',lw=lw,zorder=2))


def dim(ax,p,q,label,offset=8,rotation=0):
    ax.annotate('',xy=q,xytext=p,arrowprops=dict(arrowstyle='<->',lw=.75,
                shrinkA=0,shrinkB=0,mutation_scale=8),zorder=5)
    x=(p[0]+q[0])/2; y=(p[1]+q[1])/2
    if p[0]==q[0]:x+=offset
    else:y+=offset
    ax.text(x,y,label,ha='center',va='center',rotation=rotation,fontsize=11,
            bbox=dict(facecolor='white',edgecolor='none',pad=.5),zorder=6)


def extension(ax,xs,ys):ax.plot(xs,ys,color='black',lw=.5,zorder=1)


def horizontal_pmt(ax,edge,z,sign,length,diameter,initial_width):
    a=edge; b=edge+sign*GUIDE_LENGTH; c=b+sign*length
    ax.add_patch(Polygon([(a,z-initial_width/2),(b,z-diameter/2),
                          (b,z+diameter/2),(a,z+initial_width/2)],
                         facecolor='white',edgecolor='black',lw=.8,zorder=2))
    rect(ax,min(b,c),z-diameter/2,length,diameter,'0.96')


def front(ax):
    l,d,h = T_LENGTH,T_DEPTH,T_HEIGHT
    t1c=T1_BOTTOM+h/2; t2c=T2_BOTTOM+h/2
    t1top=T1_BOTTOM+h
    for name,z in [('ND1',ND1_BOTTOM),('ND2',ND2_BOTTOM)]:
        rect(ax,-100,z,ND_LENGTH,ND_HEIGHT)
        ax.text(0,z+59,name,ha='center',va='center',fontsize=18,zorder=4)
        ax.text(0,z+39,ND_SCINTILLATOR_TYPE,ha='center',va='center',fontsize=13,zorder=4)
        for sign in [-1,1]:horizontal_pmt(ax,sign*100,z+50,sign,
                             ND_PMT_LENGTH,ND_PMT_DIAMETER,60)
    rect(ax,-l/2,T1_BOTTOM,l,h,'0.65')
    horizontal_pmt(ax,l/2,t1c,1,T_PMT_LENGTH,T_PMT_DIAMETER,h)
    ax.annotate('T1',xy=(-l*.15,t1c),xytext=(-80,t1top+23),ha='center',fontsize=14,
                arrowprops=dict(arrowstyle='-',lw=.8))
    # T2 rotates in the x-y plane, so its front-view width is T_DEPTH.
    rect(ax,-d/2,T2_BOTTOM,d,h,'0.65')
    ax.add_patch(Circle((0,t2c),T_PMT_DIAMETER/2,facecolor='0.96',
                       edgecolor='black',lw=.9,zorder=3))
    ax.annotate('T2 (end view)',xy=(-T_PMT_DIAMETER/2,t2c),
                xytext=(-95,t2c-1),fontsize=12,
                ha='center',va='center',arrowprops=dict(arrowstyle='-',lw=.8))
    # Reference dimensions derive from the current geometry.
    for x,z0,z1 in [(-267,ND2_BOTTOM,ND1_BOTTOM+ND_HEIGHT),
                    (267,ND2_BOTTOM,t1top),(297,T2_BOTTOM,t1top)]:
        dim(ax,(x,z0),(x,z1),f'{z1-z0:g}',offset=-9 if x<0 else 9,rotation=90)
        edge=-100 if x<0 else l/2
        extension(ax,[edge,x+(-5 if x<0 else 5)],[z1,z1])
        extension(ax,[(-100 if x<0 else 100) if z0==ND2_BOTTOM else d/2,
                      x+(-5 if x<0 else 5)],[z0,z0])
    # ND width, individual height and gaps.
    dim(ax,(-100,-30),(100,-30),'200',offset=-9)
    for x in [-100,100]:extension(ax,[x,x],[0,-37])
    dim(ax,(115,0),(115,100),'100',offset=9,rotation=90)
    # These short gaps are labeled by leaders to avoid crowded dimension arrows.
    for z0,z1,tx,ty in [(ND1_BOTTOM+ND_HEIGHT,T1_BOTTOM,-180,T1_BOTTOM-2),
                           (T2_BOTTOM+h,ND1_BOTTOM,-175,ND1_BOTTOM+8),
                           (ND2_BOTTOM+ND_HEIGHT,T2_BOTTOM,-175,T2_BOTTOM-10)]:
        dim(ax,(-115,z0),(-115,z1),'',offset=0)
        ax.annotate(f'{z1-z0:g}',xy=(-115,(z0+z1)/2),xytext=(tx,ty),
                    fontsize=11,ha='center',va='center',
                    arrowprops=dict(arrowstyle='-',lw=.6),
                    bbox=dict(facecolor='white',edgecolor='none',pad=1),zorder=7)
    # PMT and optical guide sizes are schematic; no dimension annotations.
    ax.annotate(f'PMT {ND_PMT_TYPE}',xy=(175,50),xytext=(198,-26),fontsize=12,ha='center',
                arrowprops=dict(arrowstyle='-',lw=.7))
    ax.text(-310,316,'(a) Front view (x-z)',fontsize=15)
    ax.set(xlim=(-320,325),ylim=(-52,max(330,t1top+65)));ax.set_aspect('equal');ax.axis('off')


def top(ax):
    l,d,h = T_LENGTH,T_DEPTH,T_HEIGHT
    r=T_PMT_DIAMETER/2
    guide_end=-l/2-GUIDE_LENGTH
    pmt_end=guide_end-T_PMT_LENGTH
    # Dashed footprint of ND; omit the second coincident ND projection.
    ax.add_patch(Rectangle((-100,-50),200,100,facecolor='0.94',edgecolor='black',
                           lw=.9,linestyle='--',zorder=1))
    ax.text(-95,-42,'ND footprint',fontsize=10,va='bottom')
    # Orthogonal fingers, centered at the same x-y location.
    rect(ax,-l/2,-d/2,l,d,'0.65')
    horizontal_pmt(ax,l/2,0,1,T_PMT_LENGTH,T_PMT_DIAMETER,d)
    rect(ax,-d/2,-l/2,d,l,'0.65')
    ax.add_patch(Polygon([(-d/2,-l/2),(-r,guide_end),(r,guide_end),(d/2,-l/2)],
                         facecolor='white',edgecolor='black',lw=.8,zorder=2))
    rect(ax,-r,pmt_end,2*r,T_PMT_LENGTH,'0.96')
    ax.annotate('T1',xy=(-l*.35,0),xytext=(-l/2-25,22),ha='center',fontsize=14,
                arrowprops=dict(arrowstyle='-',lw=.7))
    ax.annotate('T2',xy=(0,-l*.3),xytext=(-40,-l/2-30),ha='center',fontsize=14,
                arrowprops=dict(arrowstyle='-',lw=.7))
    # Finger dimensions update automatically from the three editable values.
    dim(ax,(-l/2,38),(l/2,38),f'{l:g} (T1)',offset=8)
    for x in [-l/2,l/2]:extension(ax,[x,x],[d/2,43])
    dim(ax,(-l/2+15,-l/2),(-l/2+15,l/2),f'{l:g} (T2)',offset=-9,rotation=90)
    for y in [-l/2,l/2]:extension(ax,[-d/2,-l/2+10],[y,y])
    dim(ax,(-100,79),(100,79),'200 (ND)',offset=9)
    for x in [-100,100]:extension(ax,[x,x],[50,84])
    dim(ax,(-125,-50),(-125,50),'100 (ND)',offset=-10,rotation=90)
    for y in [-50,50]:extension(ax,[-100,-130],[y,y])
    ax.annotate('90°',xy=(20,-18),xytext=(40,-35),fontsize=12,
                arrowprops=dict(arrowstyle='-',lw=.7))
    ax.text(-165,112,'(b) Top view (x-y)',fontsize=15)
    ax.set(xlim=(min(-175,-l/2-65),max(175,l/2+GUIDE_LENGTH+T_PMT_LENGTH+15)),
           ylim=(min(-190,pmt_end-45),max(126,l/2+70)));ax.set_aspect('equal');ax.axis('off')


def draw(output):
    if min(T_LENGTH,T_DEPTH,T_HEIGHT)<=0:
        raise ValueError('Finger dimensions must be positive.')
    plt.rcParams.update({'font.family':'serif','font.serif':['Times New Roman','DejaVu Serif'],
                         'svg.fonttype':'none'})
    fig=plt.figure(figsize=(13,5.8),facecolor='white')
    ax1=fig.add_axes([.035,.055,.65,.90]);front(ax1)
    ax2=fig.add_axes([.70,.055,.28,.90]);top(ax2)
    output=Path(output);output.parent.mkdir(parents=True,exist_ok=True)
    for suffix in ['.png','.svg']:
        fig.savefig(output.with_suffix(suffix),dpi=200,bbox_inches='tight',facecolor='white')
    plt.close(fig)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output',default='detector_setup_2d.png')
    draw(p.parse_args().output)
