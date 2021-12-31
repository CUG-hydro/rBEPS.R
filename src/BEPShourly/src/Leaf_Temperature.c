/// @file calc_temp_leaf.c
/// @brief Subroutine to calculate the sunlit and shaded leaf temperatures for overstory and understory leave.
/// @authors Written and refactored by Liming He (liming.he@gmail.com)
/// @authors Original contributor: Weimin Ju
/// @date Last update: Sept. 15, 2015
/// @date Created on May 15, 2015

#include "beps.h"

/// @brief Function to calculate leaf temperature four components
///        (sunlit and shaded leaves, overstory and understory)
/// @details [output] Tc.o_sunlit,Tc.o_shaded,Tc.u_sunlit,Tc.u_shaded
/// @param Tair                 air temperature
/// @param slope                the slope of saturation vapor pressure-temperature curve
/// @param psychrometer         psychrometer constant, 0.066 kPa K
/// @param VPD_air              vapor pressure deficit
/// @param Cp_ca                specific heat of moist air in kJ/kg/K
/// @param Gw                   total conductance for water from the intercellular space of the leaves
///                             to the reference height above the canopy
/// @param Gww                  total conductance for water from the surface of the leaves
///                             to the reference height above the canopy
/// @param Gh                   total conductance for heat transfer from the leaf surface
///                             to the reference height above the canopy
/// @param Xcs_o                the fraction of canopy covered by snow, overstory
/// @param Xcl_o                the fraction of canopy covered by liquid water, overstory
/// @param Xcs_u                the fraction of canopy covered by snow, understory
/// @param Xcl_u                the fraction of canopy covered by liquid water, understory
/// @param radiation   net radiation on leaves
/// @param Tc          the effective canopy temperature in Kalvin
/// @return void
void Leaf_Temperatures(double Tair, double slope, double psychrometer, double VPD_air, double Cp_ca,
                       Leaf Gw, Leaf Gww, Leaf Gh,
                       double Xcs_o, double Xcl_o, double Xcs_u, double Xcl_u,
                       Leaf radiation, Leaf* Tc) {
    Tc->o_sunlit = Leaf_Temperature(Tair, slope, psychrometer, VPD_air, Cp_ca,
                                    Gw.o_sunlit, Gww.o_sunlit, Gh.o_sunlit, Xcs_o + Xcl_o, radiation.o_sunlit, true);

    Tc->o_shaded = Leaf_Temperature(Tair, slope, psychrometer, VPD_air, Cp_ca,
                                    Gw.o_shaded, Gww.o_shaded, Gh.o_shaded, Xcs_o + Xcl_o, radiation.o_shaded, true);

    Tc->u_sunlit = Leaf_Temperature(Tair, slope, psychrometer, VPD_air, Cp_ca,
                                    Gw.u_sunlit, Gww.u_sunlit, Gh.u_sunlit, Xcs_u + Xcl_u, radiation.u_sunlit, true);

    Tc->u_shaded = Leaf_Temperature(Tair, slope, psychrometer, VPD_air, Cp_ca,
                                    Gw.u_shaded, Gww.u_shaded, Gh.u_shaded, Xcs_u + Xcl_u, radiation.u_shaded, true);
}

// / @param Xcs            the fraction of canopy covered by snow
// / @param Xcl            the fraction of canopy covered by liquid water

/// @brief Subroutine to calculate leaf temperature
/// @param Tair           air temperature
/// @param slope          the slope of saturation vapor pressure-temperature curve, kPa K-1
/// @param psychrometer   psychrometer constant, 0.066 kPa K
/// @param VPD_air        vapor pressure deficit
/// @param Cp_ca          specific heat of moist air in kJ/kg/K
/// @param Gw             water conductance, from the intercellular space of the leaves
///                       to the reference height above the canopy
/// @param Gww            water conductance, from the surface of the leaves
///                       to the reference height above the canopy
/// @param Gh             total conductance for heat transfer from the leaf surface
///                       to the reference height above the canopy
/// @param Xc_sl          fraction of canopy covered by snow(s) and liqurd (l)
/// @param radiation      net radiation on leaves
/// @return [double Tc] the effective canopy temperature in Kalvin
double Leaf_Temperature(double Tair, double slope, double psychrometer, double VPD_air, double Cp_ca,
                        double Gw, double Gww, double Gh, double Xc_sl, double radiation, bool constrain) {
    double p_star;
    double Tc;
    // double R = 1 / Gw + 1 / (Gww * (Xcs + Xcl));
    // p_star = (Gw + Gww * (Xcs + Xcl)) / psychrometer;
    p_star = (Gw + Gww * (Xc_sl)) / psychrometer;

    Tc = Tair + (radiation - VPD_air * rho_a * Cp_ca * p_star) / (rho_a * Cp_ca * (Gh + slope * p_star));  // temperature of sunlit leaves, overstorey

    if (constrain) {
        Tc = max(Tair - 3.0, Tc);
        Tc = min(Tair + 5.0, Tc);
    }
    return Tc;
}
