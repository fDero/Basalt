//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <set>

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