import argparse
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.patches import Circle, FancyBboxPatch, Polygon, Rectangle

INK = "#000000"
MUTED = INK
SUPPLY = INK
VACUUM = INK
PIPE_LW = 1.8
SYMBOL_LW = 1.4
FONT_SCALE = 1.75


def pipe(ax, points, color=INK, lw=PIPE_LW):
    """Draw a pipe; endpoints meet the edges of equipment symbols."""
    xs, ys = zip(*points)
    ax.plot(xs, ys, color=color, lw=lw, solid_capstyle="butt",
            solid_joinstyle="miter", zorder=2)


def label(ax, x, y, text, size=10, color=INK, **kwargs):
    return ax.text(x, y, text, fontsize=size * FONT_SCALE, color=color,
                   ha=kwargs.pop("ha", "center"),
                   va=kwargs.pop("va", "center"), **kwargs)


def valve(ax, x, y, orientation="horizontal", label=None, size=0.18,
          color=INK):
    if orientation not in {"horizontal", "vertical"}:
        raise ValueError("Valve orientation must be horizontal or vertical")
    triangles = [ [(-size, -size), (0, 0), (-size, size)],
                  [(size, -size), (0, 0), (size, size)] ]
    for vertices in triangles:
        if orientation == "vertical":
            vertices = [(-dy, dx) for dx, dy in vertices]
        ax.add_patch(Polygon([(x + dx, y + dy) for dx, dy in vertices],
                             facecolor="white", edgecolor=color,
                             lw=SYMBOL_LW, zorder=4))
    if label:
        horizontal = orientation == "horizontal"
        ax.text(x if horizontal else x + 0.34,
                y + 0.42 if horizontal else y, label,
                ha="center" if horizontal else "left", va="center",
                fontsize=10 * FONT_SCALE, fontweight="semibold", color=INK)


def instrument(ax, x, y, tag, width=1.5, height=1.0, color=INK):
    ax.add_patch(FancyBboxPatch((x - width / 2, y - height / 2), width, height,
                               boxstyle="round,pad=0,rounding_size=0.07",
                               facecolor="white", edgecolor=color,
                               lw=SYMBOL_LW, zorder=4))
    label(ax, x, y, tag, size=9, zorder=5)


def equipment(ax, x, y, w, h, title, subtitle=None, color=INK):
    ax.add_patch(FancyBboxPatch((x - w / 2, y - h / 2), w, h,
                               boxstyle="round,pad=0,rounding_size=0.07",
                               facecolor="white", edgecolor=color,
                               lw=SYMBOL_LW, zorder=3))
    label(ax, x, y + (0.20 if subtitle else 0), title, size=11,
          fontweight="semibold", zorder=5)
    if subtitle:
        label(ax, x, y - 0.20, subtitle, size=8, color=MUTED, zorder=5)


def junction(ax, x, y, color):
    ax.add_patch(Circle((x, y), 0.045, facecolor=color,
                        edgecolor=color, zorder=5))


def flow_arrow(ax, start, end, color):
    ax.annotate("", xy=end, xytext=start,
                arrowprops={"arrowstyle": "-|>", "color": color,
                            "lw": PIPE_LW, "mutation_scale": 22}, zorder=3)


def build_schematic():
    """Preserve Ar → MFC → V1 → PS1 tap → V2 → cell topology.

    Chamber vacuum exits via V3; PS2 taps the pump side of V3.
    The gas cell and chamber vacuum are separate connections.
    """
    fig, ax = plt.subplots(figsize=(14, 6.05), facecolor="white")
    fig.subplots_adjust(left=0.035, right=0.965, bottom=0.04, top=0.96)
    ax.set(xlim=(0, 14), ylim=(0.95, 7.0))
    ax.set_aspect("equal")
    ax.axis("off")

    y = 4.9
    # Background chamber enclosure, drawn before its internal equipment.
    ax.add_patch(Rectangle((9.0, 3.85), 4.3, 2.15, facecolor="#FCEBEC",
                           edgecolor=INK, lw=1.3, zorder=0))
    label(ax, 11.15, 5.64, "Vacuum chamber", size=12, fontweight="semibold")
    equipment(ax, 1.0, y, 1.25, 1.10, "Ar", "Gas cylinder", SUPPLY)
    equipment(ax, 3.25, y, 1.5, 1.10, "MFC", "Mass flow\ncontroller", SUPPLY)
    equipment(ax, 11.15, y, 1.7, 0.7, "Gas cell", color=SUPPLY)

    pipe(ax, [(1.625, y), (2.5, y)], SUPPLY)
    pipe(ax, [(4.0, y), (4.92, y)], SUPPLY)
    valve(ax, 5.1, y, label="V1", color=SUPPLY)
    pipe(ax, [(5.28, y), (7.82, y)], SUPPLY)
    valve(ax, 8.0, y, label="V2", color=SUPPLY)
    pipe(ax, [(8.18, y), (10.3, y)], SUPPLY)
    pipe(ax, [(6.55, y), (6.55, 5.55)], SUPPLY)
    instrument(ax, 6.55, 6.05, "Pressure\nsensor 1", color=SUPPLY)
    junction(ax, 6.55, y, SUPPLY)
    flow_arrow(ax, (8.55, y), (8.88, y), SUPPLY)
    label(ax, 6.45, 4.43, "Argon supply", size=9, color=SUPPLY)

    vx = 11.15
    pipe(ax, [(vx, 3.85), (vx, 3.13)], VACUUM)
    valve(ax, vx, 2.95, orientation="vertical", label="V3", color=VACUUM)
    pipe(ax, [(vx, 2.77), (vx, 1.85), (4.35, 1.85)], VACUUM)
    # Pressure sensing remains downstream of the isolation valve.
    pipe(ax, [(9.1, 1.85), (9.1, 2.40)], VACUUM)
    instrument(ax, 9.1, 2.9, "Pressure\nsensor 2", color=VACUUM)
    junction(ax, 9.1, 1.85, VACUUM)
    equipment(ax, 3.25, 1.85, 2.2, 0.9, "Vacuum pump", color=VACUUM)
    flow_arrow(ax, (6.75, 1.85), (6.25, 1.85), VACUUM)
    label(ax, 7.05, 1.4, "Chamber evacuation", size=9, color=VACUUM)

    return fig, ax


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--no-show", action="store_true", help="Export without opening a window")
    parser.add_argument("--output-dir", type=Path, default=Path(__file__).resolve().parent,
                        help="Export directory (default: script directory)")
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    with plt.rc_context({"font.family": "serif",
                         "font.serif": ["Times", "Times New Roman", "STIXGeneral"],
                         "pdf.fonttype": 42}):
        fig, _ = build_schematic()
        for extension in ("pdf", "png"):
            output = args.output_dir / f"{Path(__file__).stem}.{extension}"
            fig.savefig(output, dpi=220, facecolor="white")
            print(f"Saved {output}")
        if not args.no_show:
            plt.show()
        plt.close(fig)


if __name__ == "__main__":
    main()
