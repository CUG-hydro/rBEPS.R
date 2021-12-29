test_that("beps_main works", {
  expect_silent({
    # indir = system.file("examples/input", package = "rBEPS")
    indir = system.file("examples/input", package = "rBEPS")
    beps(indir)
    beps_main(indir, d_metro, LAI, opts)
  })
})


indir = "inst/examples/input"
