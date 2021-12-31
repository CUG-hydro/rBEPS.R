/// @file transpiration.c
/// @brief This module calculates transpiration, for overstorey and understorey, sunlit and shaded
/// @author Edited by XZ Luo
/// @date May 20, 2015

#include "beps.h"

/// @brief Function to calculate transpiration
/// @details A transformation of Penman-Monteith equation is used here.
///          It could be regarded as a mass transfer process.
///          Water vapor inside cells are required by VPD from air and VPD on leaf surface.
/// @details [input] temperature of sunlit and shaded leaves from other storey (leaf temperature module);
///                  temperature of air; relative humidity;
///                  conductance of water for sunlit shaded leaves from overstorey and understorey;
///                  leaf area index, sunlit and shaded, overstorey and understorey (from leaf area index module);
/// @details [output] transpiration from overstorey and understorey
/// @param tempL.o_sunlit  temperature of leaf
/// @param temp_air        air temperature
/// @param rh_air          relative humidity of air
/// @param Gtrans.o_sunlit total conductance of water
///                        tandem of stomatal conductance and aerodynamic conductance
/// @param lai.o_sunlit    leaf area index
/// @param trans_o         transpiration from overstory
/// @param trans_u         transpiration from understory
/// @return void
void transpiration(
    Leaf tempL, double temp_air, double rh_air,
    Leaf Gtrans, Leaf lai, 
    double* trans_o, double* trans_u) 
{
    Leaf LHt;
    // double LHt.o_sunlit, LHt.o_shaded, LHt.u_sunlit, LHt.u_shaded;  // latent heat from leaves W/m2
    double meteo_pack_output[10];
    double density_air, cp_air, vpd_air, slope_vapor_air, psy_air;
    double latent_water;

    meteo_pack(temp_air, rh_air, meteo_pack_output);
    density_air     = meteo_pack_output[1];
    cp_air          = meteo_pack_output[2];  // specific heat of moist air above canopy
    vpd_air         = meteo_pack_output[3];
    slope_vapor_air = meteo_pack_output[4];  // slope of saturated vapor potential to temperature
    psy_air         = meteo_pack_output[5];          // psychrometer constant
    
    latent_water = (2.501 - 0.00237 * temp_air) * 1000000;

    LHt.o_sunlit = (vpd_air + slope_vapor_air * (tempL.o_sunlit - temp_air)) * density_air * cp_air * Gtrans.o_sunlit / psy_air;
    LHt.o_shaded = (vpd_air + slope_vapor_air * (tempL.o_shaded - temp_air)) * density_air * cp_air * Gtrans.o_shaded / psy_air;
    LHt.u_sunlit = (vpd_air + slope_vapor_air * (tempL.u_sunlit - temp_air)) * density_air * cp_air * Gtrans.u_sunlit / psy_air;
    LHt.u_shaded = (vpd_air + slope_vapor_air * (tempL.u_shaded - temp_air)) * density_air * cp_air * Gtrans.u_shaded / psy_air;

    *trans_o = 1 / (latent_water) * (LHt.o_sunlit * lai.o_sunlit + LHt.o_shaded * lai.o_shaded);
    *trans_u = 1 / (latent_water) * (LHt.u_sunlit * lai.u_sunlit + LHt.u_shaded * lai.u_shaded);
}
