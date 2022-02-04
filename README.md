
# rBEPS

Boreal Ecosystem Productivity Simulator in R

<!-- badges: start -->
<!-- badges: end -->

## Installation

You can install the released version of rBEPS from [CRAN](https://CRAN.R-project.org) with:

``` r
install.packages("rBEPS")
```

## Example

This is a basic example which shows you how to solve a common problem:

``` r
library(rBEPS)
beps(system.file("examples/input", package = "rBEPS"))
# simulating ...  
# total GPP: 2128.598891   ET: 1164.599855        NEP: 982.954108
```

## References

1. Hourly BEPS model. <https://github.com/JChen-UToronto/BEPS_hourly_site>

2. Daily BEPS model developed by Jane Liu. <https://github.com/JChen-UToronto/BEPS_D>
