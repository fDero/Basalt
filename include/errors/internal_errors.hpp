//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <list>

#include "frontend/tokenizer.hpp"
#include "frontend/syntax.hpp"
#include "language/typesignatures.hpp"
#include "language/definitions.hpp"
#include "frontend/file_representation.hpp"
#include "errors/error-types.hpp"

[[noreturn]] void assert_unreachable();

void assert_integer_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_floating_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_character_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_boolean_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_string_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_identifier_is_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_type_is_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_token_is_prefix_operator(const std::vector<Token>::iterator& iterator);

void assert_token_is_binary_operator(const std::vector<Token>::iterator& iterator);

void assert_token_is_type_operator(const std::vector<Token>::iterator& iterator);

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

void assert_tokens_not_ended(
    const std::vector<Token>::iterator& iterator, 
    const std::vector<Token>& tokens
);

template<typename T>
inline void assert_typesignature_is(const TypeSignature& typesignature) {
    #ifdef DEBUG_BUILD
    if (!typesignature.is<T>()) {
        std::string T_type_converted_to_string = typeid(T).name();
        throw InternalError {
            "somehow a typesignature was expected to be a " + 
            T_type_converted_to_string + " instead it wasn't"    
        };
    }
    #endif
}

template<typename T>
inline void assert_expression_is(const Expression& expression) {
    #ifdef DEBUG_BUILD
    if (!expression.is<T>()) {
        std::string T_type_converted_to_string = typeid(T).name();
        throw InternalError {
            "somehow a typesignature was expected to be a " + 
            T_type_converted_to_string + " instead it wasn't"    
        };
    }
    #endif
}

template<typename T>
inline void assert_statement_is(const Statement& statement) {
    #ifdef DEBUG_BUILD
    if (!statement.is<T>()) {
        std::string T_type_converted_to_string = typeid(T).name();
        throw InternalError {
            "somehow a typesignature was expected to be a " + 
            T_type_converted_to_string + " instead it wasn't"    
        };
    }
    #endif
}

inline void assert_unary_operator_is(const UnaryOperator& expression, const std::string& operator_text) {
    #ifdef DEBUG_BUILD
    if (expression.operator_text != operator_text) {
        throw InternalError("expected unary operator to be " + operator_text);
    }
    #endif
}

inline void assert_binary_operator_is(const BinaryOperator& expression, const std::string& operator_text) {
    #ifdef DEBUG_BUILD
    if (expression.operator_text != operator_text) {
        throw InternalError("expected unary operator to be " + operator_text);
    }
    #endif
}

inline void assert_operator_kind_was_found(
    const std::map<std::string, OperatorKind>::const_iterator& operator_kind_search_outcome,
    const std::map<std::string, OperatorKind>& operator_kinds
) {
    if (operator_kind_search_outcome == operator_kinds.end()) {
        throw InternalError("operator kind not found");
    }
}

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& type_parameters
);

void assert_packagename_is_found(
    const std::unordered_map<std::string, std::string>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::string>::const_iterator& end
);

void assert_files_are_found(
    const std::unordered_map<std::string, std::list<FileRepresentation>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::list<FileRepresentation>>::const_iterator& end
);

void assert_imports_vector_is_found(
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator& end
);

void assert_overload_set_exists(
    const std::unordered_map<std::string, std::vector<FunctionDefinition::Ref>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<FunctionDefinition::Ref>>::const_iterator& end
);

inline void assert_current_arg_type_is_not_generic(const TypeSignature& current_arg_type) {
    if (current_arg_type.is_generic()) {
        throw std::runtime_error("the compiler expected this type to be non-generic, the assumption was false");
    }
}