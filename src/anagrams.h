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

// Helper function for transforming all chars in a string to lower case. Used
// when arg "ignore_case" is TRUE.
// Reference: https://en.cppreference.com/w/cpp/string/byte/tolower
void to_lower(std::string &x) {
  std::transform(x.begin(), x.end(), x.begin(),
                 [](unsigned char c) {return std::tolower(c);}
  );
}

#endif /* _ANAGRAMS_H */
