/// @file sensible_heat.c
/// @brief This module will calculate sensible heat from overstorey, understorey and ground
/// @author Edited by XZ Luo
/// @date May 23, 2015

#include "beps.h"

/// @brief Function to calculate sensible heat
/// @details [input] temperature of sunlit and shaded leaves from other storey (leaf temperature module);
///                  temperature of air; relative humidity;
///                  temperature of ground (soil heat flux module);
///                  aerodynamic heat conductance of sunlit shaded leaves from overstorey and understorey;
///                  aerodynamic heat conductance of ground;
///                  leaf area index, sunlit and shaded, overstorey and understorey (from leaf area index module);
/// @details [output] sensible heat from overstorey, understorey and ground
/// @param tempL             temperature of leaves
/// @param temp_g            temperature of ground
/// @param temp_air          air temperature
/// @param rh_air            relative humidity of air
/// @param Gheat             aerodynamic resistance of heat
/// @param Gheat_g           aerodynamic resistance of heat, ground
/// @param lai               leaf area index
/// 
/// @param SH_o              sensible heat, overstory
/// @param SH_u              sensible heat, understory
/// @param SH_g              sensible heat, ground
/// @return void
void sensible_heat(
    Leaf tempL, double temp_g, double temp_air, double rh_air,
    Leaf Gheat, double Gheat_g, Leaf lai,
    double* SH_o, double* SH_u, double* SH_g) 
{
    Leaf SH;                                                    // sensible heat from leaves
    double meteo_pack_output[10];

    meteo_pack(temp_air, rh_air, meteo_pack_output);
    double density_air0 = meteo_pack_output[1];
    double cp_air = meteo_pack_output[2];
    // double vpd = meteo_pack_output[3];

    /********************************************/
    SH.o_sunlit = (tempL.o_sunlit - temp_air) * density_air0 * cp_air * Gheat.o_sunlit;
    SH.o_shaded = (tempL.o_shaded - temp_air) * density_air0 * cp_air * Gheat.o_shaded;

    SH.u_sunlit = (tempL.u_sunlit - temp_air) * density_air0 * cp_air * Gheat.u_sunlit;
    SH.u_shaded = (tempL.u_shaded - temp_air) * density_air0 * cp_air * Gheat.u_shaded;
    /********************************************/

    *SH_o = SH.o_sunlit * lai.o_sunlit + SH.o_shaded * lai.o_shaded;
    *SH_u = SH.u_sunlit * lai.u_sunlit + SH.u_shaded * lai.u_shaded;

    *SH_o = max(-200, *SH_o);
    *SH_u = max(-200, *SH_u);

    *SH_g = (temp_g - temp_air) * density_air0 * cp_air * Gheat_g;
}
