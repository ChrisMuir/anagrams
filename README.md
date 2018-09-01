
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
  anagrams_pkg = is_anagram("cats", test_vect), 
  stringdist_pkg = sd_is_anagram("cats", test_vect), 
  base_r = r_is_anagram("cats", test_vect)
) -> mb

print(mb)
#> Unit: milliseconds
#>            expr       min        lq      mean    median        uq       max neval
#>    anagrams_pkg  3.100996  3.468451  3.868257  3.723464  4.158105  5.544518   100
#>  stringdist_pkg 15.018863 15.784736 16.651755 16.271574 16.939462 24.624208   100
#>          base_r 15.097497 15.615902 16.911802 16.227311 16.908727 31.820793   100

autoplot.microbenchmark(mb)
```

![](README_files/figure-markdown_github/unnamed-chunk-7-1.png)

``` r


# Test in which each element is the same length as the input string.

test_vect <- stringi::stri_rand_strings(100000, 4)
microbenchmark(
  anagrams_pkg = is_anagram("cats", test_vect), 
  stringdist_pkg = sd_is_anagram("cats", test_vect), 
  base_r = r_is_anagram("cats", test_vect), 
  times = 25
) -> mb

print(mb)
#> Unit: milliseconds
#>            expr       min         lq       mean     median         uq        max neval
#>    anagrams_pkg   4.14881   5.391084   7.021752   6.741645   9.161284   9.275218    25
#>  stringdist_pkg  30.69577  31.381148  39.860443  39.009806  46.984034  57.092259    25
#>          base_r 237.90480 255.272808 290.234094 274.866043 327.326344 356.681086    25

autoplot.microbenchmark(mb)
```

![](README_files/figure-markdown_github/unnamed-chunk-7-2.png)

``` r


# Test in which each element is an anagram of the input string.

test_vect <- rep("tacs", 100000)
microbenchmark(
  anagrams_pkg = is_anagram("cats", test_vect), 
  stringdist_pkg = sd_is_anagram("cats", test_vect), 
  base_r = r_is_anagram("cats", test_vect), 
  times = 25
) -> mb

print(mb)
#> Unit: milliseconds
#>            expr        min         lq      mean     median        uq       max neval
#>    anagrams_pkg   6.031186   6.108313   8.51613   6.577749  11.81450  13.53926    25
#>  stringdist_pkg  20.083868  20.521320  28.48446  23.701445  39.59214  41.03456    25
#>          base_r 508.361398 545.118752 594.11975 608.963430 625.35459 689.77246    25

autoplot.microbenchmark(mb)
```

![](README_files/figure-markdown_github/unnamed-chunk-7-3.png)

``` r


# Test in which each element is a string with length between two and six chars.

test_vect <- stringi::stri_rand_strings(100000, 2:6)
microbenchmark(
  anagrams_pkg = is_anagram("cats", test_vect), 
  stringdist_pkg = sd_is_anagram("cats", test_vect), 
  base_r = r_is_anagram("cats", test_vect), 
  times = 25
) -> mb

print(mb)
#> Unit: milliseconds
#>            expr       min        lq       mean     median         uq        max neval
#>    anagrams_pkg  3.015511  3.975234   5.301403   5.170354   6.839706   7.652389    25
#>  stringdist_pkg 19.461361 21.731648  30.679704  31.180937  36.314394  40.047197    25
#>          base_r 82.909623 93.059026 122.541916 125.979663 146.103838 188.736725    25

autoplot.microbenchmark(mb)
```

![](README_files/figure-markdown_github/unnamed-chunk-7-4.png)

``` r


# Test in which each element is a long string (nchar == 1000).

test_str <- stringi::stri_rand_strings(1, 1000)
test_vect <- stringi::stri_rand_strings(100000, 1000)
microbenchmark(
  anagrams_pkg = is_anagram(test_str, test_vect), 
  stringdist_pkg = sd_is_anagram(test_str, test_vect), 
  base_r = r_is_anagram(test_str, test_vect), 
  times = 25
) -> mb

print(mb)
#> Unit: milliseconds
#>            expr        min         lq       mean     median        uq        max neval
#>    anagrams_pkg   69.80629   71.13075   77.00845   74.26053   79.6308   95.19816    25
#>  stringdist_pkg 2082.33845 2105.74981 2118.05681 2113.41687 2129.8930 2155.05893    25
#>          base_r 5233.31362 5304.26535 5406.09315 5360.12743 5406.0963 6732.25244    25

autoplot.microbenchmark(mb)
```

![](README_files/figure-markdown_github/unnamed-chunk-7-5.png)
