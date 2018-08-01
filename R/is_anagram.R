#' Check for Anagrams
#'
#' Given an input string and a character vector, check if string is an anagram
#' to each element of the character vector.
#'
#' @param string Char string.
#' @param terms Character vector.
#' @param value Logical, return anagram matches (TRUE), or logical vector
#'  (FALSE). Default value is FALSE.
#' @param any_len Logical, look for anagrams that are any length (TRUE), or
#'  restrict to same length anagrams (FALSE). Default value is FALSE.
#' @param ignore_spaces logical, ignores spaces. Default value is FALSE.
#' @param ignore_case logical, ignores case. Default value is FALSE.
#'
#' @return If \code{value} is FALSE, return logical vector with length equal
#'  to that of \code{terms}. Otherwise, return character vector of anagram matches.
#' @export
#'
#' @examples
#' str_to_check <- c("cats are great", "tacs", "frogs", "cats", "ts")
#' is_anagram("cats", str_to_check)
#' is_anagram("cats", str_to_check, value = TRUE)
#' is_anagram("cats", str_to_check, any_len = TRUE)
#' is_anagram("c a t s", str_to_check, ignore_spaces = TRUE)
#' is_anagram("CATs", str_to_check, ignore_case = TRUE)
#'
#' @useDynLib anagrams
#' @importFrom Rcpp sourceCpp
is_anagram <- function(string, terms, value = FALSE, any_len = FALSE,
                       ignore_spaces = FALSE, ignore_case = FALSE) {
  stopifnot(is.character(string) && length(string) == 1)
  stopifnot(is.character(terms))
  stopifnot(is.logical(value))
  stopifnot(is.logical(any_len))
  stopifnot(is.logical(ignore_spaces))
  stopifnot(is.logical(ignore_case))

  # If ignore_case is TRUE, make all input strings lower-case.
  if (ignore_case) {
    string <- tolower(string)
    terms <- tolower(terms)
  }

  if (value) return(get_ana_character(string, terms, any_len, ignore_spaces))
  get_ana_logical(string, terms, any_len, ignore_spaces)
}
