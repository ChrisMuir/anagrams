
<!-- README.md is generated from README.Rmd. Please edit that file -->
anagrams
========

[![Travis-CI Build Status](https://travis-ci.org/ChrisMuir/anagrams.svg?branch=master)](https://travis-ci.org/ChrisMuir/anagrams)

`anagrams` is a super simple R package providing a function for comparing character vectors and determining if strings are [anagrams](https://en.wikipedia.org/wiki/Anagram) of one another. The project was inspired by [this](http://www.programmingr.com/content/simple-anagram-finder-using-r/) blog post on finding anagrams in R. The package uses C++ and [Rcpp](https://CRAN.R-project.org/package=Rcpp) for speed.

If you're interested in anagrams and R, be sure to check out [Romain Francois'](https://github.com/romainfrancois) package [anagram](https://github.com/romainfrancois/anagram) (and h/t to Romain for [pointing out](https://twitter.com/romain_francois/status/972754279164514304) some bottle-necks in my cpp code).

Installation
------------

Install from github with:

``` r
# install.packages("devtools")
devtools::install_github("ChrisMuir/anagrams")
```

Example Usage
-------------

The exported function `is_anagram` takes as input a string and a character vector, and looks for anagrams of the string in the character vector.

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
#>  expr      min        lq     mean    median        uq      max neval
#>   rfn 19.62964 20.736104 22.20028 21.583590 23.341243 29.31049   100
#>   cpp  7.50128  8.007499  9.13518  8.294149  8.898606 49.45918   100


# Test in which each element is the same length as the input string.
test_vect <- stringi::stri_rand_strings(100000, 4)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr         min          lq        mean     median          uq
#>   rfn 1866.858158 1923.963039 1944.140191 1945.30012 1967.639737
#>   cpp    9.036433    9.144934    9.357407    9.30952    9.438548
#>         max neval
#>  2028.33617    25
#>    10.25524    25


# Test in which each element is an anagram of the input string.
test_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq        max
#>   rfn 1525.65882 1544.55901 1571.40227 1570.26181 1584.94326 1636.31662
#>   cpp   11.49238   11.68079   11.92335   11.82779   12.07595   13.01764
#>  neval
#>     25
#>     25


# Test in which each element is a string with length between two and six chars.
test_vect <- stringi::stri_rand_strings(100000, 2:6)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq        max
#>   rfn 397.009166 435.781294 437.334173 440.554268 445.260529 453.118837
#>   cpp   7.952515   8.122233   8.355262   8.456901   8.536078   8.672439
#>  neval
#>     25
#>     25


# Test in which each element is a long string (nchar == 1000).
test_str <- stringi::stri_rand_strings(1, 1000)
test_vect <- stringi::stri_rand_strings(100000, 1000)
microbenchmark(
  rfn = r_is_anagram(test_str, test_vect), 
  cpp = is_anagram(test_str, test_vect), 
  times = 5, 
  unit = "s"
)
#> Unit: seconds
#>  expr          min           lq         mean       median           uq
#>   rfn 279.34702874 279.78411828 280.63786255 279.83182677 281.90004538
#>   cpp   0.07564841   0.07676605   0.07784931   0.07710438   0.07770884
#>           max neval
#>  282.32629355     5
#>    0.08201885     5
```
