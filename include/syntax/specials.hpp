//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <set>

#include "syntax/infixes.hpp"

inline const std::string constexpr pointer_type_symbol          = "#";
inline const std::string constexpr slice_type_symbol            = "$";
inline const std::string square_brackets_access                 = "[square-brackets-access]";

inline const std::string array_literal_example                  = "[SIZE] Type {values...}";
inline const std::string constexpr array_type_example           = "[SIZE] Type";
inline const std::string constexpr array_literal_first_symbol   = "[";
inline const std::string constexpr array_type_first_symbol      = "[";

inline const std::string constexpr single_line_comment          = "//";
inline const std::string constexpr multiline_comment_opening    = "/*";
inline const std::string constexpr multiline_comment_closing    = "*/";
inline const std::string constexpr namespace_concatenation      = "::";
inline const std::string constexpr return_type_prefix           = "->";

inline const std::set<char> discardable{
    '\t','\r','\0','\n',' '
};

inline const std::string constexpr char_delim                   = "'";
inline const std::string constexpr string_delim                 = "\"";
inline const std::string constexpr string_delim2                = "`";

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