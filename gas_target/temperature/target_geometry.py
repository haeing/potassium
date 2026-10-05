import gmsh
import math
import sys


# ============================================================
# GEOMETRY PARAMETERS [mm]
# ============================================================

# ------------------------------------------------------------
# Top cold-head contact disk
# ------------------------------------------------------------
TOP_DIAMETER = 40.0
TOP_THICKNESS = 10.0


# ------------------------------------------------------------
# Long copper rod
# ------------------------------------------------------------
ROD_DIAMETER = 20.0
ROD_LENGTH = 430.0


# ------------------------------------------------------------
# Stage 1
#
# 40 x 40 mm
# height = 16 mm
# ------------------------------------------------------------
STAGE1_X = 40.0
STAGE1_Y = 40.0
STAGE1_HEIGHT = 16.0


# ------------------------------------------------------------
# Stage 2
#
# 40 x 17 mm
# height = 9 mm
#
# Starts 14 mm inward from one edge of Stage 1
# ------------------------------------------------------------
STAGE2_X = 40.0
STAGE2_Y = 17.0
STAGE2_HEIGHT = 9.0

STAGE2_EDGE_OFFSET = 14.0


# ------------------------------------------------------------
# Stage 3
#
# 40 x 12 mm
#
# Starts 0.5 mm inward from the OPPOSITE edge
# of Stage 2.
# ------------------------------------------------------------
STAGE3_X = 40.0
STAGE3_Y = 12.0

STAGE3_EDGE_OFFSET = 5.0


# ------------------------------------------------------------
# Distance between Stage 2 bottom
# and the highest point of the target disk
# ------------------------------------------------------------
STAGE2_TARGET_GAP = 2.0


# ------------------------------------------------------------
# Target disk
#
# Disk axis = Y
#
# Diameter = 60 mm
# Thickness = 22 mm
# Hole = 25 mm
# ------------------------------------------------------------
TARGET_DIAMETER = 60.0
TARGET_THICKNESS = 22.0
TARGET_HOLE_DIAMETER = 25.0


# ------------------------------------------------------------
# Penetration used to make Stage 3 and target
# one continuous OCC volume
# ------------------------------------------------------------
TARGET_OVERLAP = 1.0


# ------------------------------------------------------------
# Small overlaps for robust Boolean union
# ------------------------------------------------------------
BOOLEAN_OVERLAP = 0.5


# ============================================================
# MESH PARAMETERS [mm]
# ============================================================

MESH_MIN = 3.0
MESH_MAX = 8.0

SHOW_GUI = True

OUTPUT_FILE = "target.msh"


# ============================================================
# INITIALIZE GMSH
# ============================================================

gmsh.initialize()

gmsh.option.setNumber(
    "General.Terminal",
    1
)

gmsh.model.add(
    "thermal_target"
)

occ = gmsh.model.occ


