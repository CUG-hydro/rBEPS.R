#ifndef BEPS_R_H_
#define BEPS_R_H_

// #include <cstring>
#include <Rcpp.h>
using namespace Rcpp;
#undef ERROR /*replace C error with Rcpp*/
#undef PI    /*redefined*/

extern "C" {
    #include <beps.h>
}

// #define VIC_DRIVER "R"

// //' Calculating cosZ
// //' @param j hour of day
// //' @export
// // [[Rcpp::export]]
// double s_coszs(int jday, int j, double lat, double lon){
//     double CosZs;
//     s_coszs(jday, j, lat, lon, &CosZs);
//     return CosZs;
// }

//' @export
// [[Rcpp::export]]
int beps(String inp_dir){
    char *indir = (char*)inp_dir.get_cstring();
    return beps_c(indir);
}

#endif
