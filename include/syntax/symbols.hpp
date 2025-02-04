//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <set>

#include "syntax/infixes.hpp"
#include "syntax/specials.hpp"

inline char constexpr plus_symbol        = '+';
inline char constexpr minus_symbol       = '-';
inline char constexpr star_symbol        = '*';
inline char constexpr div_symbol         = '/';
inline char constexpr mod_symbol         = '%';

inline char constexpr eq_symbol          = '=';
inline char constexpr lx_angular_symbol  = '<';
inline char constexpr rx_angular_symbol  = '>';
inline char constexpr bang_symbol        = '!';

inline char constexpr dot_symbol         = '.';
inline char constexpr colon_symbol       = ':';
inline char constexpr comma_symbol       = ',';
inline char constexpr semicolon_symbol   = ';';

inline char constexpr sharp_symbol       = '#';
inline char constexpr dollar_symbol      = '$';
inline char constexpr ampersand_symbol   = '&';
inline char constexpr pipe_symbol        = '|';

inline char constexpr open_round_paren   = '(';
inline char constexpr close_round_paren  = ')';
inline char constexpr open_square_paren  = '[';
inline char constexpr close_square_paren = ']';
inline char constexpr open_curly_paren   = '{';
inline char constexpr close_curly_paren  = '}';

inline const std::set<char> symbols{
    plus_symbol,
    minus_symbol,
    star_symbol,
    div_symbol,
    mod_symbol,

    eq_symbol,
    lx_angular_symbol,
    rx_angular_symbol,
    bang_symbol,

    colon_symbol,
    dot_symbol,
    comma_symbol,
    semicolon_symbol,

    sharp_symbol,
    dollar_symbol,
    ampersand_symbol,
    pipe_symbol,

    open_round_paren,
    close_round_paren, 
    open_square_paren, 
    close_square_paren,
    open_curly_paren,
    close_curly_paren 
};

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