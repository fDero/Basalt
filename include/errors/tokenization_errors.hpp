//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <optional> 

#include "frontend/tokenizer.hpp"
#include "errors/error-types.hpp"
#include "misc/debug_informations_aware_entity.hpp"

[[noreturn]] void throw_unexpected_token(
    const Token& token
);

void avoid_multiple_floating_points(
    bool floating, 
    char maybe_decimal_point,
    const DebugInformationsAwareEntity& coordinates
);

void ensure_string_gets_closed(
    const std::string& buffer, 
    const DebugInformationsAwareEntity& coordinates
);

void ensure_multiline_comments_get_closed(
    const std::stack<Token>& multiline_comments_tracker, 
    const DebugInformationsAwareEntity& coordinates
);