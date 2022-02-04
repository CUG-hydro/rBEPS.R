#! /usr/bin/Rscript
devtools::load_all()
# beps(system.file("examples/input", package = "rBEPS"))

library(data.table)
library(dplyr)

d_metro <- fread("inst/examples/input/p1_meteo.txt")
colnames(d_metro) <- c("jday", "hour", "rad", "tem", "hum", "pre", "wind")

LAI <- read.table("inst/examples/input/p1_lai.txt") %>%
    unlist() %>%
    as.numeric()
d_LAI = data.table(jday = 1:365, LAI)

d = d_metro %>% data.table()
d_day = d[, mean(rad), .(jday)]
plot(d_day$V1, type = "l")

opts <- c(
    long = 120.5, lat = 30.5,
    clumping = 0.85,
    LC = 25,
    soil_type = 8,
    soiltemp = 2.2,
    soilwater = 0.4115,
    snowdepth = 0
    # j_start = 1, 
    # j_end = 365
)

func <- function() {
    beps_main(d_metro, d_LAI$LAI, opts) %>% mat2df() %>% 
        dplyr::select(-starts_with("npp"), -ends_with("resp"), -NEP) %>% 
        .[, -(1:4)] %>% 
        dplyr::mutate(ET = Trans + Evap) %>% 
        tibble::add_column(d_metro[, 1:2], .before = 1)
}

# indir <- "inst/examples/input"
res1 = func(); print(res1)
res2 = func(); print(res2)

diff = tibble::as_tibble(res1 - res2)
print(diff)

err = colSums(diff) %>% as.numeric() %>% abs() %>% max()
err <= 1e-4


## 2. Visualization ------------------------------------------------------------
library(ggplot2)
library(tidyr)

d = res1 %>% group_by(jday) %>% select(-hour) %>% 
    summarise(across(everything(), sum))

dat = pivot_longer(d, -jday, "variable")

p = ggplot(dat, aes(jday, value)) +
    geom_line() + 
    facet_wrap(~variable, scale = "free_y")
write_fig(p, "a.pdf")
