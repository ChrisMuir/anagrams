// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// get_ana_logical
LogicalVector get_ana_logical(const StringVector& x_, const StringVector& terms, const bool& any_len, const bool& ignore_spaces, const bool& ignore_case);
RcppExport SEXP _anagrams_get_ana_logical(SEXP x_SEXP, SEXP termsSEXP, SEXP any_lenSEXP, SEXP ignore_spacesSEXP, SEXP ignore_caseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const StringVector& >::type x_(x_SEXP);
    Rcpp::traits::input_parameter< const StringVector& >::type terms(termsSEXP);
    Rcpp::traits::input_parameter< const bool& >::type any_len(any_lenSEXP);
    Rcpp::traits::input_parameter< const bool& >::type ignore_spaces(ignore_spacesSEXP);
    Rcpp::traits::input_parameter< const bool& >::type ignore_case(ignore_caseSEXP);
    rcpp_result_gen = Rcpp::wrap(get_ana_logical(x_, terms, any_len, ignore_spaces, ignore_case));
    return rcpp_result_gen;
END_RCPP
}
// get_ana_character
CharacterVector get_ana_character(const StringVector& x_, const StringVector& terms, const bool& any_len, const bool& ignore_spaces, const bool& ignore_case);
RcppExport SEXP _anagrams_get_ana_character(SEXP x_SEXP, SEXP termsSEXP, SEXP any_lenSEXP, SEXP ignore_spacesSEXP, SEXP ignore_caseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const StringVector& >::type x_(x_SEXP);
    Rcpp::traits::input_parameter< const StringVector& >::type terms(termsSEXP);
    Rcpp::traits::input_parameter< const bool& >::type any_len(any_lenSEXP);
    Rcpp::traits::input_parameter< const bool& >::type ignore_spaces(ignore_spacesSEXP);
    Rcpp::traits::input_parameter< const bool& >::type ignore_case(ignore_caseSEXP);
    rcpp_result_gen = Rcpp::wrap(get_ana_character(x_, terms, any_len, ignore_spaces, ignore_case));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_anagrams_get_ana_logical", (DL_FUNC) &_anagrams_get_ana_logical, 5},
    {"_anagrams_get_ana_character", (DL_FUNC) &_anagrams_get_ana_character, 5},
    {NULL, NULL, 0}
};

RcppExport void R_init_anagrams(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
