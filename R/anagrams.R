#' @title Find Anagrams Among Character Vectors
#'
#' @description Simple package for testing whether anagrams exist among
#'  input strings. Vectorized function built in C++ using Rcpp.
#'
#' @section \code{anagrams} features the following function:
#' \itemize{
#'   \item \code{\link{is_anagram}}
#' }
#'
#' @section Development links:
#'
#' \itemize{
#'   \item \url{https://github.com/ChrisMuir/anagrams}
#'   \item Report bugs at \url{https://github.com/ChrisMuir/anagrams/issues}
#' }
#'
#' @useDynLib anagrams
#' @importFrom Rcpp sourceCpp
#' @docType package
#' @name anagrams
NULL
