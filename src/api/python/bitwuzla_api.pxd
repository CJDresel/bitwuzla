###
# Bitwuzla: Satisfiability Modulo Theories (SMT) solver.
#
# Copyright (C) 2020 by the authors listed in the AUTHORS file at
# https://github.com/bitwuzla/bitwuzla/blob/main/AUTHORS
#
# This file is part of Bitwuzla under the MIT license. See COPYING for more
# information at https://github.com/bitwuzla/bitwuzla/blob/main/COPYING
##

from bitwuzla import BitwuzlaException
from cpython.ref cimport PyObject
from libc.stdint cimport uint8_t, uint32_t, uint64_t, int64_t
from libcpp cimport bool
from libcpp.optional cimport optional
from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "<functional>" namespace "std" nogil:
    cdef cppclass reference_wrapper[T]:
        pass

cdef extern from "<iostream>" namespace "std":
    cdef cppclass ostream:
        pass

cdef extern from "<sstream>" namespace "std":
    cdef cppclass stringstream(ostream):
        string to_string "str" () const
        stringstream &operator << (set_bv_format)


# Extract C++ exception message.
cdef extern from *:
    """
    #include <bitwuzla/cpp/bitwuzla.h>

    const char*
    get_err_msg()
    {
      try
      {
        throw;
      }
      catch (const bitwuzla::Exception& e)
      {
        return e.what();
      }
    }
    """
    cdef const char* get_err_msg()

cdef inline int raise_error() except *:
    raise BitwuzlaException(get_err_msg().decode().split("', ")[1])


# Terminator wrapper
cdef extern from "terminator.h":
    cdef cppclass PyTerminator:
        PyTerminator(PyObject* terminator)
        bool terminate()


