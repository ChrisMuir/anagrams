#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

LogicalVector get_anagrams(std::string &x, const StringVector &terms,
                           const bool &any_len);
