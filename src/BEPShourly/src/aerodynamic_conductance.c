/// @file aerodynamic_conductance.c
/// @brief Calculation of aerodynamic resistance/conductance.
/// @authors Written by: J. Liu and W. Ju
/// @authors Modified by G. Mo
/// @date Last update:  May 2015

#include "beps.h"

/**
 * @brief Function to calculate aerodynamic resistance and conductance
 *
 * @param  canopy_height_o  canopy height, overstory
 * @param  canopy_height_u  height of understory
 * @param  zz               the height to measure wind speed
 * @param  clumping         clumping index
 * @param  temp_air         air temperature
 * @param  wind_sp          wind speed
 * @param  SH_o_p           sensible heat flux from overstory
 * @param  lai_o            leaf area index, overstory (lai_o+stem_o)
 * @param  lai_u            leaf area index, understory (lai_u+stem_u)
 *
 * @return
 * - `ra_o`  : aerodynamic resistance, overstory, in s/m
 * - `ra_u`  : aerodynamic resistance, understory, in s/m
 * - `ra_g`  : aerodynamic resistance, ground, in s/m
 *
 * - `Ga_o` : aerodynamic conductance for leaves, overstory
 * - `Ga_u` : aerodynamic conductance for leaves, understory
 * - `Gb_o` : boundary layer conductance for leaves, overstory
 * - `Gb_u` : boundary layer conductance for leaves, understory
 */
void aerodynamic_conductance(
    double canopy_height_o, double canopy_height_u, double zz, double clumping,
    double temp_air, double wind_sp, double SH_o_p, double lai_o, double lai_u,
    double *ra_o, double *ra_u, double *ra_g, double *Ga_o, double *Gb_o, double *Ga_u, double *Gb_u) 
{
    double kh_o;
    double rb_o, rb_u;           // leaf boundary layer for overstory and understory
    double k = 0.4;              // von Karman's constant
    // double lw = 0.3;             // leaf characteristic width =0.3 for BS
    // double sigma = 5;            // shelter factor =5 for BS
    // double beta = 0.5;           // Bowen's ratio
    double cp = 1010;            // specific heat  of air (J/kg/K)
    double density_air = 1.225;  // density of air at 15 C (kg/m3)
    double gg = 9.8;             // gravitational acceleration (m/s2)
    double n = 5.0;

    double nu_lower;  // viscosity (cm2/s)
    // double uf;
    double psi;
    double d;      // displacement height (m)
    double z0;     // roughness length (m)
    double ustar;  // friction velocity (m/s)
    double L;
    double Le;

    double uh;  // wind speed at height h
    double ud;  // wind speed at height d
    double gamma;
    double Re;  // Reynold's number
    double Nu;  // Nusselt number
    double alfaw;

    double ram;
    double un_d;  // un_t, kh_u;

    nu_lower = (13.3 + temp_air * 0.07) / 1000000;
    // double alfac = 0.15;  // for CO2
    //alfaw=0.25;	// for H2O
    alfaw = (18.9 + temp_air * 0.07) / 1000000;

    if (wind_sp == 0) {
        uh = 0;
        // uf = 0;
        psi = 6;
        *Ga_o = 1 / 200.0;
        *Gb_o = 1 / 200.0;
        *Ga_u = 1 / 200.0;
        *Gb_u = 1 / 200.0;
        *ra_g = 300;
    } else {
        d = 0.8 * canopy_height_o;
        z0 = 0.08 * canopy_height_o;

        ustar = wind_sp * k / log((zz - d) / z0);
        L = -(k * gg * SH_o_p) / (density_air * cp * (temp_air + 273.3) * pow(ustar, 3));
        L = max(-2.0, L);

        ram = 1 / (k * ustar) * (log((zz - d) / z0) + (n * (zz - d) * L));
        ram = max(2, ram);
        ram = min(100, ram);

        if (L > 0)
            psi = 1 + 5 * (zz - d) * L;
        else
            psi = pow((1 - 16 * (zz - d) * L), -0.5);
        psi = min(10.0, psi);

        /********* Leaf boundary layer resistance ******************/
        /* Wind speed at tree top */
        uh = 1.1 * ustar / k;
        Le = lai_o * clumping;
        gamma = (0.167 + 0.179 * uh) * pow(Le, 1.0 / 3.0);

        /* Wind speed at d, taking as the mean wind speed inside a stand */
        ud = uh * exp(-gamma * (1 - d / canopy_height_o));

        /* Reynold's number */
        //Re=(ud*lw/sigma)/nu_lower;
        Re = (ud * 0.1) / nu_lower;

        /* Nusselt number */
        Nu = 1.0 * pow(Re, 0.5);

        /* leaf boundary resistance */
        rb_o = min(40, 0.5 * 0.1 / (alfaw * Nu));

        // uf = ustar;
        *ra_o = ram;
        *Ga_o = 1 / ram;
        *Gb_o = 1 / rb_o;

        kh_o = 0.41 * ustar * (canopy_height_o - canopy_height_o * 0.8) / psi;
        gamma = 0.1 + pow(lai_o, 0.75);

        // wind speed at the zero displacement of canopy
        un_d = uh * exp(-gamma * (1 - canopy_height_u * 0.8 / canopy_height_o));
        // wind speed at the zero displacement of canopy
        // double un_t = uh * exp(-gamma * (1 - canopy_height_u / canopy_height_o));

        /* Reynold's number */
        //Re=(ud*lw/sigma)/nu_lower;
        Re = (un_d * 0.1) / nu_lower;

        /* Nusselt number */
        Nu = 1.0 * pow(Re, 0.5);

        /* leaf boundary resistance */
        rb_u = 0.5 * 0.1 / (alfaw * Nu);
        rb_u = min(40, rb_u);
        *Gb_u = 1.0 / rb_u;

        *ra_u = canopy_height_o / (gamma * kh_o) * (exp(gamma * (1 - canopy_height_u / canopy_height_o)) - 1);
        *Ga_u = 1 / (ram + *ra_u);

        gamma = 4.0;
        // double kh_u = kh_o * exp(-gamma * (1 - canopy_height_u / canopy_height_o));

        *ra_g = canopy_height_o / (gamma * kh_o) *
                (exp(gamma * (1 - 0 / canopy_height_o)) - exp(gamma * (1 - canopy_height_u / canopy_height_o)));
        *ra_g = *ra_g + *ra_u + ram;
        *ra_g = max(120, *ra_g);
    }
    return;
}
