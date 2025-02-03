//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <set>

inline const std::string constexpr int_type        = "Int";
inline const std::string constexpr float_type      = "Float";
inline const std::string constexpr bool_type       = "Bool";
inline const std::string constexpr char_type       = "Char";
inline const std::string constexpr string_type     = "String";
inline const std::string constexpr raw_string_type = "RawString";

inline const std::set<std::string> primitive_types {
    int_type,
    float_type,
    bool_type,
    char_type,
    string_type,
    raw_string_type
};