//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "frontend/tokenizer.hpp"
#include "language/definitions.hpp"
#include "errors/compilation_error.hpp"
#include "language/functions.hpp"
#include "misc/forward_declarations.hpp"
#include "core/scope_context.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "syntax/primitive_types.hpp"

inline void ensure_assignment_is_valid(
    bool assignment_is_valid,
    const TypeSignature& dest_type,
    const Expression& dest_expression,
    const TypeSignature& source_type,
    const Expression& source_expression
) {
    if (!assignment_is_valid) {
        throw std::runtime_error("assignment is invalid-1");
    }
}

[[noreturn]] inline void throw_bad_dot_member_access_on_type(
    const TypeSignature& left_operand_type,
    const DotMemberAccess& dot_member_access
) {
    throw std::runtime_error("bad dot member access on type");
}

[[noreturn]]  inline void throw_no_such_primitive_field(
    const std::string& member_name,
    const PrimitiveType& primitive_type_definition,
    const DotMemberAccess& dot_member_access
) {
    throw std::runtime_error("no such primitive field");
}

[[noreturn]] inline void throw_no_such_slice_field(
    const std::string& member_name,
    const SliceType& pointer_type_definition,
    const DotMemberAccess& dot_member_access
) {
    throw std::runtime_error("no such pointer field");
}

inline void ensure_type_is_union_for_type_operator(bool type_operator_typesignature_is_union) {
    if (!type_operator_typesignature_is_union) {
        throw std::runtime_error("type operator must be applied to a union type");
    }
}

inline void ensure_variable_declaration_is_valid(
    bool assignment_is_valid, 
    const VariableDeclaration& variable_declaration, 
    const std::optional<TypeSignature>& expr_type
) {
    if (!assignment_is_valid) {
        throw std::runtime_error("variable declaration is invalid");
    }
}

inline void ensure_const_declaration_is_valid(
    bool assignment_is_valid, 
    const ConstDeclaration& const_declaration, 
    const std::optional<TypeSignature>& expr_type
) {
    if (!assignment_is_valid) {
        throw std::runtime_error("variable declaration is invalid");
    }
}

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
);

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome
);

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved
);

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
);

void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier, 
    const bool search_outcome
);

void ensure_parent_scope_exists_for_further_local_object_search(
    const bool parent_scope,
    const std::string& identifier
);

void ensure_object_is_mutable(bool is_const);

inline void ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(
    const std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRule::Set::Ref>>& best_matches_so_far
) {
    if (best_matches_so_far.size() > 1) {
        throw std::runtime_error("ambiguous function call");
    }
}

template <typename T>
inline void ensure_typesignature_is(const TypeSignature& type_signature) {
    if (!type_signature.is<T>()) {
        throw std::runtime_error(std::string("type must be ") + typeid(T).name());
    }
}

template <typename T>
inline void ensure_expression_is(const Expression& expression) {
    if (!expression.is<T>()) {
        throw std::runtime_error(std::string("expression must be ") + typeid(T).name());
    }
}

template <typename T>
inline void ensure_type_definition_is(const TypeDefinition& tdef) {
    if (!tdef.is<T>()) {
        throw std::runtime_error(std::string("type must be ") + typeid(T).name());
    }
}

template <typename T>
inline void ensure_statement_is(const Statement& statement) {
    if (!statement.is<T>()) {
        throw std::runtime_error(std::string("statement must be ") + typeid(T).name());
    }
}

inline void ensure_not_trying_to_dereference_a_literal(const Expression& expression, bool is_addressable) {
    if (!is_addressable) {
        throw std::runtime_error("trying to address a non addressable expression");
    }
}

void ensure_function_overload_was_successfully_retrieved(
    const FunctionCall& function_call,
    const std::optional<FunctionDefinition::Ref>& retrieved
);

void ensure_function_has_a_return_type(
    const FunctionCall& function_call, 
    const std::optional<TypeSignature>& return_type
);

void ensure_typesignature_is_numeric(
    const TypeSignature& type_signature
);

