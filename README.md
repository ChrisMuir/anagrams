
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
is_anagram("cats", c("cats are great", "tacs", "frogs", "cats", "ts"))
#> [1] FALSE  TRUE FALSE  TRUE FALSE

# Set arg "any_len" to TRUE to test for anagrams that are any length (either same length or sub-string).
is_anagram("cats", c("cats are great", "tacs", "frogs", "cats", "ts"), any_len = TRUE)
#> [1]  TRUE  TRUE FALSE  TRUE FALSE

# Use arg "ignore_spaces" to make anagram searching insensitive to spaces.
is_anagram("c a t s", c("cats are great", "t acs", "frogs", "ca   ts", "ts"), ignore_spaces = TRUE)
#> [1] FALSE  TRUE FALSE  TRUE FALSE

# Use arg "ignore_case" to make anagram searching insensitive to lower/upper case.
is_anagram("CATs", c("catS are great", "tacs", "frogs", "CaTS", "ts"), ignore_case = TRUE)
#> [1] FALSE  TRUE FALSE  TRUE FALSE
```

Benchmarks
----------

Let's create a simple, pure R version of `is_anagram` that searches for same length anagrams, and compare speeds.

``` r
r_is_anagram <- function(string, terms) {
  # Get sorted version of arg "string".
  str_key <- paste(sort(
    unlist(strsplit(string, "", fixed = TRUE), FALSE, FALSE)
  ), collapse = "")
  
  # Get number of chars in arg "string".
  str_len <- nchar(string)
  
  # For each element of arg "terms", if nchar is equal to str_len, then get
  # sorted version and compare that to str_key.
  vapply(terms, function(x) {
    if (nchar(x) != str_len) {
      return(FALSE)
    }
    if (x == string) {
      return(TRUE)
    }
    paste(sort(
      unlist(strsplit(x, "", fixed = TRUE), FALSE, FALSE)
    ), collapse = "") == str_key
  }, logical(1), USE.NAMES = FALSE)
}

# Test to make sure its output is identical to that of pkg function is_anagram.
identical(
  r_is_anagram("cats", c("cats are great", "tacs", "frogs", "cats", "ts")), 
  is_anagram("cats", c("cats are great", "tacs", "frogs", "cats", "ts"))
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

# Test with short input vector.
microbenchmark(
  rfn = r_is_anagram("cats", c("cats are great", "tacs", "frogs", "cats", "ts")), 
  cpp = is_anagram("cats", c("cats are great", "tacs", "frogs", "cats", "ts"))
)
#> Unit: microseconds
#>  expr    min      lq      mean median     uq      max neval
#>   rfn 56.451 59.3840 142.20751 61.582 63.049 8032.428   100
#>   cpp 28.593 31.5245  34.42051 33.907 35.557   69.280   100


# Test with long input vector, in which each element is shorter than the input string.
long_vect <- get_rand_str(n = 100000, str_len = 3)
microbenchmark(
  rfn = r_is_anagram("cats", long_vect), 
  cpp = is_anagram("cats", long_vect), 
  times = 20
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq       max neval
#>   rfn 118.39152 119.74394 123.09335 124.03343 125.11386 126.71170    20
#>   cpp  17.06043  17.32161  17.62812  17.55364  17.86301  18.40607    20


# Test with long input vector, in which each element is the same length as the input string.
long_vect <- get_rand_str(n = 100000, str_len = 4)
microbenchmark(
  rfn = r_is_anagram("cats", long_vect), 
  cpp = is_anagram("cats", long_vect), 
  times = 20
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq        max
#>   rfn 3027.49969 3113.09950 3162.88341 3177.92727 3199.14192 3293.18012
#>   cpp   31.45341   31.91491   35.28051   34.00705   37.46444   43.16866
#>  neval
#>     20
#>     20


# Test with long input vector, in which each element is an anagram of the input string.
long_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", long_vect), 
  cpp = is_anagram("cats", long_vect), 
  times = 20
)
#> Unit: milliseconds
#>  expr       min         lq       mean    median         uq        max
#>   rfn 2669.1625 2699.32648 2774.64875 2781.4627 2843.27951 2964.43563
#>   cpp   24.3909   25.36136   26.29793   26.1502   26.98156   29.10119
#>  neval
#>     20
#>     20
```
