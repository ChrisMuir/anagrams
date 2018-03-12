// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// cpp_is_anagram
SEXP cpp_is_anagram(std::string x, StringVector terms, bool value, bool any_len);
RcppExport SEXP _anagrams_cpp_is_anagram(SEXP xSEXP, SEXP termsSEXP, SEXP valueSEXP, SEXP any_lenSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type x(xSEXP);
    Rcpp::traits::input_parameter< StringVector >::type terms(termsSEXP);
    Rcpp::traits::input_parameter< bool >::type value(valueSEXP);
    Rcpp::traits::input_parameter< bool >::type any_len(any_lenSEXP);
    rcpp_result_gen = Rcpp::wrap(cpp_is_anagram(x, terms, value, any_len));
    return rcpp_result_gen;
END_RCPP
}
// get_anagrams
LogicalVector get_anagrams(std::string x, StringVector terms, bool any_len);
RcppExport SEXP _anagrams_get_anagrams(SEXP xSEXP, SEXP termsSEXP, SEXP any_lenSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type x(xSEXP);
    Rcpp::traits::input_parameter< StringVector >::type terms(termsSEXP);
    Rcpp::traits::input_parameter< bool >::type any_len(any_lenSEXP);
    rcpp_result_gen = Rcpp::wrap(get_anagrams(x, terms, any_len));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_anagrams_cpp_is_anagram", (DL_FUNC) &_anagrams_cpp_is_anagram, 4},
    {"_anagrams_get_anagrams", (DL_FUNC) &_anagrams_get_anagrams, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_anagrams(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
