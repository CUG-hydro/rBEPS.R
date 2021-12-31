#include "beps.h"

void init_leaf_struct(Leaf* x, Leaf* replacement) {
    x->o_sunlit = replacement->o_sunlit;
    x->o_shaded = replacement->o_shaded;
    x->u_sunlit = replacement->u_sunlit;
    x->u_shaded = replacement->u_shaded;
}

void init_leaf_dbl(Leaf* x, double replacement) {
    x->o_sunlit = replacement;
    x->o_shaded = replacement;
    x->u_sunlit = replacement;
    x->u_shaded = replacement;
}

void init_leaf_dbl2(Leaf* x, double overstory, double understory) {
    x->o_sunlit = overstory;
    x->o_shaded = overstory;
    x->u_sunlit = understory;
    x->u_shaded = understory;
}
