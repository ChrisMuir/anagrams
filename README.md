
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
  str_counts <- vapply(string_spl, function(x) sum(string_spl == x), integer(1))
  terms_spl <- strsplit(terms, "", fixed = TRUE)
  
  out[terms_to_insp] <- vapply(terms_spl[terms_to_insp], function(x) {
    anagram <- TRUE
    for (char in string_spl) {
      if (str_counts[char] != sum(x == char)) {
        anagram <- FALSE
        break
      }
    }
    anagram
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
#>  expr       min        lq      mean    median       uq      max neval
#>   rfn 19.460292 20.441574 22.627882 21.445946 23.81209 64.69597   100
#>   cpp  7.406341  7.754939  8.516918  8.121682  8.68985 15.79909   100


# Test in which each element is the same length as the input string.
test_vect <- stringi::stri_rand_strings(100000, 4)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median        uq       max
#>   rfn 258.287917 292.260310 295.184739 297.383716 300.36531 309.66823
#>   cpp   8.841422   9.006374   9.255606   9.240972   9.39896  10.04117
#>  neval
#>     25
#>     25


# Test in which each element is an anagram of the input string.
test_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq       max neval
#>   rfn 516.84196 551.43567 572.57919 572.64042 587.10047 651.38112    25
#>   cpp  11.47112  11.82375  12.13163  12.00227  12.34573  13.21888    25


# Test in which each element is a string with length between two and six chars.
test_vect <- stringi::stri_rand_strings(100000, 2:6)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr      min        lq       mean     median         uq       max neval
#>   rfn 81.29563 97.253442 105.877772 101.809779 109.863098 156.34361    25
#>   cpp  8.33337  8.593262   9.068279   8.810265   9.404459  10.31793    25


# Test in which each element is a long string (nchar == 1000).
test_str <- stringi::stri_rand_strings(1, 1000)
test_vect <- stringi::stri_rand_strings(100000, 1000)
microbenchmark(
  rfn = r_is_anagram(test_str, test_vect), 
  cpp = is_anagram(test_str, test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq      mean     median         uq        max
#>   rfn 3519.91189 3648.74703 3745.9833 3661.02971 3754.12563 5180.80744
#>   cpp   72.40326   74.31926   76.2475   75.23493   76.10331   87.80353
#>  neval
#>     25
#>     25
```
