//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/generics_substitution_rules.hpp"
#include "language/definitions.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "misc/forward_declarations.hpp"


void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& insertion_outcome,
    const TypeDefinition& type_definition
);

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome,
    const TypeDefinition& type_definition
);

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved,
    const CustomType& type_signature
);

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
);

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name,
    const std::unordered_set<std::string>& visited_definitions,
    const std::string& type_name,
    const DebugInformationsAwareEntity& type_definition
);

void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier,
    bool already_declared,
    const DebugInformationsAwareEntity& declaration
);

void ensure_identifier_is_declared(
    bool is_declared,
    const Identifier& identifier
);

void ensure_parent_scope_exists_for_further_local_object_search(
    bool success,
    const std::string& identifier
);

void ensure_typesignature_is_boolean(
    const std::optional<TypeSignature>& operand_type,
    const Expression& operand,
    const std::string& operator_text
);

void ensure_typesignature_is_either_numeric_or_generic(
    const std::optional<TypeSignature>& operand_type,
    const Expression& operand,
    const std::string& operator_text
);

void ensure_typesignature_is_non_string_primitive_or_generic(
    const std::optional<TypeSignature>& operand_type,
    const Expression& operand,
    const std::string& operator_text
);

void ensure_numeric_or_generics_types_are_equal(
    const std::optional<TypeSignature>& left_operand_type,
    const std::optional<TypeSignature>& right_operand_type,
    const BinaryOperator& binary_operator
);

void ensure_operand_is_a_pointer_for_dereference(
    const TypeSignature& operand_type,
    const UnaryOperator& dereference
);

void ensure_not_trying_to_dereference_a_literal(
    const UnaryOperator& address_of,
    bool is_addressable
);

void ensure_type_is_union_for_type_operator(
    bool is_union,
    const TypeOperator& type_operator,
    const TypeSignature& operand_type
);

void ensure_type_operator_union_operand_can_be_queried_for_the_given_type(
    bool can_hold_queried_type,
    const TypeOperator& type_operator,
    const TypeSignature& operand_type
);

void ensure_typesignature_is_int(
    const std::optional<TypeSignature>& index_type,
    const Expression& index
);

void ensure_typesignature_is_compatible_square_brackets_access(
    const TypeSignature& storage_type,
    const SquareBracketsAccess& square_brackets_access
);

[[noreturn]] void throw_cannot_access_square_brackets_on_type(
    const TypeSignature& storage_type,
    const SquareBracketsAccess& square_brackets_access
);

void ensure_type_definition_is_struct_for_member_access(
    const TypeDefinition& type_definition,
    const DotMemberAccess& dot_member_access
);

[[noreturn]] void throw_no_such_struct_field(
    const std::string& member_name,
    const StructDefinition& struct_definition,
    const DotMemberAccess& dot_member_access
);

[[noreturn]] void throw_bad_dot_member_access_on_type(
    const TypeSignature& left_operand_type,
    const DotMemberAccess& dot_member_access
);

[[noreturn]] void throw_no_such_primitive_field(
    const std::string& member_name,
    const PrimitiveType& primitive_type,
    const DotMemberAccess& dot_member_access
);

[[noreturn]] void throw_no_such_slice_field(
    const std::string& member_name,
    const SliceType& slice_type,
    const DotMemberAccess& dot_member_access
);

void ensure_array_literal_contains_the_right_number_of_elements(
    const ArrayLiteral& array_literal
);

void ensure_element_is_compatible_with_array_literal(
    bool element_can_be_analyzed,
    bool element_is_compatible,
    const ArrayLiteral& array_literal,
    const Expression& element,
    const std::optional<TypeSignature>& element_type
);

void ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(
    const std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRule::Set::Ref>>& best_matches,
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
);

void ensure_common_feature_adoption_is_possible(
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator,
    std::vector<TypeSignature>::const_iterator arg_types_end,
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
);

void ensure_common_feature_adoption_is_possible(
    const FunctionCall& function_call,
    bool contains_void_return_type,
    bool contains_non_void_return_type
);

void ensure_function_has_a_return_type(
    const FunctionCall& function_call,
    const std::optional<TypeSignature>& return_type
);

void ensure_return_value_of_function_is_treated_correctly_during_fcall(
    const FunctionCall& function_call,
    bool should_return_something,
    bool is_void
);

void ensure_variable_declaration_is_valid(
    bool assignment_is_valid,
    const VariableDeclaration& variable_declaration,
    const std::optional<TypeSignature>& value_type
);

void ensure_const_declaration_is_valid(
    bool assignment_is_valid,
    const ConstDeclaration& const_declaration,
    const std::optional<TypeSignature>& value_type
);

void ensure_assignment_is_valid(
    bool assignment_is_valid,
    const Assignment& assignment,
    const std::optional<TypeSignature>& target_type,
    const std::optional<TypeSignature>& value_type
);

void ensure_condition_expression_is_boolean(
    const std::optional<TypeSignature>& condition_type,
    const Expression& condition
);

void ensure_return_statement_is_congruent_to_function_definition(
    bool return_value_is_expected,
    bool return_value_is_provided,
    const Return& return_statement,
    const std::optional<TypeSignature>& expected_return_type
);

void ensure_return_statement_is_valid(
    bool assignment_is_valid,
    const Return& return_statement,
    const std::optional<TypeSignature>& return_value_type,
    const std::optional<TypeSignature>& expected_return_type
);

void ensure_every_path_returns_in_function_exit_navigation(
    bool every_path_returns,
    const FunctionDefinition& function_definition
);

void ensure_no_unreachable_code_in_function_exit_navigation(
    bool code_from_now_on_is_unreachable,
    const Statement& statement
);

void ensure_no_loop_specific_statements_outside_loop_body_in_function_exit_navigation(
    bool inside_loop,
    const Statement& statement
);

void ensure_use_as_function_argument_complies_with_const_qualifiers(
    const FunctionCall& function_call,
    const Expression& argument,
    bool assignment_discards_qualifiers
);

void ensure_assignment_complies_with_const_qualifiers(
    const VariableDeclaration& variable_declaration,
    bool assignment_discards_qualifiers
);

void ensure_assignment_complies_with_const_qualifiers(
    const Assignment& assignment,
    bool assignment_discards_qualifiers,
    bool target_is_a_constant,
    bool value_gives_access_to_immutable_data
);

void ensure_not_multiple_main_functions_have_been_found(
    const std::vector<FunctionDefinition::Ref>& main_function_definitions
);

void ensure_main_function_exists(
    const std::vector<FunctionDefinition::Ref>& main_function_definitions
);

void ensure_main_function_is_not_generic(
    const FunctionDefinition::Ref& main_function_definition
);

void ensure_main_function_has_no_arguments(
    const FunctionDefinition::Ref& main_function_definition
);

void ensure_main_function_returns_either_int_or_void(
    const FunctionDefinition::Ref& main_function_definition
);

void ensure_main_function_is_in_main_package(
    const FunctionDefinition::Ref& main_function_definition,
    const std::string& package_name
);