void ensure_typesignature_is_non_string_primitive_or_generic(
    const std::optional<TypeSignature>& type_signature_opt
);

void ensure_typesignature_is_either_numeric_or_generic(
    const std::optional<TypeSignature>& type_signature_opt
);

void ensure_typesignature_is_boolean(
    const std::optional<TypeSignature>& type_signature_opt
);

[[noreturn]] void throw_cannot_access_square_brackets_on_type(
    const TypeSignature& left_operand_type,
    const SquareBracketsAccess& expression
);

[[noreturn]] void throw_no_such_struct_field(
    const std::string& member_name,
    const StructDefinition& struct_type_definition,
    const DotMemberAccess& expression
);

[[noreturn]] void throw_bad_assignment_target(const Expression& expression);

void ensure_typesignature_is_int(const std::optional<TypeSignature>& type_signature_opt);

void ensure_typesignature_is_compatible_square_brackets_access(
    const TypeSignature& type_signature
);

void ensure_typesignatures_are_mutually_compatible_for_structure_comparison(
    bool lx_compatible_with_rx,
    bool rx_compatible_with_lx,
    const BinaryOperator& expression
);

void ensure_numeric_types_are_equal(
    const TypeSignature& left_operand_type,
    const TypeSignature& right_operand_type
);

void ensure_template_types_are_equal(
    const TypeSignature& left_operand_type,
    const TypeSignature& right_operand_type
);

void ensure_numeric_or_generics_types_are_equal(
    const std::optional<TypeSignature>& left_operand_type_opt,
    const std::optional<TypeSignature>& right_operand_type_opt
);

void assert_number_of_arguments_match(
    FunctionDefinition::Ref function_definition,
    const std::vector<TypeSignature>& arg_types
);

inline void ensure_return_statement_is_congruent_to_function_definition(
    bool return_value_is_expected,
    bool return_value_is_provided
) {
    if (return_value_is_expected != return_value_is_provided) {
        throw std::runtime_error("return statement is not congruent to function definition");
    }
}

inline void ensure_return_statement_is_valid(
    bool assignment_is_valid,
    const Return& return_statement
) {
    if (!assignment_is_valid) {
        throw std::runtime_error("return value type is not congruent to function definition");
    }
}

inline void ensure_assignment_is_valid(
    bool assignment_is_valid,
    const Assignment& assignment
) {
    if (!assignment_is_valid) {
        throw std::runtime_error("assignment is invalid-2");
    }
}

inline void ensure_return_value_of_function_is_treated_correctly_during_fcall(
    const FunctionCall& function_call,
    bool should_return_something,
    bool is_void
) {
    if (should_return_something == is_void) {
        throw std::runtime_error("invalid return statement");
    }
}

inline void ensure_array_literal_contains_the_right_number_of_elements(
    const ArrayLiteral& array_literal
) {
    if (!array_literal.array_length.has_value()) {
        return;
    }
    if (array_literal.elements.size() != *array_literal.array_length) {
        throw std::runtime_error(
            "array literal contains the wrong number of elements"
        );
    }
}

inline void ensure_element_is_compatible_with_array_literal(
    bool element_can_be_analyzed,
    bool element_is_compatible,
    const ArrayLiteral& array_literal,
    const Expression& element
) {
    if (!element_is_compatible) {
        throw std::runtime_error("element is not compatible with array literal");
    }
}

inline void ensure_type_operator_union_operand_can_be_queried_for_the_given_type(
    bool can_assign_values_of_given_type_to_the_given_union
) {
    if (!can_assign_values_of_given_type_to_the_given_union) {
        throw std::runtime_error("not valid use of type operator");
    }
}

inline void ensure_identifier_exists(
    bool identifier_exists, 
    const Identifier& identifier
) {
    if (!identifier_exists) {
        throw std::runtime_error("");
    }
}

