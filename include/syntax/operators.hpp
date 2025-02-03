//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <set>
#include <map>

#include "syntax/symbols.hpp"

inline const std::string constexpr address_operator = "&";
inline const std::string constexpr pointer_dereference_operator = "#";
inline const std::string constexpr pointer_type_symbol = "#";
inline const std::string constexpr slice_type_symbol = "$";

inline const std::string square_brackets_access = "[square-brackets-access]";
inline const std::string array_literal_example = "[SIZE] Type {values...}";

inline const std::string constexpr array_type_example = "[SIZE] Type";
inline const std::string constexpr array_literal_first_symbol = "[";
inline const std::string constexpr array_type_first_symbol = "[";



inline const std::set<std::string> combinations{
    ">=", "<=", "!=", "++", "--", "==", "->", "<-", "&&", "||", "^^", "::"
};

inline const std::set<char> string_opening_characters {
    '\'','"','`'
};

inline const std::set<char> discardable{
    '\t','\r','\0','\n',' '
};

inline const std::set<std::string> prefixes{
    "!","-","+","#",".","&"
};

inline const std::set<std::string> infixies {
    "+","-","*","/","%",
    "<",">","<=",">=","==","!=",
    "&&","||","^^"
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
    math_increment,
    math_decrement,
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
    { "<" ,                         OperatorKind::less_then                 },
    { ">" ,                         OperatorKind::greater_then              },
    { "<=" ,                        OperatorKind::less_then_or_equal        },
    { ">= ",                        OperatorKind::greater_then_or_equal     },
    { "==",                         OperatorKind::equal                     },
    { "!=",                         OperatorKind::not_equal                 },
    { address_operator,             OperatorKind::address_of                },
    { pointer_dereference_operator, OperatorKind::pointer_dereference_op    },
    { "!",                          OperatorKind::boolean_not               },
    { "++",                         OperatorKind::math_increment            },
    { "--",                         OperatorKind::math_decrement            },
    { "&&",                         OperatorKind::and_operator              },
    { "||",                         OperatorKind::or_operator               },
    { "^^",                         OperatorKind::xor_operator              },
    { "+",                          OperatorKind::plus_operator             },
    { "-",                          OperatorKind::minus_operator            },
    { "*",                          OperatorKind::mul_operator              },
    { "/",                          OperatorKind::div_operator              },
    { "%",                          OperatorKind::mod_operator              },
    { "is",                         OperatorKind::is_operator               },
    { "as",                         OperatorKind::as_operator               }
};

inline const std::set<std::string> type_operators { 
    "is", "as" 
};

inline const std::map<std::string, short> infix_operators_priority {
    { "||", 1 }, 
    { "&&", 1 }, 
    { "^^", 1 }, 
    { "<", 2 }, 
    { ">", 2 }, 
    { "<=", 2 }, 
    { ">=", 2 }, 
    { "!=", 2 }, 
    { "==", 2 }, 
    { "+", 3 }, 
    { "-", 3 }, 
    { "*", 4 }, 
    { "/", 4 }, 
    { "%", 4 }
};

inline const std::map<std::string, short> prefix_operators_priority {
    { "!", 5 }, 
    { "-", 5 }, 
    { "+", 5 }, 
    { "++", 5 }, 
    { "--", 5 },
    { pointer_dereference_operator, 7 }, 
    { address_operator, 7 }
};