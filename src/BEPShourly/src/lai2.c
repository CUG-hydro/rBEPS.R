/// @file lai_cacl.c
/// @brief recalculate sunlit and shaded leaf area index
/// @author Bin Chen
/// @date May, 2015

#include "beps.h"

// int LC, 

/// @brief Function to recalculate sunlit and shaded leaf area index
/// @param  stem_o        overstory woody area
/// @param  stem_u        understory woody area
/// @param  LC            land cover type
/// @param  CosZs         cosine solar zenith angle
/// @param  lai_o         overstory lai
/// @param  clumping      clumping index
/// @param  lai_u         understory lai
/// @param  lai           LAI
/// @param  PAI           LAI + SAI
/// @return void
void lai2(double clumping, double CosZs, double stem_o, double stem_u, double lai_o, double lai_u,
    Leaf* LAI, Leaf* PAI) 
{
    if (CosZs > 0)
        PAI->o_sunlit = 2 * CosZs * (1 - exp(-0.5 * clumping * (lai_o + stem_o) / CosZs));
    else
        PAI->o_sunlit = 0;

    PAI->o_shaded = (lai_o + stem_o) - PAI->o_sunlit;

    if (CosZs > 0)
        PAI->u_sunlit = 2 * CosZs * (1 - exp(-0.5 * clumping * (lai_o + stem_o + lai_u + stem_u) / CosZs)) - PAI->o_sunlit;
    else
        PAI->u_sunlit = 0;

    PAI->u_shaded = (lai_u + stem_u) - PAI->u_sunlit;

    if (CosZs > 0)
        LAI->o_sunlit = 2 * CosZs * (1 - exp(-0.5 * clumping * lai_o / CosZs));
    else
        LAI->o_sunlit = 0;

    //LAI->o_shaded = max(0,lai_o-PAI->o_sunlit);  // original
    LAI->o_shaded = max(0, lai_o - LAI->o_sunlit);  // edited by J. Leng

    if (CosZs > 0)
        LAI->u_sunlit = 2 * CosZs * (1 - exp(-0.5 * clumping * (lai_o + lai_u) / CosZs)) - LAI->o_sunlit;
    else
        LAI->u_sunlit = 0;

    //LAI->u_shaded = max(0,lai_u-PAI->u_sunlit);  // original
    LAI->u_shaded = max(0, lai_u - LAI->u_sunlit);  // edited by J. Leng
}
