
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
#>  expr       min        lq      mean   median        uq      max neval
#>   rfn 16.538609 17.891446 19.224693 18.63669 20.002143 29.05610   100
#>   s_d 15.130036 17.484508 18.667232 18.28911 19.576342 26.84238   100
#>   cpp  2.593297  3.010695  3.734578  3.31950  4.145136 13.20300   100


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
#>   rfn 281.848907 303.684006 344.425906 353.860277 371.907531 411.656022
#>   s_d  32.368039  34.535709  42.925822  38.206274  51.425549  74.358380
#>   cpp   4.234619   4.931136   5.993128   5.760558   6.843008   9.744149
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
#>  expr        min         lq       mean     median        uq       max
#>   rfn 551.626521 607.516866 642.253184 649.206344 679.97978 711.87194
#>   s_d  20.919277  22.293526  30.913094  31.678190  39.55025  41.78014
#>   cpp   6.224328   6.516704   8.410436   8.138024  10.00268  12.27380
#>  neval
#>     25
#>     25
#>     25


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
#>   rfn 90.851283 107.419543 131.436974 128.471831 160.548210 174.905508
#>   s_d 20.987079  31.248456  33.732528  36.096213  37.996611  40.057380
#>   cpp  3.250264   4.034849   4.969135   5.112386   5.728403   7.896778
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
#>  expr        min         lq       mean    median         uq       max
#>   rfn 4958.78117 5167.57418 5394.27186 5248.1990 5491.97195 6796.9682
#>   s_d 2037.97565 2073.10818 2109.64264 2100.8282 2138.02972 2245.1922
#>   cpp   70.70364   74.06483   78.72708   76.2287   82.05243  104.2459
#>  neval
#>     25
#>     25
#>     25
```
