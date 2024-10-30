//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <map>

#include "frontend/tokenizer.hpp"
#include "language/definitions.hpp"
#include "errors/error-types.hpp"

[[noreturn]] void throw_expression_expected_got_unrecognized(
    const std::vector<Token>::iterator& iterator
);

[[noreturn]] void throw_invalid_use_of_token_within_global_scope(
    const std::vector<Token>::iterator& iterator
);

void ensure_there_are_still_tokens(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
);

void ensure_either_comma_or_closed_paren_for_function_args(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
);

void ensure_either_comma_or_closed_angular_for_generics(
    const std::vector<Token>& source_tokens,
    const Token& generics_opening_angular_bracket,
    const std::vector<Token>::iterator& iterator
);

void ensure_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const std::string& text_to_match
);

void ensure_token_is_identifier(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
);

void ensure_token_is_struct_name(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
);

void ensure_token_is_simple_type_for_template_generics(
    const std::vector<Token>::iterator& iterator
);

void ensure_token_is_fixed_array_length(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
);

void ensure_token_is_typesignature(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
);

void ensure_parenthesis_gets_closed_in_expression_wrapping(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const Token& parenthesis_opening_token

);

void ensure_square_bracket_gets_closed_in_expression_wrapping(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const Token& brackets_opening_token

);

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name, 
    const std::unordered_set<std::string>& visited_definitions
);