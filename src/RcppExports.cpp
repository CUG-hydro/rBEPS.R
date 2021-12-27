// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// S_coszs
double S_coszs(int jday, int j, double lat, double lon);
RcppExport SEXP _rBEPS_S_coszs(SEXP jdaySEXP, SEXP jSEXP, SEXP latSEXP, SEXP lonSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type jday(jdaySEXP);
    Rcpp::traits::input_parameter< int >::type j(jSEXP);
    Rcpp::traits::input_parameter< double >::type lat(latSEXP);
    Rcpp::traits::input_parameter< double >::type lon(lonSEXP);
    rcpp_result_gen = Rcpp::wrap(S_coszs(jday, j, lat, lon));
    return rcpp_result_gen;
END_RCPP
}
// Readcoef
NumericVector Readcoef(int lc, int stxt);
RcppExport SEXP _rBEPS_Readcoef(SEXP lcSEXP, SEXP stxtSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type lc(lcSEXP);
    Rcpp::traits::input_parameter< int >::type stxt(stxtSEXP);
    rcpp_result_gen = Rcpp::wrap(Readcoef(lc, stxt));
    return rcpp_result_gen;
END_RCPP
}
// beps
int beps(String inp_dir);
RcppExport SEXP _rBEPS_beps(SEXP inp_dirSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type inp_dir(inp_dirSEXP);
    rcpp_result_gen = Rcpp::wrap(beps(inp_dir));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_rBEPS_S_coszs", (DL_FUNC) &_rBEPS_S_coszs, 4},
    {"_rBEPS_Readcoef", (DL_FUNC) &_rBEPS_Readcoef, 2},
    {"_rBEPS_beps", (DL_FUNC) &_rBEPS_beps, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_rBEPS(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
