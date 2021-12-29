#ifndef RBEPS_H_
#define RBEPS_H_

// #include <cstring>
#include <Rcpp.h>
using namespace Rcpp;

#undef ERROR /*replace C error with Rcpp*/
#undef PI    /*redefined*/

extern "C" {
    #include <beps.h>
}

NumericVector results2vec(struct results* r);
NumericVector c2numvec(double x[], int N);
double s_coszs_(int jday, int j, double lat, double lon);
double Leaf_Temperature_(double Tair, double VPD_air, double Cp_ca,
                         double Gw, double Gww, double Gh, double Xc_sl, double radiation, bool constrain);
NumericVector lai2_(double clumping, double CosZs, double stem_o, double stem_u, double lai_o, double lai_u);
NumericVector meteo_pack_(double temp, double rh);
NumericVector readcoef_(int lc, int stxt);

int beps(String inp_dir);
NumericMatrix beps_main(String inp_dir, DataFrame d_metro, NumericVector LAI, NumericVector opts);


#endif
