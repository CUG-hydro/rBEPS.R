test_that("beps_main works", {
  expect_silent({
    # indir = system.file("examples/input", package = "rBEPS")
    beps(system.file("examples/input", package = "rBEPS"))
  })
})
