#ifndef POTASSIUM_KINEMATICS_CONSTANTS_HH
#define POTASSIUM_KINEMATICS_CONSTANTS_HH

#include <cmath>
#include <array>

namespace kinematics {
// Atomic masses [u], matching the values used in draw_level_scheme.py.
// Neutral-atom mass convention; electron binding corrections are neglected.
constexpr double hydrogen1_u = 1.007825031898;
constexpr double helium4_u = 4.00260325413;
constexpr double argon40_u = 39.96238312204;
constexpr double chlorine37_u = 36.965902573;
constexpr double atomic_mass_mev = 931.49410242; // u*c^2 [MeV]

constexpr double default_max_beam_mev = 10.0; // Total LAB kinetic energy.
constexpr int graph_intervals = 1000;
constexpr double default_proton_beam_mev = 3.5;
constexpr double default_flight_path_m = 2.0;
constexpr double default_time_sigma_ns = 0.100; // 100 ps, Gaussian sigma.
constexpr double speed_of_light_m_ns = 0.299792458;
constexpr int tof_events_per_level = 100000;
constexpr double tof_bin_width_ns = 0.025;
constexpr unsigned tof_random_seed = 20261117;
// AME2020 mass excesses [MeV], as in draw_level_scheme.py.
constexpr double neutron_mass_mev = atomic_mass_mev + 8.07131806;
constexpr double potassium40_mass_mev = 40.0 * atomic_mass_mev - 33.535497;
// Selected 40K excitation energies [MeV] from the existing level scheme.
constexpr std::array<double, 4> potassium40_excitation_mev = {{0.0, 0.0298299, 0.8001431, 0.891394}};

// Exact relativistic entrance-channel CM kinetic energy for a stationary target.
// Rationalized form avoids cancellation at small beam energies.
inline double cm_energy(double beam_mev, double projectile_u, double target_u) {
    const double projectile = projectile_u * atomic_mass_mev;
    const double target = target_u * atomic_mass_mev;
    const double rest = projectile + target;
    return 2.0 * target * beam_mev /
           (std::sqrt(rest * rest + 2.0 * target * beam_mev) + rest);
}
} // namespace kinematics

#endif
