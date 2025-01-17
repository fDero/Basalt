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
#include "core/common_feature_adoption_plan_descriptor.hpp"

[[noreturn]] void assert_unreachable();

[[noreturn]] inline void throw_unrecognized_binary_operator(const BinaryOperator& bop) {
    throw InternalError("unrecognized binary operator: " + bop.operator_text);
}

[[noreturn]] inline void throw_generics_not_allowed_in_backend_layer(const TemplateType& template_type) {
    throw InternalError("generics_not_allowed_in_backend_layer: " + template_type.type_name + " is a template type in " + std::to_string(template_type.as_debug_informations_aware_entity().line_number));
}

[[noreturn]] inline void throw_unrecognized_unary_operator(const UnaryOperator& uop) {
    throw InternalError("unrecognized unary operator: " + uop.operator_text);
}

[[noreturn]] inline void throw_unrecognized_type_operator(const TypeOperator& top) {
    throw InternalError("unrecognized type operator: " + top.operator_text);
}

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
inline void assert_typedefinition_is(const TypeDefinition& type_definition) {
    #ifdef DEBUG_BUILD
    if (!type_definition.is<T>()) {
        std::string T_type_converted_to_string = typeid(T).name();
        throw InternalError {
            "somehow a typesignature was expected to be a " + 
            T_type_converted_to_string + " instead it wasn't"    
        };
    }
    #endif
}

inline void assert_type_deduction_success_in_backend_layer(bool success) {
    #ifdef DEBUG_BUILD
    if (!success) {
        throw InternalError {
            "type deduction failed in backend"    
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
    #ifndef DEBUG_BUILD
    if (current_arg_type.is_generic()) {
        throw std::runtime_error("the compiler expected this type to be non-generic, the assumption was false");
    }
    #endif
}

inline void assert_is_recursive_cfa_plan(
    const CommonFeatureAdoptionPlan& plan
) {
    #ifndef DEBUG_BUILD
    if (!plan.is_recursive_adoption()) {
        throw std::runtime_error("the compiler expected this cfa plan to be recursive, and it wasn't");
    }
    #endif
}

inline void assert_local_variable_was_found_in_translation_aware_scope_context(bool search_success) {
    #ifndef DEBUG_BUILD
    if (!search_success) {
        throw std::runtime_error("the compiler expected this local variable to be found, and it wasn't");
    }
    #endif
}

inline void assert_is_assignment_of_non_union_to_union(bool is_union_source, bool is_union_target) {
    #ifndef DEBUG_BUILD
    if (is_union_source || !is_union_target) {
        throw std::runtime_error("the compiler expected this assignment to be of a non-union type to a union type, and it wasn't");
    }
    #endif
}