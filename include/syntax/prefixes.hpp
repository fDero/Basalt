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

inline const std::string constexpr address_operator             = "&";
inline const std::string constexpr boolean_not_operator         = "!";
inline const std::string constexpr pointer_dereference_operator = "#";
inline const std::string constexpr prefix_minus_sign            = "-";
inline const std::string constexpr prefix_plus_sign             = "+";

inline const std::set<std::string> prefixes{
    address_operator,
    pointer_dereference_operator,
    boolean_not_operator,
    prefix_minus_sign,
    prefix_plus_sign,
};

inline const std::map<std::string, short> prefix_operators_priority {
    { boolean_not_operator,         5 }, 
    { prefix_minus_sign,            5 }, 
    { prefix_plus_sign,             5 }, 
    { pointer_dereference_operator, 7 }, 
    { address_operator,             7 },
};