# Bitwuzla C++ API
cdef extern from "bitwuzla/cpp/bitwuzla.h" namespace "bitwuzla":

    string copyright() except +raise_error
    string version() except +raise_error
    string git_id() except +raise_error

    cdef enum class Kind:
        pass

    cdef enum class Result:
        pass

    cdef enum class RoundingMode:
        pass

    cdef enum class Option:
        pass

    cdef cppclass set_bv_format:
        set_bv_format(uint8_t base) except +raise_error
        pass

    cdef cppclass Options:
        Options() except +raise_error
        bool is_bool(Option option) except +raise_error
        bool is_numeric(Option option) except +raise_error
        bool is_mode(Option option) except +raise_error
        const char *shrt(Option option) except +raise_error
        const char *lng(Option option) except +raise_error
        const char *description(Option option) except +raise_error
        vector[string] modes(Option option) except +raise_error
        Option option(const char *name) except +raise_error
        void set(Option option, uint64_t value) except +raise_error
        void set(Option option, const string &mode) except +raise_error
        #void set(const string &lng, const string &value) except +raise_error
        void set(const vector[string] &args) except +raise_error
        uint64_t get(Option option) except +raise_error
        const string &get_mode(Option option) except +raise_error

    cdef cppclass Sort:
        Sort() except +raise_error
        bool is_null() except +raise_error
        uint64_t id() except +raise_error
        uint64_t bv_size() except +raise_error
        uint64_t fp_exp_size() except +raise_error
        uint64_t fp_sig_size() except +raise_error
        Sort array_index() except +raise_error
        Sort array_element() except +raise_error
        vector[Sort] fun_domain() except +raise_error
        Sort fun_codomain() except +raise_error
        size_t fun_arity() except +raise_error
        optional[string] uninterpreted_symbol() except +raise_error
        bool is_array() except +raise_error
        bool is_bool() except +raise_error
        bool is_bv() except +raise_error
        bool is_fp() except +raise_error
        bool is_fun() except +raise_error
        bool is_rm() except +raise_error
        bool is_uninterpreted() except +raise_error
        string str() except +raise_error
        bool operator==(const Sort&) except +raise_error

    cdef cppclass Term:
        Term() except +raise_error
        bool is_null() except +raise_error
        uint64_t id() except +raise_error
        Kind kind() except +raise_error
        Sort sort() except +raise_error
        size_t num_children() except +raise_error
        vector[Term] children() except +raise_error
        Term operator[](size_t index) except +raise_error
        size_t num_indices() except +raise_error
        vector[uint64_t] indices() except +raise_error
        optional[reference_wrapper[const string]] symbol() except +raise_error
        bool is_const() except +raise_error
        bool is_variable() except +raise_error
        bool is_value() except +raise_error
        bool is_true() except +raise_error
        bool is_false() except +raise_error
        bool is_bv_value_zero() except +raise_error
        bool is_bv_value_one() except +raise_error
        bool is_bv_value_ones() except +raise_error
        bool is_bv_value_min_signed() except +raise_error
        bool is_bv_value_max_signed() except +raise_error
        bool is_fp_value_pos_zero() except +raise_error
        bool is_fp_value_neg_zero() except +raise_error
        bool is_fp_value_pos_inf() except +raise_error
        bool is_fp_value_neg_inf() except +raise_error
        bool is_fp_value_nan() except +raise_error
        bool is_rm_value_rna() except +raise_error
        bool is_rm_value_rne() except +raise_error
        bool is_rm_value_rtn() except +raise_error
        bool is_rm_value_rtp() except +raise_error
        bool is_rm_value_rtz() except +raise_error
        string str(uint8_t base) except +raise_error
        bool value[bool](uint8_t base) except +raise_error
        RoundingMode value[RoundingMode](uint8_t base) except +raise_error
        string value[string](uint8_t base) except +raise_error
        bool operator==(const Term&) except +raise_error

    cdef cppclass Terminator:
        pass

    cdef cppclass Bitwuzla:
        Bitwuzla(const Options &options);
        void configure_terminator(Terminator *terminator) except +raise_error
        void push(uint32_t nlevels) except +raise_error
        void pop(uint32_t nlevels) except +raise_error
        void assert_formula(const Term &term) except +raise_error
        vector[Term] get_assertions() except +raise_error;
        bool is_unsat_assumption(const Term &term) except +raise_error
        vector[Term] get_unsat_assumptions() except +raise_error
        vector[Term] get_unsat_core() except +raise_error
        void simplify() except +raise_error
        Result check_sat(const vector[Term] &assumptions) except +raise_error
        Term get_value(const Term &term) except +raise_error
        void print_formula(ostream& outfile, string& fmt) except +raise_error


    Sort mk_array_sort(const Sort &index, const Sort &element) except +raise_error
    Sort mk_bool_sort() except +raise_error
    Sort mk_bv_sort(uint64_t size) except +raise_error
    Sort mk_fp_sort(uint64_t exp_size, uint64_t sig_size) except +raise_error
    Sort mk_fun_sort(const vector[Sort] &domain, const Sort &codomain) except +raise_error
    Sort mk_rm_sort() except +raise_error
    Sort mk_uninterpreted_sort(optional[const string] symbol) except +raise_error


    Term mk_true() except +raise_error
    Term mk_false() except +raise_error
    Term mk_bv_zero(const Sort &sort) except +raise_error
    Term mk_bv_one(const Sort &sort) except +raise_error
    Term mk_bv_ones(const Sort &sort) except +raise_error
    Term mk_bv_min_signed(const Sort &sort) except +raise_error
    Term mk_bv_max_signed(const Sort &sort) except +raise_error
    Term mk_bv_value(const Sort &sort, const string &value, uint8_t base) except +raise_error
    Term mk_fp_pos_zero(const Sort &sort) except +raise_error
    Term mk_fp_neg_zero(const Sort &sort) except +raise_error
    Term mk_fp_pos_inf(const Sort &sort) except +raise_error
    Term mk_fp_neg_inf(const Sort &sort) except +raise_error
    Term mk_fp_nan(const Sort &sort) except +raise_error
    Term mk_fp_value(const Term &bv_sign,
                     const Term &bv_exponent,
                     const Term &bv_significand) except +raise_error

    Term mk_fp_value(const Sort &sort, const Term &rm, const string &real) except +raise_error
    Term mk_fp_value(const Sort &sort,
                     const Term &rm,
                     const string &num,
                     const string &den) except +raise_error
    Term mk_const_array(const Sort &sort, const Term &term) except +raise_error
    Term mk_rm_value(RoundingMode rm) except +raise_error
    Term mk_term(Kind kind,
                 const vector[Term] &args,
                 const vector[uint64_t] &indices) except +raise_error
    Term mk_const(const Sort &sort,
                  optional[const string] symbol) except +raise_error
    Term mk_var(const Sort &sort,
                optional[const string] symbol) except +raise_error

# -------------------------------------------------------------------------- #
