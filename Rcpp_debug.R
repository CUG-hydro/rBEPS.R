devtools::load_all()
# beps(system.file("examples/input", package = "rBEPS"))

library(data.table)

d_metro <- fread("inst/examples/input/p1_meteo.txt")
colnames(d_metro) <- c("day", "hour", "rad", "tem", "hum", "pre", "wind")

LAI <- read.table("inst/examples/input/p1_lai.txt") %>%
    unlist() %>%
    as.numeric()

opts <- c(
    long = 120.5, lat = 30.5,
    clumping = 0.85,
    LC = 25,
    soil_type = 8,
    soiltemp = 2.2,
    soilwater = 0.4115,
    snowdepth = 0, 
    j_start = 1, 
    j_end = 20
)
indir <- "inst/examples/input"
res1 = beps_main(indir, d_metro, LAI, opts) %>% mat2df()
res2 = beps_main(indir, d_metro, LAI, opts)
