void target_number_density(double temperature_K = 180.0,
                           double pressure_bar = 1.5,
                           double L_target = 0.5)
{
    if (!std::isfinite(temperature_K) || temperature_K <= 0
        || !std::isfinite(pressure_bar) || pressure_bar <= 0
        || !std::isfinite(L_target) || L_target <= 0)
        throw std::runtime_error("Temperature [K], absolute pressure [bar] and target length [cm] must be finite and positive");

    const double R = 8.31;              // J/(mol K), same as target_thickness.cc
    const double M = 39.948e-3;         // kg/mol, Ar
    const double NA = 6.02214076e23;    // mol^-1
    const double P = pressure_bar * 1e5; // bar -> Pa

    // Ideal gas (Z = 1): P/(R T) is the molar density in mol/m^3.
    const double molar_density = P / (R * temperature_K);
    const double number_density = molar_density * NA * 1e-6; // atoms/cm^3
    const double target_number_density = number_density * L_target; // atoms/cm^2
    const double rho = molar_density * M * 0.001; // kg/m^3 -> g/cm^3
    const double mass_thickness = rho * L_target; // g/cm^2
    if (!std::isfinite(number_density) || number_density <= 0
        || !std::isfinite(target_number_density) || target_number_density <= 0
        || !std::isfinite(rho) || rho <= 0
        || !std::isfinite(mass_thickness) || mass_thickness <= 0)
        throw std::runtime_error("Invalid ideal-gas target density");

    // Format locally so later macros keep their own console precision.
    std::ostringstream output;
    output << "Ar target (ideal gas, Z = 1 assumed)\n"
           << "Temperature               = " << temperature_K << " K\n"
           << "Absolute pressure         = " << pressure_bar << " bar\n"
           << "Target length             = " << L_target << " cm\n"
           << std::scientific << std::setprecision(6)
           << "Mass density              = " << rho << " g/cm^3\n"
           << "Mass thickness            = " << mass_thickness * 1000 << " mg/cm^2\n"
           << "Volume number density     = " << number_density << " atoms/cm^3\n"
           << "Areal target number density = " << target_number_density << " atoms/cm^2\n";
    std::cout << output.str();
}