inline void ensure_return_value_type_is_congruent_to_function_definition(
    bool assignment_is_valid,
    const TypeSignature& return_value_type,
    const FunctionDefinition& function_definition,
    const Statement& return_statement
) {
    if (!assignment_is_valid) {
        throw std::runtime_error("return value type is not congruent to function definition");
    }
}

inline void ensure_common_feature_adoption_is_possible(
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator,
    std::vector<TypeSignature>::const_iterator arg_types_end
) {
    if (current_arg_type_iterator == arg_types_end) {
        throw std::runtime_error("common feature adoption is not possible");
    }
}

inline void ensure_common_feature_adoption_is_possible(
    const FunctionCall& function_call,
    bool contains_void_return_type,
    bool contains_non_void_return_type
) {
    if (contains_non_void_return_type && contains_void_return_type) {
        throw std::runtime_error("common feature adoption is not possible");
    }
}

inline void ensure_every_path_returns_in_function_exit_navigation(
    bool every_path_returns
) {
    if (!every_path_returns) {
        throw std::runtime_error("not every path returns");
    }
}

inline void ensure_no_unreachable_code_in_function_exit_navigation(
    bool code_from_now_on_is_unreachable
) {
    if (code_from_now_on_is_unreachable) {
        throw std::runtime_error("unreachable code");
    }
}

inline void ensure_no_loop_specific_statements_outside_loop_body_in_function_exit_navigation(
    bool inside_loop
) {
    if (!inside_loop) {
        throw std::runtime_error("loop specific statement outside loop body");
    }
}

inline void ensure_use_as_function_argument_complies_with_const_qualifiers(
    const FunctionCall& function_call,
    const Expression& argument,
    bool assignment_discard_qualifiers
) {
    if (assignment_discard_qualifiers) {
        throw std::runtime_error("use as function argument does not comply with const qualifiers");
    }
}

inline void ensure_assignment_complies_with_const_qualifiers(
    const Statement& function_call,
    bool assignment_discard_qualifiers
) {
    if (assignment_discard_qualifiers) {
        throw std::runtime_error("assignment discards qualifiers");
    }
}

inline void ensure_condition_expression_is_boolean(
    const std::optional<TypeSignature>& condition_type,
    const Expression& condition
) {
    if (!condition_type.has_value()) {
        throw std::runtime_error("condition expression is not boolean");
    }
    if (!condition_type->is<PrimitiveType>() || condition_type->get<PrimitiveType>().type_name != bool_type) {
        throw std::runtime_error("condition expression is not boolean");
    }
}

inline void ensure_not_multiple_main_functions_have_been_found(
    size_t main_function_counter
) {
    if (main_function_counter > 1) {
        throw std::runtime_error("multiple main functions have been found");
    }
}

inline void ensure_main_function_is_not_generic(
    const FunctionDefinition::Ref& main_function_definition
) {
    if (!main_function_definition->template_generics_names.empty()) {
        throw std::runtime_error("main function cannot be generic");
    }
}

inline void assert_function_name_is_main(
    const FunctionDefinition::Ref& main_function_definition
) {
    if (main_function_definition->function_name != "main") {
        throw std::runtime_error("main function must be named 'main'");
    }
}

inline void ensure_main_function_has_no_arguments(
    const FunctionDefinition::Ref& main_function_definition
) {
    if (!main_function_definition->arguments.empty()) {
        throw std::runtime_error("main function cannot have arguments");
    }
}

inline void ensure_main_function_returns_either_int_or_void(
    const FunctionDefinition::Ref& main_function_definition
) {
    if (!main_function_definition->return_type.has_value()) {
        return;
    }
    TypeSignature return_type = *main_function_definition->return_type;
    if (!return_type.is<PrimitiveType>()) {
        throw std::runtime_error("main function must return either void or int");
    }
    if (return_type.get<PrimitiveType>().type_name != int_type) {
        throw std::runtime_error("main function must return either void or int");
    }
}

inline void ensure_main_function_is_in_main_package(
    const std::string& package_name
) {
    if (package_name != "main") {
        throw std::runtime_error("main function must be in the 'main' package");
    }
}