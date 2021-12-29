#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
DataFrame mat2dt(NumericMatrix mat) {
    Environment pkg = Environment::namespace_env("data.table");
    Function func = pkg["as.data.table"];
    return func(mat);
}

// [[Rcpp::export]]
DataFrame mat2df(NumericMatrix mat) {
    Environment pkg = Environment::namespace_env("tibble");
    Function func = pkg["as_tibble"];
    return func(mat);
}
