#' Check for Anagrams
#'
#' Given an input string and a character vector, for each element of the
#' character vector, check if input string is an anagram of the given element.
#'
#' @param string Char string.
#' @param terms Character vector.
#' @param any_len Logical, should the function be looking for anagrams that
#'  are any length, or be restricted to anagrams that are the same length as
#'  input arg \code{string}.
#'
#' @return Logical vector, with length equal to that of \code{terms}.
#' @export
#'
#' @examples
#' str_to_check <- c("cats are great", "tacs", "frogs", "cats", "ts")
#' is_anagram("cats", str_to_check)
#' is_anagram("cats", str_to_check, any_len = TRUE)
#'
#' @useDynLib anagrams
#' @importFrom Rcpp sourceCpp
is_anagram <- function(string, terms, any_len = FALSE) {
  stopifnot(is.character(string) && length(string) == 1)
  stopifnot(is.character(terms))
  stopifnot(is.logical(any_len))
  .Call('_anagrams_cpp_is_anagram', PACKAGE = 'anagrams', string, terms,
        any_len)
}
