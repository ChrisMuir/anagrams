#include <Rcpp.h>
using namespace Rcpp;
LogicalVector cpp_is_anagram(std::string x, std::vector<std::string> terms, bool any_len);
LogicalVector cpp_is_anagram_any_len(std::string x, std::vector<std::string> terms);
LogicalVector cpp_is_anagram_same_len(std::string x, std::vector<std::string> terms);