try:

    # ========================================================
    # COORDINATE SYSTEM
    # ========================================================
    #
    #                  Z
    #                  ↑
    #                  |
    #                  |
    #                  +------→ X
    #
    #
    # Y = depth direction
    #
    #
    # Rod axis:
    #       Z
    #
    # Target disk axis:
    #       Y
    #
    #
    # Therefore the circular face of the target lies
    # in the X-Z plane.
    #
    # ========================================================


    # ========================================================
    # 1. TARGET DISK
    # ========================================================

    print()
    print("Creating target disk...")


    target_radius = (
        TARGET_DIAMETER / 2.0
    )

    hole_radius = (
        TARGET_HOLE_DIAMETER / 2.0
    )


    # --------------------------------------------------------
    # Target position
    #
    # Bottom = Z = 0
    #
    # Center = Z = 30 mm
    #
    # Top = Z = 60 mm
    # --------------------------------------------------------

    target_center_z = target_radius

    target_top_z = (
        target_center_z
        + target_radius
    )


    # --------------------------------------------------------
    # Outer disk
    #
    # IMPORTANT:
    #
    # Cylinder axis = Y
    #
    # This is the 90-degree rotated orientation.
    # --------------------------------------------------------

    target_outer = occ.addCylinder(

        # Start point
        0.0,
        -TARGET_THICKNESS / 2.0,
        target_center_z,

        # Axis vector
        0.0,
        TARGET_THICKNESS,
        0.0,

        # Radius
        target_radius
    )


    # --------------------------------------------------------
    # Central hole
    #
    # Cutting cylinder is made longer than the disk.
    # --------------------------------------------------------

    target_hole = occ.addCylinder(

        0.0,
        -TARGET_THICKNESS,
        target_center_z,

        0.0,
        2.0 * TARGET_THICKNESS,
        0.0,

        hole_radius
    )


    # --------------------------------------------------------
    # Cut central hole
    # --------------------------------------------------------

    target_result, _ = occ.cut(

        [(3, target_outer)],

        [(3, target_hole)],

        removeObject=True,
        removeTool=True
    )


    if len(target_result) != 1:

        raise RuntimeError(
            "Target disk Boolean cut failed."
        )


    target = target_result[0]


    # ========================================================
    # 2. STAGE 2 VERTICAL POSITION
    # ========================================================
    #
    # Target highest point:
    #
    #       Z = 60 mm
    #
    # Stage 2 bottom:
    #
    #       Z = 60 + 2
    #         = 62 mm
    #
    # ========================================================

    stage2_bottom_z = (
        target_top_z
        + STAGE2_TARGET_GAP
    )


    stage2_top_z = (
        stage2_bottom_z
        + STAGE2_HEIGHT
    )


    # ========================================================
    # 3. STAGE 1 VERTICAL POSITION
    # ========================================================

    stage1_bottom_z = (
        stage2_top_z
    )


    stage1_top_z = (
        stage1_bottom_z
        + STAGE1_HEIGHT
    )


    # ========================================================
    # 4. STAGE 1
    # ========================================================

    print()
    print(
        "Creating Stage 1 "
        "(40 x 40 x 16 mm)..."
    )


    # Stage 1 centered in Y.
    #
    # Y range:
    #
    #       -20 ... +20 mm

    stage1_ymin = (
        -STAGE1_Y / 2.0
    )

    stage1_ymax = (
        +STAGE1_Y / 2.0
    )


    stage1 = occ.addBox(

        # X
        -STAGE1_X / 2.0,

        # Y
        stage1_ymin,

        # Z
        stage1_bottom_z,

        # dX
        STAGE1_X,

        # dY
        STAGE1_Y,

        # dZ
        STAGE1_HEIGHT
    )


    # ========================================================
    # 5. STAGE 2
    # ========================================================
    #
    # Stage 1 Y:
    #
    #       -20 ---------------- +20
    #
    #
    # Stage 2 begins 14 mm inward
    # from the -Y edge.
    #
    #
    # Stage 2 Ymin:
    #
    #       -20 + 14
    #       = -6 mm
    #
    #
    # Stage 2 depth = 17 mm
    #
    #
    # Therefore:
    #
    #       -6 ----------- +11
    #
    # ========================================================

    print(
        "Creating Stage 2 "
        "(40 x 17 x 9 mm)..."
    )


    stage2_ymin = (
        stage1_ymin
        + STAGE2_EDGE_OFFSET
    )


    stage2_ymax = (
        stage2_ymin
        + STAGE2_Y
    )


    stage2 = occ.addBox(

        # X
        -STAGE2_X / 2.0,

        # Y
        stage2_ymin,

        # Z
        stage2_bottom_z,

        # dX
        STAGE2_X,

        # dY
        STAGE2_Y,

        # dZ
        STAGE2_HEIGHT
    )


    # ========================================================
    # 6. STAGE 3 HORIZONTAL POSITION
    # ========================================================
    #
    # Stage 2:
    #
    #       Y = -6 ... +11 mm
    #
    #
    # Stage 3 is offset from the OPPOSITE side.
    #
    # The +Y edge of Stage 3 is 0.5 mm inward
    # from the +Y edge of Stage 2.
    #
    #
    # Stage 3 Ymax:
    #
    #       +11 - 0.5
    #       = +10.5 mm
    #
    #
    # Stage 3 depth:
    #
    #       12 mm
    #
    #
    # Therefore Stage 3:
    #
    #       -1.5 ... +10.5 mm
    #
    # ========================================================

    stage3_ymax = (
        stage2_ymax
        - STAGE3_EDGE_OFFSET
    )


    stage3_ymin = (
        stage3_ymax
        - STAGE3_Y
    )


    # ========================================================
    # 7. CALCULATE TARGET CIRCULAR SURFACE
    # ========================================================
    #
    # Because the target disk axis is now Y,
    #
    # its circular cross-section lies in X-Z:
    #
    #
    #       X² + (Z-Zc)² = R²
    #
    #
    # Upper surface:
    #
    #                 __________________
    #       Z = Zc + √(R² - X²)
    #
    #
    # Stage 3 has:
    #
    #       X = -20 ... +20 mm
    #
    #
    # To connect the entire 40-mm-wide Stage 3
    # to the circular target, calculate the target
    # surface at |X| = 20 mm.
    #
    # ========================================================

    stage3_xmin = (
        -STAGE3_X / 2.0
    )

    stage3_xmax = (
        +STAGE3_X / 2.0
    )


    x_farthest = max(
        abs(stage3_xmin),
        abs(stage3_xmax)
    )


    if x_farthest >= target_radius:

        raise RuntimeError(
            "Stage 3 is wider than the target disk."
        )


    target_surface_z = (
        target_center_z
        + math.sqrt(
            target_radius**2
            - x_farthest**2
        )
    )


    # ========================================================
    # 8. STAGE 3 VERTICAL POSITION
    # ========================================================
    #
    # At X = ±20 mm:
    #
    # target surface is approximately:
    #
    #       52.36 mm
    #
    #
    # Stage 3 extends 1 mm into the disk:
    #
    #       ~51.36 mm
    #
    #
    # This guarantees a real volume overlap.
    #
    # After OCC fuse, Stage 3 and the target become
    # one continuous solid.
    #
    # ========================================================

    stage3_bottom_z = (
        target_surface_z
        - TARGET_OVERLAP
    )


    # Slightly overlap Stage 2 as well.

    stage3_top_z = (
        stage2_bottom_z
        + BOOLEAN_OVERLAP
    )


    stage3_height = (
        stage3_top_z
        - stage3_bottom_z
    )


    if stage3_height <= 0:

        raise RuntimeError(
            "Invalid Stage 3 height."
        )


    # ========================================================
    # 9. CREATE STAGE 3
    # ========================================================

    print(
        "Creating Stage 3 "
        "(40 x 12 mm)..."
    )


    stage3 = occ.addBox(

        # X
        stage3_xmin,

        # Y
        stage3_ymin,

        # Z
        stage3_bottom_z,

        # dX
        STAGE3_X,

        # dY
        STAGE3_Y,

        # dZ
        stage3_height
    )


    # ========================================================
    # 10. LONG COPPER ROD
    # ========================================================

    print(
        "Creating long copper rod..."
    )


    # Slight penetration into Stage 1.

    rod_bottom_z = (
        stage1_top_z
        - BOOLEAN_OVERLAP
    )


    rod_height = (
        ROD_LENGTH
        + BOOLEAN_OVERLAP
    )


    rod = occ.addCylinder(

        0.0,
        0.0,
        rod_bottom_z,

        0.0,
        0.0,
        rod_height,

        ROD_DIAMETER / 2.0
    )


    # ========================================================
    # 11. TOP COLD-HEAD CONTACT DISK
    # ========================================================

    print(
        "Creating cold-head contact disk..."
    )


    # Physical top of the 430-mm rod.

    rod_top_z = (
        stage1_top_z
        + ROD_LENGTH
    )


    top_disk_bottom_z = (
        rod_top_z
        - BOOLEAN_OVERLAP
    )


    top_disk_height = (
        TOP_THICKNESS
        + BOOLEAN_OVERLAP
    )


    top_disk = occ.addCylinder(

        0.0,
        0.0,
        top_disk_bottom_z,

        0.0,
        0.0,
        top_disk_height,

        TOP_DIAMETER / 2.0
    )


    # ========================================================
    # 12. BOOLEAN UNION
    # ========================================================

    print()
    print(
        "Fusing all components..."
    )


    parts = [

        target,

        (3, stage3),

        (3, stage2),

        (3, stage1),

        (3, rod),

        (3, top_disk)
    ]


    fused, _ = occ.fuse(

        [parts[0]],

        parts[1:],

        removeObject=True,

        removeTool=True
    )


    occ.synchronize()


    


    # ========================================================
    # 13. CHECK NUMBER OF VOLUMES
    # ========================================================

    volumes = (
        gmsh.model.getEntities(3)
    )


    print()
    print(
        "========================================"
    )
    print(
        " GEOMETRY CHECK"
    )
    print(
        "========================================"
    )


    print(
        f"Number of volumes = {len(volumes)}"
    )


    if len(volumes) == 1:

        print(
            "OK: geometry is one continuous volume."
        )

    else:

        print(
            "WARNING: geometry is NOT one continuous volume."
        )


    # ========================================================
    # 14. PHYSICAL GROUPS FOR THERMAL ANALYSIS
    # ========================================================

    if len(volumes) != 1:
        raise RuntimeError(
            f"Expected one copper volume, but found {len(volumes)} volumes."
        )

    # Fixed IDs used later by thermal_analysis.py
    COPPER_ID = 1
    COLD_HEAD_ID = 2
    RADIATION_ID = 3

    copper_volume = volumes[0][1]

    # Copper volume
    gmsh.model.addPhysicalGroup(
        3, [copper_volume], tag=COPPER_ID
    )
    gmsh.model.setPhysicalName(
        3, COPPER_ID, "COPPER"
    )

    # Get all external surfaces of the fused copper body
    surfaces = gmsh.model.getBoundary(
        [(3, copper_volume)],
        oriented=False,
        recursive=False
    )

    # Highest Z coordinate of the complete copper geometry
    _, _, _, _, _, geometry_zmax = gmsh.model.getBoundingBox(
        3, copper_volume
    )

    cold_head_surfaces = []
    radiation_surfaces = []
    TOL = 1.0e-3

    for dim, tag in surfaces:
        sxmin, symin, szmin, sxmax, symax, szmax = (
            gmsh.model.getBoundingBox(dim, tag)
        )

        # The top horizontal face of the upper disk is the
        # cold-head contact surface. All other external faces
        # are exposed to vacuum and receive thermal radiation.
        is_cold_head = (
            abs(szmin - geometry_zmax) < TOL
            and abs(szmax - geometry_zmax) < TOL
        )

        if is_cold_head:
            cold_head_surfaces.append(tag)
        else:
            radiation_surfaces.append(tag)

    if len(cold_head_surfaces) == 0:
        raise RuntimeError("Could not find COLD_HEAD surface.")

    gmsh.model.addPhysicalGroup(
        2, cold_head_surfaces, tag=COLD_HEAD_ID
    )
    gmsh.model.setPhysicalName(
        2, COLD_HEAD_ID, "COLD_HEAD"
    )

    gmsh.model.addPhysicalGroup(
        2, radiation_surfaces, tag=RADIATION_ID
    )
    gmsh.model.setPhysicalName(
        2, RADIATION_ID, "RADIATION"
    )

    print()
    print("========================================")
    print(" THERMAL PHYSICAL GROUPS")
    print("========================================")
    print(f"COPPER volume      = {copper_volume}")
    print(f"COLD_HEAD surfaces = {cold_head_surfaces}")
    print(f"RADIATION surfaces = {radiation_surfaces}")
    print(f"Physical IDs       = COPPER:{COPPER_ID}, "
          f"COLD_HEAD:{COLD_HEAD_ID}, RADIATION:{RADIATION_ID}")


    # ========================================================
    # 15. PRINT IMPORTANT DIMENSIONS
    # ========================================================

    print()
    print(
        "========================================"
    )
    print(
        " GEOMETRY INFORMATION"
    )
    print(
        "========================================"
    )


    print()
    print("Stage 1")

    print(
        f"  Y = "
        f"{stage1_ymin:.2f} "
        f"to {stage1_ymax:.2f} mm"
    )


    print()
    print("Stage 2")

    print(
        f"  Y = "
        f"{stage2_ymin:.2f} "
        f"to {stage2_ymax:.2f} mm"
    )


    print()
    print("Stage 3")

    print(
        f"  X = "
        f"{stage3_xmin:.2f} "
        f"to {stage3_xmax:.2f} mm"
    )

    print(
        f"  Y = "
        f"{stage3_ymin:.2f} "
        f"to {stage3_ymax:.2f} mm"
    )

    print(
        f"  Z = "
        f"{stage3_bottom_z:.2f} "
        f"to {stage3_top_z:.2f} mm"
    )


    print()
    print("Target")

    print(
        f"  diameter  = "
        f"{TARGET_DIAMETER:.2f} mm"
    )

    print(
        f"  thickness = "
        f"{TARGET_THICKNESS:.2f} mm"
    )

    print(
        f"  hole      = "
        f"{TARGET_HOLE_DIAMETER:.2f} mm"
    )

    print(
        "  axis      = Y"
    )


    print()
    print("Stage 3 / target connection")

    print(
        f"  target surface at "
        f"X = ±{x_farthest:.1f} mm:"
    )

    print(
        f"      Z = "
        f"{target_surface_z:.3f} mm"
    )

    print(
        f"  Stage 3 bottom:"
    )

    print(
        f"      Z = "
        f"{stage3_bottom_z:.3f} mm"
    )

    print(
        f"  overlap = "
        f"{TARGET_OVERLAP:.2f} mm"
    )


    # ========================================================
    # 16. BOUNDING BOX
    # ========================================================

    xmin, ymin, zmin, xmax, ymax, zmax = (
        gmsh.model.getBoundingBox(
            -1,
            -1
        )
    )


    print()
    print("Bounding box")

    print(
        f"  X: "
        f"{xmin:.2f} -> {xmax:.2f} mm"
    )

    print(
        f"  Y: "
        f"{ymin:.2f} -> {ymax:.2f} mm"
    )

    print(
        f"  Z: "
        f"{zmin:.2f} -> {zmax:.2f} mm"
    )


    # ========================================================
    # 17. MESH SETTINGS
    # ========================================================

    print()
    print(
        "========================================"
    )
    print(
        " MESH GENERATION"
    )
    print(
        "========================================"
    )


    gmsh.option.setNumber(
        "Mesh.MeshSizeMin",
        MESH_MIN
    )


    gmsh.option.setNumber(
        "Mesh.MeshSizeMax",
        MESH_MAX
    )


    # Do not automatically make a very fine mesh
    # on curved surfaces.

    gmsh.option.setNumber(
        "Mesh.MeshSizeFromCurvature",
        0
    )


    # 2D surface mesh:
    # Frontal-Delaunay

    gmsh.option.setNumber(
        "Mesh.Algorithm",
        6
    )


    # 3D:
    # Delaunay tetrahedra

    gmsh.option.setNumber(
        "Mesh.Algorithm3D",
        1
    )


    # Linear tetrahedra

    gmsh.option.setNumber(
        "Mesh.ElementOrder",
        1
    )


    # ========================================================
    # 18. GENERATE 3D MESH
    # ========================================================

    print()
    print(
        "Generating 3D tetrahedral mesh..."
    )


    gmsh.model.mesh.generate(3)


    print()
    print(
        "3D mesh generation completed."
    )


    # ========================================================
    # 19. MESH STATISTICS
    # ========================================================

    node_tags, _, _ = (
        gmsh.model.mesh.getNodes()
    )


    element_types, element_tags, _ = (
        gmsh.model.mesh.getElements(3)
    )


    n_elements = sum(
        len(tags)
        for tags in element_tags
    )


    print()
    print(
        "========================================"
    )
    print(
        " MESH STATISTICS"
    )
    print(
        "========================================"
    )


    print(
        f"Nodes       = "
        f"{len(node_tags)}"
    )


    print(
        f"3D elements = "
        f"{n_elements}"
    )


    # ========================================================
    # 20. SAVE MESH
    # ========================================================

    gmsh.write(
        OUTPUT_FILE
    )


    print()
    print(
        "========================================"
    )
    print(
        " SUCCESS"
    )
    print(
        "========================================"
    )


    print(
        f"Mesh saved as: "
        f"{OUTPUT_FILE}"
    )


    # ========================================================
    # 21. OPEN GUI
    # ========================================================

    if SHOW_GUI:

        print()
        print(
            "Opening Gmsh GUI..."
        )

        print(
            "Target disk axis = Y"
        )

        print(
            "Close the Gmsh window to finish."
        )

        gmsh.fltk.run()


# ============================================================
# ERROR HANDLING
# ============================================================

except Exception as error:

    print()
    print(
        "========================================"
    )
    print(
        " ERROR"
    )
    print(
        "========================================"
    )

    print(error)

    gmsh.finalize()

    sys.exit(1)


# ============================================================
# FINISH
# ============================================================

gmsh.finalize()
