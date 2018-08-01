
<!-- README.md is generated from README.Rmd. Please edit that file -->
anagrams
========

[![Travis-CI Build Status](https://travis-ci.org/ChrisMuir/anagrams.svg?branch=master)](https://travis-ci.org/ChrisMuir/anagrams)

`anagrams` is a super simple R package providing a function for comparing character vectors and determining if strings are [anagrams](https://en.wikipedia.org/wiki/Anagram) of one another. The project was inspired by [this](http://www.programmingr.com/content/simple-anagram-finder-using-r/) blog post on finding anagrams in R. The package uses C++ and [Rcpp](https://CRAN.R-project.org/package=Rcpp) for speed.

If you're interested in anagrams and R, be sure to check out [Romain Francois'](https://github.com/romainfrancois) package [anagram](https://github.com/romainfrancois/anagram) (and h/t to Romain for [pointing out](https://twitter.com/romain_francois/status/972754279164514304) some bottle-necks in my cpp code).

Also, thank you to [Mark van der Loo](https://github.com/markvanderloo) for pointing out that the [stringdist](https://github.com/markvanderloo/stringdist) package can be used to concisely determine same-length anagrams (and it's faster than the base R function I defined below!) I included the `stringdist` solution in the benchmarks below.

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

Let's create a simple, base R version of `is_anagram` that searches for same-length anagrams, and compare speeds.

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

And for completeness, here is a similar function that uses `stringdist::stringdist()` to find same-length anagrams.

``` r
library(stringdist)

sd_is_anagram <- function(string, terms) {
  out <- rep(FALSE, length(terms))
  terms_to_insp <- which(nchar(terms) == nchar(string))
  if (length(terms_to_insp) == 0) {
    return(out)
  }
  
  out[terms_to_insp] <- stringdist(string, terms[terms_to_insp], method="qgram", q=1) == 0
  
  out
}

# Test to make sure its output is identical to that of pkg function is_anagram.
identical(
  sd_is_anagram("stac", c("cats are great", "tacs", "frogs", "cats", "ts")), 
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
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect)
)
#> Unit: milliseconds
#>  expr       min        lq     mean    median        uq       max neval
#>   rfn 15.523595 16.225164 17.23116 16.689078 17.964045 23.986981   100
#>   s_d 15.214702 16.336869 17.63126 16.826903 17.847623 27.623193   100
#>   cpp  2.903553  3.351449  3.73067  3.584815  3.978435  5.320013   100


# Test in which each element is the same length as the input string.
test_vect <- stringi::stri_rand_strings(100000, 4)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq        max
#>   rfn 249.143841 264.177513 304.151393 319.738449 338.089919 377.248336
#>   s_d  30.062288  31.299391  39.704185  37.209156  47.762130  51.626224
#>   cpp   4.011165   4.444028   6.109578   5.432683   8.140196   8.383604
#>  neval
#>     25
#>     25
#>     25


# Test in which each element is an anagram of the input string.
test_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr       min         lq       mean     median        uq       max neval
#>   rfn 554.62012 583.376076 622.027291 632.720996 654.97842 712.27013    25
#>   s_d  20.40935  20.915622  29.273505  24.177871  37.53971  41.68178    25
#>   cpp   6.19830   6.375018   8.299201   6.579702  12.06081  12.45613    25


# Test in which each element is a string with length between two and six chars.
test_vect <- stringi::stri_rand_strings(100000, 2:6)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr       min         lq       mean     median         uq        max
#>   rfn 86.847427 113.286132 127.161862 122.461839 144.911105 217.461613
#>   s_d 20.001668  22.917895  30.476989  32.036125  37.662928  39.605131
#>   cpp  3.259805   3.854613   4.822341   4.869514   5.587162   6.307721
#>  neval
#>     25
#>     25
#>     25


# Test in which each element is a long string (nchar == 1000).
test_str <- stringi::stri_rand_strings(1, 1000)
test_vect <- stringi::stri_rand_strings(100000, 1000)
microbenchmark(
  rfn = r_is_anagram(test_str, test_vect), 
  s_d = sd_is_anagram(test_str, test_vect), 
  cpp = is_anagram(test_str, test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq       mean     median         uq       max
#>   rfn 5426.48767 5517.82890 5628.50316 5575.45672 5623.64851 7084.8195
#>   s_d 2059.51150 2070.77517 2087.54561 2083.78355 2097.36705 2155.9608
#>   cpp   66.45165   72.14306   79.90926   74.58059   88.21388  108.7775
#>  neval
#>     25
#>     25
#>     25
```
