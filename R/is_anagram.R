#' Check for Anagrams
#'
#' Given an input string and a character vector, for each element of the
#' character vector, check if input string is an anagram of the given element.
#'
#' @param string Char string.
#' @param terms Character vector.
#' @param any_len Logical, should the function be looking for anagrams that
#'  are any length, or be restricted to anagrams that are the same length as
#'  input arg \code{string}. Default value is FALSE.
#' @param ignore_spaces, logical, should spaces within input arg \code{string}
#'  be ignored when searching for anagrams. Default value is FALSE.
#'
#' @return Logical vector, with length equal to that of \code{terms}.
#' @export
#'
#' @examples
#' str_to_check <- c("cats are great", "tacs", "frogs", "cats", "ts")
#' is_anagram("cats", str_to_check)
#' is_anagram("cats", str_to_check, any_len = TRUE)
#' is_anagram("c a t s", str_to_check, ignore_spaces = TRUE)
#'
#' @useDynLib anagrams
#' @importFrom Rcpp sourceCpp
is_anagram <- function(string, terms, any_len = FALSE, ignore_spaces = FALSE) {
  stopifnot(is.character(string) && length(string) == 1)
  stopifnot(is.character(terms))
  stopifnot(is.logical(any_len))

  # If ignore_spaces is TRUE, remove all spaces in both string and terms.
  if (ignore_spaces) {
    string <- gsub(" ", "", string, fixed = TRUE)
    terms <- gsub(" ", "", terms, fixed = TRUE)
  }

  .Call('_anagrams_cpp_is_anagram', PACKAGE = 'anagrams', string, terms,
        any_len)
}
