
#pragma once
#include "toolchain/tokenizer.hpp"
#include "language/syntax.hpp"
#include "language/typesystem.hpp"
#include "language/definitions.hpp"
#include "toolchain/rappresentation.hpp"
#include <vector>
#include "errors/error-types.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>

[[noreturn]] void assert_unreachable();

[[noreturn]] void throw_unknown_token_type();

[[noreturn]] void throw_attempt_to_retrieve_struct_definition_from_primitive_type(const TypeSignature& type);

void assert_integer_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_floating_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_character_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_boolean_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_string_literal_properly_fromatted(const std::vector<Token>::iterator& iterator);

void assert_identifier_is_properly_fromatted(const std::vector<Token>::iterator& iterator);

void assert_type_is_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_token_is_prefix_operator(const std::vector<Token>::iterator& iterator);

void assert_token_is_binary_operator(const std::vector<Token>::iterator& iterator);

void assert_basetype_has_no_generics(const TypeSignature& typesignature);

void assert_type_is_non_primitive(const TypeSignature& type);

void assert_token_sourcetext_non_empty(const std::string& sourcetext);

void assert_current_token_is_function_keyword(const std::vector<Token>::iterator& iterator);

void assert_symbol_is_binary_operator(const std::string& symbol);

void assert_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator, 
    const std::string& text
);

void assert_token_is_string_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_boolean_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_character_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_floating_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_integer_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_text(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_simple_type(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_tokens_not_ended(
    const std::vector<Token>::iterator& iterator, 
    const std::vector<Token>& tokens
);

void assert_function_actually_retrieved(
    const std::unordered_map<std::string, FunctionDefinition>::const_iterator iterator,
    const std::unordered_map<std::string, FunctionDefinition>& function_definitions_register
);

void assert_get_operation_is_possible(
    const std::string& wanted_type, 
    const std::type_info& type_info
);

template<typename T>
inline void assert_typesignature_is(const TypeSignature& typesignature){
    #ifdef DEBUG_BUILD
    if (!typesignature.is<T>()){
        std::string T_type_converted_to_string = typeid(T).name();
        throw InternalError {
            "somehow a typesignature was expected to be a " + 
            T_type_converted_to_string + " instead it wasn't"    
        };
    }
    #endif
}

void assert_no_errors_encountered_during_async_parsing_hence_error_vector_is_empty(
    const std::vector<std::exception_ptr>& errors
);

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& instanciated_generics
);