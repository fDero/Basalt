//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <set>
#include <map>

#include "syntax/symbols.hpp"
#include "syntax/keywords.hpp"
#include "syntax/specials.hpp"

inline const std::string constexpr pointer_type_symbol          = "#";
inline const std::string constexpr slice_type_symbol            = "$";
inline const std::string square_brackets_access                 = "[square-brackets-access]";

inline const std::string constexpr char_delim                   = "'";
inline const std::string constexpr string_delim                 = "\"";
inline const std::string constexpr string_delim2                = "`";

inline const std::string constexpr address_operator             = "&";
inline const std::string constexpr boolean_not_operator         = "!";
inline const std::string constexpr pointer_dereference_operator = "#";
inline const std::string constexpr dot_member_access_operator   = ".";
inline const std::string constexpr prefix_minus_sign            = "-";
inline const std::string constexpr prefix_plus_sign             = "+";

inline const std::string constexpr math_infix_sum_operator      = "+";
inline const std::string constexpr math_infix_sub_operator      = "-";
inline const std::string constexpr math_infix_mul_operator      = "*";
inline const std::string constexpr math_infix_div_operator      = "/";
inline const std::string constexpr math_infix_mod_operator      = "%";

inline const std::string constexpr cmp_geq_operator             = ">=";
inline const std::string constexpr cmp_leq_operator             = "<=";
inline const std::string constexpr cmp_neq_operator             = "!=";
inline const std::string constexpr cmp_eq_operator              = "==";
inline const std::string constexpr cmp_le_operator              = "<";
inline const std::string constexpr cmp_ge_operator              = ">";

inline const std::string constexpr boolean_and_operator         = "&&";
inline const std::string constexpr boolean_or_operator          = "||";
inline const std::string constexpr boolean_xor_operator         = "^^";

inline const std::set<std::string> combinations{
    cmp_geq_operator,
    cmp_leq_operator,
    cmp_neq_operator,
    cmp_eq_operator,
    boolean_and_operator,
    boolean_or_operator,
    boolean_xor_operator,
    namespace_concatenation,
    return_type_prefix,
};

inline const std::set<char> string_opening_characters {
    char_delim[0],
    string_delim[0],
    string_delim2[0]
};

inline const std::set<std::string> prefixes{
    address_operator,
    pointer_dereference_operator,
    boolean_not_operator,
    prefix_minus_sign,
    prefix_plus_sign,
};

inline const std::set<std::string> infixies {
    math_infix_sum_operator,
    math_infix_sub_operator,
    math_infix_mul_operator,
    math_infix_div_operator,
    math_infix_mod_operator,
    cmp_geq_operator,
    cmp_leq_operator,
    cmp_neq_operator,
    cmp_eq_operator,
    boolean_and_operator,
    boolean_or_operator,
    boolean_xor_operator,
    dot_member_access_operator
};

enum class OperatorKind {
    less_then,
    greater_then,
    less_then_or_equal,
    greater_then_or_equal,
    equal,
    not_equal,
    address_of,
    pointer_dereference_op,
    boolean_not,
    and_operator,
    or_operator,
    xor_operator,
    plus_operator,
    minus_operator,
    mul_operator,
    div_operator,
    mod_operator,
    is_operator,
    as_operator
};

inline const std::map<std::string, OperatorKind> operator_kinds {
    { cmp_le_operator,              OperatorKind::less_then                 },
    { cmp_ge_operator,              OperatorKind::greater_then              },
    { cmp_leq_operator,             OperatorKind::less_then_or_equal        },
    { cmp_geq_operator,             OperatorKind::greater_then_or_equal     },
    { cmp_eq_operator,              OperatorKind::equal                     },
    { cmp_neq_operator,             OperatorKind::not_equal                 },
    { address_operator,             OperatorKind::address_of                },
    { pointer_dereference_operator, OperatorKind::pointer_dereference_op    },
    { boolean_not_operator,         OperatorKind::boolean_not               },
    { boolean_and_operator,         OperatorKind::and_operator              },
    { boolean_or_operator,          OperatorKind::or_operator               },
    { boolean_xor_operator,         OperatorKind::xor_operator              },
    { math_infix_sum_operator,      OperatorKind::plus_operator             },
    { math_infix_sub_operator,      OperatorKind::minus_operator            },
    { math_infix_mul_operator,      OperatorKind::mul_operator              },
    { math_infix_div_operator,      OperatorKind::div_operator              },
    { math_infix_mod_operator,      OperatorKind::mod_operator              },
    { is_keyword,                   OperatorKind::is_operator               },
    { as_keyword,                   OperatorKind::as_operator               },
};

inline const std::set<std::string> type_operators { 
    is_keyword, as_keyword 
};

inline const std::map<std::string, short> infix_operators_priority {
    { boolean_xor_operator,     1 }, 
    { boolean_and_operator,     1 }, 
    { boolean_or_operator,      1 }, 
    { cmp_le_operator,          2 }, 
    { cmp_ge_operator,          2 }, 
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

inline const std::map<std::string, short> prefix_operators_priority {
    { boolean_not_operator,         5 }, 
    { prefix_minus_sign,            5 }, 
    { prefix_plus_sign,             5 }, 
    { pointer_dereference_operator, 7 }, 
    { address_operator,             7 },
};