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

//' Calculating cosZ
//' @param j hour of day
//' @export
// [[Rcpp::export]]
double s_coszs_R(int jday, int j, double lat, double lon){
    double CosZs;
    s_coszs(jday, j, lat, lon, &CosZs);
    return CosZs;
}

//' @export
// [[Rcpp::export]]
NumericVector readcoef_R(int lc, int stxt) {
    const int N = 48;
    NumericVector ans(N);
    double coef[N];
    readcoef(lc, stxt, coef); // 传递数组works
    for(int i = 0; i < N; i++) 
        ans[i] = coef[i];
    return ans;
}

//' @export
// [[Rcpp::export]]
int beps(String inp_dir){
    char *indir = (char*)inp_dir.get_cstring();
    return beps_c(indir);
}

#endif
