#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

LogicalVector get_anagrams(std::string x, StringVector terms, bool any_len);
SEXP cpp_is_anagram(std::string x, StringVector terms, bool value, bool any_len);
