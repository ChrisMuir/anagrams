#include <Rcpp.h>
using namespace Rcpp;


// Check for anagram sub-strings.
LogicalVector cpp_is_anagram_any_len(std::string x,
                                     std::vector<std::string> terms) {
  int terms_len = terms.size();
  int x_len = x.size();
  LogicalVector out(terms_len);
  std::string::iterator x_begin = x.begin();
  std::string::iterator x_end = x.end();

  // Iterate over elements of "terms".
  for(int i = 0; i < terms_len; ++i) {
    std::string terms_iter = terms[i];

    // If terms_iter == x, append "true" and move on to the next iteration.
    if(terms_iter == x) {
      out[i] = true;
      continue;
    }

    // If length of terms_iter is less than the length of x, append "false"
    // and move on to the next iteration.
    int terms_iter_len = terms_iter.size();
    if(terms_iter_len < x_len) {
      out[i] = false;
      continue;
    }

    // For each char in x, look to see if it's present in terms_iter. Each time
    // a char is found in terms_iter, remove that char from terms_iter. Append
    // FALSE as soon as a char is not found. If all chars are found, append
    // TRUE.
    std::string::iterator x_char;
    bool anagram = true;
    for(x_char = x_begin; x_char != x_end; ++x_char) {
      int char_match = terms_iter.find_first_of(*x_char);
      if(char_match != -1) {
        terms_iter.erase(char_match, 1);
      } else {
        anagram = false;
        break;
      }
    }
    out[i] = anagram;
  }
  return out;
}


// Check for anagrams that are the same length as input arg x.
LogicalVector cpp_is_anagram_same_len(std::string x,
                                      std::vector<std::string> terms) {
  int terms_len = terms.size();
  int x_len = x.size();
  std::string x_original = x;
  LogicalVector out(terms_len);

  // Sort chars in x.
  std::sort(x.begin(), x.end());

  // For each element of arg "terms", if nchar is equal to term_len, then get
  // sorted version and compare that to the sorted version of x.
  for(int i = 0; i < terms_len; ++i) {
    std::string terms_iter = terms[i];
    int terms_iter_len = terms_iter.size();

    // If terms_iter == x_original, append "true" and move on to the next
    // iteration.
    if(terms_iter == x_original) {
      out[i] = true;
      continue;
    }

    // If length of terms_iter is not equal to the length of x, append "false"
    // and move on to the next iteration.
    if(terms_iter_len != x_len) {
      out[i] = false;
      continue;
    }

    std::sort(terms_iter.begin(), terms_iter.end());
    out[i] = x == terms_iter;
  }
  return out;
}


// Exported function, passes input args "X" and "terms" to one of the two
// anagram finding functions, depending on whether arg "any_len" is TRUE or
// FALSE.
//[[Rcpp::export]]
SEXP cpp_is_anagram(std::string x, std::vector<std::string> terms,
                             bool value, bool any_len) {
  // If "any_len" is true, return output from cpp_is_anagram_any_len,
  // otherwise return output from cpp_is_anagram_same_len.
  LogicalVector ana;
  if(any_len) {
    ana = cpp_is_anagram_any_len(x, terms);
  } else {
    ana = cpp_is_anagram_same_len(x, terms);
  }
  if(value) {
    CharacterVector out(sum(ana));
    int terms_len = terms.size();
    int counter = 0;
    for(int i = 0; i < terms_len; ++i) {
      if(ana[i]) {
        out[counter] = terms[i];
        counter += 1;
      }
    }
    return(out);
  } else {
    return(ana);
  }
}
