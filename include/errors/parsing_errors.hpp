
#pragma once
#include "toolchain/tokenizer.hpp"
#include "language/definitions.hpp"
#include "errors/error-types.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <map>

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

void ensure_struct_doesnt_already_exists(
    const std::string& struct_tag_name,
    const StructDefinition& struct_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
);

void ensure_union_doesnt_already_exists(
    const std::string& union_tag_name,
    const UnionDefinition& union_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
);

void ensure_there_is_still_a_scope_in_wich_keep_searching_struct_definitions(
    const std::map<std::string, std::string>::const_iterator& parent_scope_search_outcome,
    const std::map<std::string, std::string>& struct_parents
);

void ensure_function_doesnt_already_exists(
    const std::string& func_tag_name,
    const FunctionDefinition& func_def,
    const std::unordered_map<std::string, FunctionDefinition>& function_definitions_register
);

void ensure_overload_set_does_exist(
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator overload_set, 
    const std::string& func_name, 
    const std::unordered_map<std::string, std::vector<std::string>> func_names_register
);

void ensure_a_valid_function_overload_was_actually_found(
    const std::string& target_func_tag_name
);

void ensure_no_function_overload_was_found_before_this(
    const std::string& previous_func_tag_name, const std::string& current_func_tag_name,
    const std::unordered_map<std::string, FunctionDefinition>& function_definitions_register
);

void ensure_no_multiple_definition_of_the_same_function(
    const std::pair<std::unordered_map<std::string, FunctionDefinition>::iterator, bool>& 
        function_definition_insertion_outcome
);

void ensure_no_multiple_definition_of_the_same_struct(
    const std::pair<std::map<std::string, TypeDefinition>::iterator, bool>& 
        struct_definition_insertion_outcome
);

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name, 
    const std::unordered_set<std::string>& visited_definitions
);

void ensure_primitive_type_is_not_generic(
    const Token& typesignature_token,
    const std::vector<TypeSignature>& template_generics
);