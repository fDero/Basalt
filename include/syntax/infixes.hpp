//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <set>
#include <map>

#include "syntax/keywords.hpp"

inline const std::string constexpr dot_member_access_operator   = ".";

inline const std::string constexpr math_infix_sum_operator      = "+";
inline const std::string constexpr math_infix_sub_operator      = "-";
inline const std::string constexpr math_infix_mul_operator      = "*";
inline const std::string constexpr math_infix_div_operator      = "/";
inline const std::string constexpr math_infix_mod_operator      = "%";

inline const std::string constexpr cmp_geq_operator             = ">=";
inline const std::string constexpr cmp_leq_operator             = "<=";
inline const std::string constexpr cmp_neq_operator             = "!=";
inline const std::string constexpr cmp_eq_operator              = "==";
inline const std::string constexpr cmp_lt_operator              = "<";
inline const std::string constexpr cmp_gt_operator              = ">";

inline const std::string constexpr boolean_and_operator         = "&&";
inline const std::string constexpr boolean_or_operator          = "||";
inline const std::string constexpr boolean_xor_operator         = "^^";

inline const std::set<std::string> infixies {
    math_infix_sum_operator,
    math_infix_sub_operator,
    math_infix_mul_operator,
    math_infix_div_operator,
    math_infix_mod_operator,
    cmp_geq_operator,
    cmp_leq_operator,
    cmp_neq_operator,
    cmp_lt_operator,
    cmp_gt_operator,
    cmp_eq_operator,
    boolean_and_operator,
    boolean_or_operator,
    boolean_xor_operator,
    dot_member_access_operator
};

inline const std::set<std::string> type_operators { 
    is_keyword, as_keyword 
};

inline const std::map<std::string, short> infix_operators_priority {
    { boolean_xor_operator,     1 }, 
    { boolean_and_operator,     1 }, 
    { boolean_or_operator,      1 }, 
    { cmp_lt_operator,          2 }, 
    { cmp_gt_operator,          2 }, 
    { cmp_leq_operator,         2 }, 
    { cmp_geq_operator,         2 }, 
    { cmp_eq_operator,          2 }, 
    { cmp_neq_operator,         2 }, 
    { math_infix_sum_operator,  3 }, 
    { math_infix_sub_operator,  3 }, 
    { math_infix_mul_operator,  4 }, 
    { math_infix_div_operator,  4 }, 
    { math_infix_mod_operator,  4 },
};