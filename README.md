
<!-- README.md is generated from README.Rmd. Please edit that file -->
anagrams
========

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
#>  expr    min      lq      mean  median     uq      max neval
#>   rfn 57.550 61.7660 161.23924 65.2480 70.380 9285.696   100
#>   cpp 27.859 34.0905  39.21142 36.6565 39.222   76.612   100


# Test with long input vector, in which each element is shorter than the input string.
long_vect <- get_rand_str(n = 100000, str_len = 3)
microbenchmark(
  rfn = r_is_anagram("cats", long_vect), 
  cpp = is_anagram("cats", long_vect), 
  times = 20
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq      max neval
#>   rfn 120.93618 124.45039 128.02877 126.82588 130.18760 140.4584    20
#>   cpp  16.93434  17.82764  18.26786  18.31957  18.65882  19.6916    20


# Test with long input vector, in which each element is the same length as the input string.
long_vect <- get_rand_str(n = 100000, str_len = 4)
microbenchmark(
  rfn = r_is_anagram("cats", long_vect), 
  cpp = is_anagram("cats", long_vect), 
  times = 20
)
#> Unit: milliseconds
#>  expr        min        lq       mean     median         uq        max
#>   rfn 2961.01012 3012.8663 3038.16532 3036.05227 3063.79425 3125.83379
#>   cpp   30.24632   31.6935   32.06241   31.99738   32.74681   33.25138
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
#>  expr        min         lq       mean     median         uq        max
#>   rfn 2697.30326 2722.34406 2796.92552 2740.82748 2843.67374 3170.45586
#>   cpp   23.39166   24.20084   25.35031   24.80291   25.61246   34.37782
#>  neval
#>     20
#>     20
```
