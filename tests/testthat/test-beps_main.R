test_that("beps_main works", {
  expect_silent({
    beps("src/BEPShourly/examples/input")
  })
  # expect_equal(2 * 2, 4)
})
