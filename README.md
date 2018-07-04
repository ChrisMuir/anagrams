
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
#>  expr       min        lq      mean    median        uq      max neval
#>   rfn 20.463208 20.735195 21.334605 20.923607 21.172685 33.20117   100
#>   s_d 20.514160 20.755172 21.573250 20.910227 21.252228 32.11578   100
#>   cpp  8.140196  8.291403  8.687152  8.403754  8.555142 18.82432   100


# Test in which each element is the same length as the input string.
test_vect <- stringi::stri_rand_strings(100000, 4)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr        min         lq      mean     median        uq       max neval
#>   rfn 241.213651 246.965709 267.92405 255.806402 291.48991 312.26139    25
#>   s_d  34.028857  34.398349  35.50593  34.807063  36.28650  39.03790    25
#>   cpp   9.749761   9.906282  10.31813   9.980327  10.24865  13.43625    25


# Test in which each element is an anagram of the input string.
test_vect <- rep("tacs", 100000)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq       max neval
#>   rfn 490.11700 509.35113 528.93433 525.66744 549.91245 576.90739    25
#>   s_d  23.61489  24.53349  24.83281  24.67168  25.14088  25.80289    25
#>   cpp  12.66391  12.87982  12.99958  13.01361  13.09682  13.37650    25


# Test in which each element is a string with length between two and six chars.
test_vect <- stringi::stri_rand_strings(100000, 2:6)
microbenchmark(
  rfn = r_is_anagram("cats", test_vect), 
  s_d = sd_is_anagram("cats", test_vect), 
  cpp = is_anagram("cats", test_vect), 
  times = 25
)
#> Unit: milliseconds
#>  expr       min        lq      mean    median        uq       max neval
#>   rfn 81.200719 83.455429 94.745049 88.068586 94.688657 151.15758    25
#>   s_d 23.416215 23.837392 24.140815 24.038633 24.621463  25.12292    25
#>   cpp  7.901933  8.376994  8.732499  8.595831  8.818333  12.93150    25


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
#>  expr        min         lq       mean     median        uq       max
#>   rfn 3683.46232 3799.81430 3872.10255 3814.59693 3875.9609 5207.4876
#>   s_d 1539.78183 1555.38079 1574.99575 1571.39872 1595.5558 1629.5216
#>   cpp   78.22865   79.70845   79.88178   79.92142   80.1388   80.9115
#>  neval
#>     25
#>     25
#>     25
```
