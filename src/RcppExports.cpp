// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// cpp_is_anagram
LogicalVector cpp_is_anagram(std::string x, std::vector<std::string> terms, bool any_len);
RcppExport SEXP _anagrams_cpp_is_anagram(SEXP xSEXP, SEXP termsSEXP, SEXP any_lenSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type x(xSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type terms(termsSEXP);
    Rcpp::traits::input_parameter< bool >::type any_len(any_lenSEXP);
    rcpp_result_gen = Rcpp::wrap(cpp_is_anagram(x, terms, any_len));
    return rcpp_result_gen;
END_RCPP
}
// cpp_is_anagram_any_len
LogicalVector cpp_is_anagram_any_len(std::string x, std::vector<std::string> terms);
RcppExport SEXP _anagrams_cpp_is_anagram_any_len(SEXP xSEXP, SEXP termsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type x(xSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type terms(termsSEXP);
    rcpp_result_gen = Rcpp::wrap(cpp_is_anagram_any_len(x, terms));
    return rcpp_result_gen;
END_RCPP
}
// cpp_is_anagram_same_len
LogicalVector cpp_is_anagram_same_len(std::string x, std::vector<std::string> terms);
RcppExport SEXP _anagrams_cpp_is_anagram_same_len(SEXP xSEXP, SEXP termsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type x(xSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type terms(termsSEXP);
    rcpp_result_gen = Rcpp::wrap(cpp_is_anagram_same_len(x, terms));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_anagrams_cpp_is_anagram", (DL_FUNC) &_anagrams_cpp_is_anagram, 3},
    {"_anagrams_cpp_is_anagram_any_len", (DL_FUNC) &_anagrams_cpp_is_anagram_any_len, 2},
    {"_anagrams_cpp_is_anagram_same_len", (DL_FUNC) &_anagrams_cpp_is_anagram_same_len, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_anagrams(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}