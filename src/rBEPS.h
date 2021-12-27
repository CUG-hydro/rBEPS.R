#ifndef rBEPS_H_
#define rBEPS_H_

// #include <cstring>
#include <Rcpp.h>
using namespace Rcpp;

#undef ERROR /*replace C error with Rcpp*/
#undef PI    /*redefined*/

extern "C" {
    #include <beps.h>
}

NumericVector c2numvec (double x[], int N){
    NumericVector ans(N);
    for (int i = 0; i < N; i++)
        ans[i] = x[i];
    return ans;
}

//' @export
// [[Rcpp::export]]
double Leaf_Temperature_(double Tair, double VPD_air, double Cp_ca,
                             double Gw, double Gww, double Gh, double Xc_sl, double radiation, bool constrain = false) {
    Environment pkg = Environment::namespace_env("hydroTools");
    Function cal_slope = pkg["cal_slope"];
    Function cal_gamma = pkg["cal_gamma"];
    double slope1 = *REAL(cal_slope(Tair));
    double gamma1 = *REAL(cal_gamma(Tair));
    double Cp_ca1 = 1013; // J kg-1 degC-1
    return Leaf_Temperature(Tair, slope1, gamma1, VPD_air, Cp_ca1,
                            Gw, Gww, Gh, Xc_sl, radiation, constrain);
}

//' @export
// [[Rcpp::export]]
NumericVector lai2_(double clumping, double CosZs, double stem_o, double stem_u, double lai_o, double lai_u) {

    double lai_o_sunlit, lai_o_shaded, lai_u_sunlit, lai_u_shaded, 
        PAI_o_sunlit, PAI_o_shaded, PAI_u_sunlit, PAI_u_shaded;

    lai2(clumping, CosZs, stem_o, stem_u, lai_o, lai_u,
          &lai_o_sunlit, &lai_o_shaded, &lai_u_sunlit, &lai_u_shaded,
          &PAI_o_sunlit, &PAI_o_shaded, &PAI_u_sunlit, &PAI_u_shaded);
    
    auto v = NumericVector::create(
        lai_o_sunlit, lai_o_shaded, lai_u_sunlit, lai_u_shaded, 
        PAI_o_sunlit, PAI_o_shaded, PAI_u_sunlit, PAI_u_shaded);
    v.names() = CharacterVector({
        "lai_o_sunlit", "lai_o_shaded", "lai_u_sunlit", "lai_u_shaded", 
        "PAI_o_sunlit", "PAI_o_shaded", "PAI_u_sunlit", "PAI_u_shaded"
    });
    return v;
}

//' @export
// [[Rcpp::export]]
NumericVector meteo_pack_(double temp, double rh) {
    const int n = 9;
    double meteo_pack_output[n];
    meteo_pack(temp, rh, meteo_pack_output);

    auto v = c2numvec(meteo_pack_output, n);
    v.names() = CharacterVector({"lambda", "density_air", "cp_air", "vpd", "slope_vapor", "psy", "e_saturate", "e_actual", "sp_humidity"});
    return v;
}

//' Calculating cosZ
//' @param j hour of day
//' @export
//' @examples
//' s_coszs_(1, 1, 15, 120)
// [[Rcpp::export]]
double s_coszs_(int jday, int j, double lat, double lon) {
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
//' readcoef_(1, 1)
// [[Rcpp::export]]
NumericVector readcoef_(int lc, int stxt) {
    const int N = 48;
    double coef[N];
    readcoef(lc, stxt, coef); // 传递数组works
    return c2numvec(coef, N);
}

//' @export
// [[Rcpp::export]]
int beps(String inp_dir){
    char *indir = (char*)inp_dir.get_cstring();
    return beps_c(indir);
}

#endif
