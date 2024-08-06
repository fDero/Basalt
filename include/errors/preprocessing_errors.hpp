
#include "toolchain/tokenizer.hpp"
#include "language/definitions.hpp"
#include "errors/error-types.hpp"
#include "language/functions.hpp"
#include "misc/forward_declarations.hpp"
#include <unordered_map>
#include <unordered_set>

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
    const ScopeContext* parent_scope,
    const std::string& identifier
);

void ensure_object_is_mutable(bool is_const);

inline void ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(
    const std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRuleSet::Ref>>& best_matches_so_far
) {
    if (best_matches_so_far.size() > 1) {
        throw std::runtime_error("ambiguous function call");
    }
}

template <typename T>
inline void ensure_typesignature_is(const TypeSignature& type_signature) {
    if (!type_signature.is<T>()) {
        throw InternalError(std::string("type must be ") + typeid(T).name());
    }
}

template <typename T>
inline void ensure_expression_is(const Expression& expression) {
    if (!expression.is<T>()) {
        throw InternalError(std::string("expression must be ") + typeid(T).name());
    }
}

template <typename T>
inline void ensure_type_definition_is(const TypeDefinition& tdef) {
    if (!tdef.is<T>()) {
        throw InternalError(std::string("type must be ") + typeid(T).name());
    }
}

template <typename T>
inline void ensure_statement_is(const Statement& statement) {
    if (!statement.is<T>()) {
        throw InternalError(std::string("statement must be ") + typeid(T).name());
    }
}

void ensure_function_overload_was_successfully_retrieved(
    const FunctionCall& function_call,
    const std::optional<FunctionDefinition::Ref>& retrieved
);

void ensure_function_has_a_return_type(FunctionDefinition::Ref function_definition);

void ensure_operator_kind_was_found(
    const std::map<std::string, OperatorKind>::const_iterator& operator_kind_search_outcome,
    const std::map<std::string, OperatorKind>& operator_kinds
);

void ensure_typesignature_is_numeric(const TypeSignature& type_signature);

void ensure_typesignature_is_boolean(const TypeSignature& type_signature);

[[noreturn]] void throw_cannot_access_square_brackets_on_type(
    const TypeSignature& left_operand_type,
    const BinaryOperator& expression
);

[[noreturn]] void throw_no_such_struct_field(
    const std::string& member_name,
    const StructDefinition& struct_type_definition,
    const BinaryOperator& expression
);

void ensure_typesignature_is_int(const TypeSignature& type_signature);

void ensure_typesignatures_are_mutually_compatibile_for_structure_comparison(
    bool lx_compatible_with_rx,
    bool rx_compatible_with_lx,
    const BinaryOperator& expression
);

void ensure_numeric_types_are_equal(
    const TypeSignature& left_operand_type,
    const TypeSignature& right_operand_type
);

void assert_number_of_arguments_match(
    FunctionDefinition::Ref function_definition,
    const std::vector<TypeSignature>& arg_types
);