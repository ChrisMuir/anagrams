#include <Rcpp.h>
using namespace Rcpp;
#include "anagrams.h"


// Look for anagrams of x within terms, return a logical vector the same
// length as terms. Use arg any_len to choose between looking for anagram
// sub-strings within terms (TRUE), or same length anagrams only (FALSE).
// [[Rcpp::export]]
LogicalVector get_ana_logical(std::string &x,
                              const StringVector &terms,
                              const bool &any_len,
                              const bool &ignore_spaces) {

  // If ignore_spaces is true, remove spaces from x.
  if(ignore_spaces) {
    remove_spaces(x);
  }

  // Initialize variables.
  int terms_len = terms.size();
  int x_len = x.size();
  LogicalVector out(terms_len);
  std::string::iterator x_begin = x.begin();
  std::string::iterator x_end = x.end();

  // Create unordered_map containing the unique chars in x, and the count for
  // each.
  std::string::iterator x_char;
  std::unordered_map<char, int> x_counts;
  x_counts.reserve(x_len);
  for(x_char = x_begin; x_char != x_end; ++x_char) {
    x_counts[*x_char]++;
  }

  // Initialize variables to be used in the main loop.
  std::string curr_term;
  int curr_term_len;
  bool anagram;
  std::string::iterator curr_term_begin;
  std::string::iterator curr_term_end;

  // Iterate over the strings in terms.
  for(int i = 0; i < terms_len; ++i) {
    // If terms[i] is NA, append false to output and move to the next term.
    if(StringVector::is_na(terms[i])) {
      //out[i] = false;
      continue;
    }

    // Cast terms[i] as an std::string.
    curr_term = as<std::string>(terms[i]);

    // If ignore_spaces is true, remove spaces from curr_term.
    if(ignore_spaces) {
      remove_spaces(curr_term);
    }

    // Compare size of curr_term to that of x.
    curr_term_len = curr_term.size();
    if(any_len) {
      // If any_len == TRUE and length of curr_term is less than the length of
      // x, append FALSE to the output and move on to the next string in terms.
      if(curr_term_len < x_len) {
        //out[i] = false;
        continue;
      }
    } else {
      // If any_len == FALSE and length of terms_iter is not equal to the
      // length of x, return FALSE and move on to the next iteration.
      if(curr_term_len != x_len) {
        //out[i] = false;
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
    curr_term_begin = curr_term.begin();
    curr_term_end = curr_term.end();
    anagram = true;

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


// Look for anagrams of x within terms, return a char vector containing all of
// the anagram strings. Use arg any_len to choose between looking for anagram
// sub-strings within terms (TRUE), or same length anagrams only (FALSE).
// [[Rcpp::export]]
CharacterVector get_ana_character(std::string &x,
                                  const StringVector &terms,
                                  const bool &any_len,
                                  const bool &ignore_spaces) {

  // If ignore_spaces is true, remove spaces from x.
  if(ignore_spaces) {
    remove_spaces(x);
  }

  // Initialize variables.
  int terms_len = terms.size();
  int x_len = x.size();
  std::vector<std::string> out;
  std::string::iterator x_begin = x.begin();
  std::string::iterator x_end = x.end();

  // Create unordered_map containing the unique chars in x, and the count for
  // each.
  std::string::iterator x_char;
  std::unordered_map<char, int> x_counts;
  x_counts.reserve(x_len);
  for(x_char = x_begin; x_char != x_end; ++x_char) {
    x_counts[*x_char]++;
  }

  // Initialize variables to be used in the main loop.
  std::string curr_term;
  int curr_term_len;
  bool anagram;
  std::string::iterator curr_term_begin;
  std::string::iterator curr_term_end;

  // Iterate over the strings in terms.
  for(int i = 0; i < terms_len; ++i) {
    // If terms[i] is NA, move to the next term.
    if(StringVector::is_na(terms[i])) {
      continue;
    }

    // Cast terms[i] as an std::string.
    curr_term = as<std::string>(terms[i]);

    // If ignore_spaces is true, remove spaces from curr_term.
    if(ignore_spaces) {
      remove_spaces(curr_term);
    }

    // Compare size of curr_term to that of x.
    curr_term_len = curr_term.size();
    if(any_len) {
      // If any_len == TRUE and length of curr_term is less than the length of
      // x, move on to the next string in terms.
      if(curr_term_len < x_len) {
        continue;
      }
    } else {
      // If any_len == FALSE and length of terms_iter is not equal to the
      // length of x, move on to the next iteration.
      if(curr_term_len != x_len) {
        continue;
      }
    }

    // If curr_term == x, add curr_term to output and move on to the next
    // string in terms.
    if(curr_term == x) {
      if(!ignore_spaces) {
        out.push_back(curr_term);
      } else {
        out.push_back(as<std::string>(terms[i]));
      }
      continue;
    }

    // initialize iterators to be used in the loop below.
    curr_term_begin = curr_term.begin();
    curr_term_end = curr_term.end();
    anagram = true;

    // Loop over the chars in x. For each, compare the number of times it
    // appears in x to the number of times it appears in curr_term.
    if(any_len) {
      // If any_len == TRUE, if x_char appears in curr_term fewer times than
      // it appears in x, then we know curr_terms is not an any-length anagram
      // of x, move on the next string in terms.
      for(x_char = x_begin; x_char != x_end; ++x_char) {
        if(std::count(curr_term_begin, curr_term_end, *x_char) < x_counts[*x_char]) {
          anagram = false;
          break;
        }
      }
    } else {
      // If any_len == FALSE, if the count of x_char in curr_term and x is not
      // equal, then we know curr_terms is not a same-length anagram of x,
      // move on the next string in terms.
      for(x_char = x_begin; x_char != x_end; ++x_char) {
        if(std::count(curr_term_begin, curr_term_end, *x_char) != x_counts[*x_char]) {
          anagram = false;
          break;
        }
      }
    }

    if(anagram) {
      if(!ignore_spaces) {
        out.push_back(curr_term);
      } else {
        out.push_back(as<std::string>(terms[i]));
      }
    }
  }

  return(wrap(out));
}
