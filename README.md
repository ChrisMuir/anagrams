
<!-- README.md is generated from README.Rmd. Please edit that file -->
anagrams
========

[![Travis-CI Build Status](https://travis-ci.org/ChrisMuir/anagrams.svg?branch=master)](https://travis-ci.org/ChrisMuir/anagrams)

`anagrams` is a super simple R package providing a function for comparing character vectors and determining if strings are [anagrams](https://en.wikipedia.org/wiki/Anagram) of one another. The project was inspired by [this](http://www.programmingr.com/content/simple-anagram-finder-using-r/) blog post on finding anagrams in R. The package uses C++ and [Rcpp](https://CRAN.R-project.org/package=Rcpp) for speed.

Installation
------------

Install from github with:

``` r
# install.packages("devtools")
devtools::install_github("ChrisMuir/anagrams")
```

Example Usage
-------------

The exported function `is_anagram` takes as input a string and a character vector, and returns a logical vector with length equal to that of the character vector.

``` r
library(anagrams)

# Test for anagrams that are the same length as the input string.
is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts"))
#> [1] FALSE  TRUE FALSE  TRUE FALSE

# Set arg "any_len" to TRUE to test for anagrams that are any length (either same length or sub-string).
is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts"), any_len = TRUE)
#> [1]  TRUE  TRUE FALSE  TRUE FALSE

# Use arg "ignore_spaces" to make anagram searching insensitive to spaces.
is_anagram("s t a c", c("cats are great", "t acs", "frogs", "ca   ts", "ts"), ignore_spaces = TRUE)
#> [1] FALSE  TRUE FALSE  TRUE FALSE

# Use arg "ignore_case" to make anagram searching insensitive to lower/upper case.
is_anagram("STAc", c("catS are great", "tacs", "frogs", "CaTS", "ts"), ignore_case = TRUE)
#> [1] FALSE  TRUE FALSE  TRUE FALSE
```

Benchmarks
----------

Let's create a simple, pure R version of `is_anagram` that searches for same length anagrams, and compare speeds.

``` r
r_is_anagram <- function(string, terms) {
  # Split both inputs into char vectors of individual chars.
  string <- unlist(strsplit(string, "", fixed = TRUE))
  terms <- strsplit(terms, "", fixed = TRUE)
  
  # Get number of chars of input "string"
  str_len <- length(string)
  
  vapply(terms, function(x) {
    if (length(x) != str_len) {
      return(FALSE)
    }
    if (identical(x, string)) {
      return(TRUE)
    }
    
    # Check if all chars of "string" appear in those of x.
    all(string %in% x)
  }, logical(1))
}

# Test to make sure its output is identical to that of pkg function is_anagram.
identical(
  r_is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts")), 
  is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts"))
)
#> [1] TRUE
```

Also create a quick convenience function for generating random strings.

``` r
get_rand_str <- function(n, str_len) {
  vapply(seq_len(n), function(x) {
    paste(letters[sample(length(letters), str_len, replace = TRUE)], collapse = "")
  }, character(1))
}
```

Now we'll compare speeds.

``` r
library(microbenchmark)

# Test in which each element is shorter than the input string.
test_vect <- get_rand_str(n = 100000, str_len = 3)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 100
)
#> Unit: milliseconds
#>  expr      min       lq      mean   median        uq       max neval
#>   rfn 86.18531 91.21542 106.48151 94.60389 126.30866 140.31029   100
#>   cpp 16.72535 17.23285  17.53494 17.56935  17.77884  18.37779   100


# Test in which each element is the same length as the input string.
test_vect <- get_rand_str(n = 100000, str_len = 4)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 100
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq       max neval
#>   rfn 271.01454 287.73329 310.58123 297.95937 340.05847 352.65563   100
#>   cpp  30.07249  31.08401  31.50718  31.57794  31.89025  32.76963   100


# Test in which each element is an anagram of the input string.
test_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 100
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq       max neval
#>   rfn 226.08139 243.98871 259.55139 250.69417 282.54758 300.68308   100
#>   cpp  23.23763  23.80818  24.36091  24.38386  24.77003  26.54729   100
```
