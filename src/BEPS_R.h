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
//' @examples
//' S_coszs(1, 1, 15, 120)
// [[Rcpp::export]]
double S_coszs(int jday, int j, double lat, double lon){
    double CosZs;
    s_coszs(jday, j, lat, lon, &CosZs);
    return CosZs;
}

//' Real soil coefficients
//'
//' @param lc    land cover type
//'               1-ENF 2-DNF 6-DBF 9-EBF 13-Shrub 40-C4 Plants default:Others
//' @param stxt  soil texture, 1-11
//' @export
//' @examples
//' Readcoef(1, 1)
// [[Rcpp::export]]
NumericVector Readcoef(int lc, int stxt) {
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
