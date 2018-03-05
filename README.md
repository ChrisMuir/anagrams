
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

# Use arg "value" to return the values that are anagrams.
is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts"), value = TRUE)
#> [1] "tacs" "cats"

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
  out <- rep(FALSE, length(terms))
  terms_to_insp <- which(nchar(terms) == nchar(string))
  if (length(terms_to_insp) == 0) {
    return(out)
  }
  string_spl <- unlist(strsplit(string, "", fixed = TRUE), FALSE, FALSE)
  str_key <- paste(string_spl[sort.list(string_spl)], collapse = "")
  terms_spl <- strsplit(terms, "", fixed = TRUE)
  
  out[terms_to_insp] <- vapply(terms_spl[terms_to_insp], function(x) {
    if (all(x == string_spl)) {
      return(TRUE)
    }
    paste(x[sort.list(x)], collapse = "") == str_key
  }, logical(1), USE.NAMES = FALSE)
  
  out
}

# Test to make sure its output is identical to that of pkg function is_anagram.
identical(
  r_is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts")), 
  is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts"))
)
#> [1] TRUE
```

Now we'll compare speeds.

``` r
library(microbenchmark)
library(stringi)

# Test in which each element is shorter than the input string.
test_vect <- stringi::stri_rand_strings(100000, 3)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect)
)
#> Unit: milliseconds
#>  expr      min       lq     mean   median       uq      max neval
#>   rfn 21.14317 21.71995 23.63203 22.14149 25.13629 66.05444   100
#>   cpp 16.83316 17.49718 17.96451 17.71877 18.07946 21.92761   100


# Test in which each element is the same length as the input string.
test_vect <- stringi::stri_rand_strings(100000, 4)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect)
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq        max
#>   rfn 1872.35949 1923.98045 1954.61521 1950.37990 1981.90863 2053.52103
#>   cpp   25.88975   26.64083   27.32908   27.03378   27.44653   36.18348
#>  neval
#>    100
#>    100


# Test in which each element is an anagram of the input string.
test_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect)
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq        max
#>   rfn 1538.40960 1581.33320 1603.89282 1601.65069 1625.43529 1681.48081
#>   cpp   23.82749   24.60166   25.09615   24.89766   25.37767   28.47656
#>  neval
#>    100
#>    100

# Test in which each element is a string with length between two and six chars.
test_vect <- stringi::stri_rand_strings(100000, 2:6)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect)
)
#> Unit: milliseconds
#>  expr       min        lq     mean    median        uq      max neval
#>   rfn 398.26793 452.02667 456.9868 457.14238 462.38474 505.4922   100
#>   cpp  18.11612  18.73872  19.2822  19.20187  19.68848  22.0647   100
```
