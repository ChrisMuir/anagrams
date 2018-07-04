#include <Rcpp.h>
#include"anagrams.h"

using namespace Rcpp;


// If arg "value" is TRUE, cpp_is_anagram() returns a character vector of
// matching anagrams, otherwise returns a logical vector indicating which
// elements of "terms" are anagrams to "x".
//[[Rcpp::export]]
SEXP cpp_is_anagram(std::string &x, const StringVector &terms,
                    const bool &value, const bool &any_len) {

  // Get logical vector indicating which elements of terms are anagrams of x.
  LogicalVector ana = get_anagrams(x, terms, any_len);

  // If value is TRUE, subset terms by ana, and return subset as a char vector.
  // Otherwise, return ana as a logical vector.
  if(value) {
    CharacterVector out(sum(ana));
    LogicalVector::iterator ana_end = ana.end();
    LogicalVector::iterator ana_iter;
    int out_i = 0;
    int ana_i = 0;
    // For each element of ana, if it's TRUE, append the element from terms of
    // the current iteration to the output char vector.
    for(ana_iter = ana.begin(); ana_iter != ana_end; ++ana_iter) {
      if(*ana_iter) {
        out[out_i] = terms[ana_i];
        out_i++;
      }
      ana_i++;
    }
    return(out);
  } else {
    return(ana);
  }
}


// Look for anagrams of x within terms. Use arg any_len to choose between
// looking for anagram sub-strings within terms (TRUE), or same length
// anagrams only (FALSE).
LogicalVector get_anagrams(std::string &x, const StringVector &terms,
                           const bool &any_len) {
  // Initialize variables.
  int terms_len = terms.size();
  int x_len = x.size();
  LogicalVector out(terms_len);
  std::string::iterator x_begin = x.begin();
  std::string::iterator x_end = x.end();

  // create unordered_map containing the unique chars in x, and the count for
  // each.
  std::string::iterator x_char;
  std::unordered_map<char, int> x_counts;
  for(x_char = x_begin; x_char != x_end; ++x_char) {
    x_counts[*x_char]++;
  }

  // Iterate over the strings in terms.
  for(int i = 0; i < terms_len; ++i) {
    // If terms[i] is NA, append false to output and move to the next term.
    if(StringVector::is_na(terms[i])) {
      out[i] = false;
      continue;
    }

    // Cast terms[i] as an std::string.
    std::string curr_term = as<std::string>(terms[i]);

    // Compare size of curr_term to that of x.
    int curr_term_len = curr_term.size();
    if(any_len) {
      // If any_len == TRUE and length of curr_term is less than the length of
      // x, append FALSE to the output and move on to the next string in terms.
      if(curr_term_len < x_len) {
        out[i] = false;
        continue;
      }
    } else {
      // If any_len == FALSE and length of terms_iter is not equal to the
      // length of x, return FALSE and move on to the next iteration.
      if(curr_term_len != x_len) {
        out[i] = false;
        continue;
      }
    }

    // If curr_term == x, append "true" to the output and move on to the next
    // string in terms.
    if(curr_term == x) {
      out[i] = true;
      continue;
    }

    // initialize iterators to be used in the loop below.
    std::string::iterator curr_term_begin = curr_term.begin();
    std::string::iterator curr_term_end = curr_term.end();
    std::string::iterator x_char;
    bool anagram = true;

    // Loop over the chars in x. For each, compare the number of times it
    // appears in x to the number of times it appears in curr_term.
    if(any_len) {
      // If any_len == TRUE, if x_char appears in curr_term fewer times than
      // it appears in x, then we know curr_terms is not an any-length anagram
      // of x, append FALSE to the output and move on the next string in terms.
      for(x_char = x_begin; x_char != x_end; ++x_char) {
        if(std::count(curr_term_begin, curr_term_end, *x_char) < x_counts[*x_char]) {
          anagram = false;
          break;
        }
      }
    } else {
      // If any_len == FALSE, if the count of x_char in curr_term and x is not
      // equal, then we know curr_terms is not a same-length anagram of x,
      // append FALSE to the output and move on the next string in terms.
      for(x_char = x_begin; x_char != x_end; ++x_char) {
        if(std::count(curr_term_begin, curr_term_end, *x_char) != x_counts[*x_char]) {
          anagram = false;
          break;
        }
      }
    }

    out[i] = anagram;
  }

  return(out);
}
