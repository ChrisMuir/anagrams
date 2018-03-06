#include <Rcpp.h>
using namespace Rcpp;


// Check for anagram sub-strings.
//[[Rcpp::export]]
LogicalVector cpp_is_anagram_any_len(std::string x,
                                     StringVector terms) {
  int terms_len = terms.size();
  int x_len = x.size();
  LogicalVector out(terms_len);
  std::string::iterator x_begin = x.begin();
  std::string::iterator x_end = x.end();

  StringVector::iterator terms_end = terms.end();
  StringVector::iterator terms_iter;
  int i = 0;

  // Iterate over elements of "terms".
  for(terms_iter = terms.begin(); terms_iter != terms_end; ++terms_iter) {
    std::string curr_term = as<std::string>(*terms_iter);

    // If terms_iter == x, append "true" and move on to the next iteration.
    if(curr_term == x) {
      out[i] = true;
      i++;
      continue;
    }

    // If length of terms_iter is less than the length of x, append "false"
    // and move on to the next iteration.
    int curr_term_len = curr_term.size();
    if(curr_term_len < x_len) {
      out[i] = false;
      i++;
      continue;
    }

    // For each char in x, look to see if it's present in terms_iter. Each time
    // a char is found in terms_iter, remove that char from terms_iter. Return
    // FALSE for this iteration as soon as a char is not found. If all chars
    // are found, return TRUE.
    std::string::iterator x_char;
    bool anagram = true;
    for(x_char = x_begin; x_char != x_end; ++x_char) {
      // Get index of first matching char value.
      int char_match = curr_term.find_first_of(*x_char);

      // If match was found, remove that index from terms_iter. Otherwise,
      // return FALSE for the current term and move on to the next term.
      if(char_match != -1) {
        curr_term.erase(char_match, 1);
      } else {
        anagram = false;
        break;
      }
    }

    out[i] = anagram;
    i++;
  }

  return out;
}


// Check for anagrams that are the same length as input arg x.
//[[Rcpp::export]]
LogicalVector cpp_is_anagram_same_len(std::string x,
                                      StringVector terms) {
  int terms_len = terms.size();
  int x_len = x.size();
  std::string x_original = x;
  LogicalVector out(terms_len);

  // Sort chars in x.
  std::sort(x.begin(), x.end());

  StringVector::iterator terms_end = terms.end();
  StringVector::iterator terms_iter;
  int i = 0;

  // For each element of arg "terms", if nchar is equal to x_len, then get
  // sorted version and compare that to the sorted version of x.
  for(terms_iter = terms.begin(); terms_iter != terms_end; ++terms_iter) {
    std::string curr_term = as<std::string>(*terms_iter);
    int curr_term_len = curr_term.size();

    // If length of terms_iter is not equal to the length of x, return FALSE
    // and move on to the next iteration.
    if(curr_term_len != x_len) {
      out[i] = false;
      i++;
      continue;
    }

    // If terms_iter == x_original, return TRUE and move on to the next
    // iteration.
    if(curr_term == x_original) {
      out[i] = true;
      i++;
      continue;
    }

    // sort chars of terms_iter, and compare it to the sorted version of x.
    std::sort(curr_term.begin(), curr_term.end());
    out[i] = x == curr_term;
    i++;
  }

  return out;
}


// Main function, passes input args "X" and "terms" to one of the two
// anagram finding functions, depending on whether arg "any_len" is TRUE or
// FALSE. If arg "value" is TRUE, returns a character vector of matching
// anagrams, otherwise returns a logical vector indicating which elements of
// "terms" are anagrams to "x".
//[[Rcpp::export]]
SEXP cpp_is_anagram(std::string x, StringVector terms,
                    bool value, bool any_len) {
  // If any_len is true, get result from cpp_is_anagram_any_len,
  // otherwise get result from cpp_is_anagram_same_len.
  LogicalVector ana;
  if(any_len) {
    ana = cpp_is_anagram_any_len(x, terms);
  } else {
    ana = cpp_is_anagram_same_len(x, terms);
  }

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
