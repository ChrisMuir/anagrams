#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]


#ifndef _ANAGRAMS_H
#define _ANAGRAMS_H

#ifndef SINGLE_SPACE
std::string s = " ";
#define SINGLE_SPACE *s.c_str()
#endif

// Helper function for removing all spaces from a string. Used when arg
// "ignore_spaces" is TRUE.
void remove_spaces(std::string &x) {
  x.erase(std::remove(x.begin(), x.end(), SINGLE_SPACE), x.end());
}

#endif /* _ANAGRAMS_H */
