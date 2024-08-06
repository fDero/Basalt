
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "toolchain/representation.hpp"
#include <unordered_set>

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name, 
    const std::unordered_set<std::string>& visited_definitions
) {
    auto search_outcome = visited_definitions.find(target_tag_name);
    if (search_outcome != visited_definitions.end()) {
        throw std::runtime_error ("Cyclic dependency!!");
    }
}

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
) {
    if (!(type_definition_insertion_outcome.second)) {
        throw std::runtime_error {
            "Multiple definition of the same type in different files\n"
        };
    }
}

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome
) {
    if (!(insertion_outcome.second)) {
        throw std::runtime_error {
            "Multiple definition of the same type in different packages, both included by the same file\n"
        };
    }
}

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved
) {
    if (!retrieved.has_value()) {
        throw std::runtime_error {
            "No type definition found\n"
        };
    }
}

void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier, 
    const bool search_outcome
) {
    if (search_outcome) {
        throw std::runtime_error {
            "Identifier " + identifier + " already exists in the current scope\n"
        };
    }
}

void ensure_parent_scope_exists_for_further_local_object_search(
    const ScopeContext* parent_scope, 
    const std::string& identifier
) {
    if (parent_scope == nullptr) {
        throw std::runtime_error {
            "Cannot find object: " + identifier + "\n"
        };
    }
}

void ensure_object_is_mutable(
    const bool is_const
) {
    if (is_const) {
        throw std::runtime_error {
            "Cannot modify a constant object\n"
        };
    }
}

void ensure_typesignature_is_int(const TypeSignature& type_signature) {
    if (!type_signature.is<PrimitiveType>() || type_signature.get<PrimitiveType>().type_name != "Int") {
        throw InternalError("type must be Int");
    }
}

void ensure_typesignatures_are_mutually_compatibile_for_structure_comparison(
    bool lx_compatible_with_rx,
    bool rx_compatible_with_lx,
    const BinaryOperator& expression
) {
    if (!lx_compatible_with_rx || !rx_compatible_with_lx) {
        throw InternalError("types are not compatible");
    }
}

void ensure_numeric_types_are_equal(
    const TypeSignature& left_operand_type,
    const TypeSignature& right_operand_type
) {
    assert_typesignature_is<PrimitiveType>(left_operand_type);
    assert_typesignature_is<PrimitiveType>(right_operand_type);
    const PrimitiveType& left_primitive = left_operand_type.get<PrimitiveType>();
    const PrimitiveType& right_primitive = right_operand_type.get<PrimitiveType>();
    if (left_primitive.type_name != right_primitive.type_name) {
        throw InternalError("numeric types are not equal");
    }
}

void ensure_typesignature_is_boolean(const TypeSignature& type_signature) {
    if (!type_signature.is<PrimitiveType>() || type_signature.get<PrimitiveType>().type_name != "Bool") {
        throw InternalError("type must be Bool");
    }
}

void ensure_typesignature_is_numeric(const TypeSignature& type_signature) {
    if (!type_signature.is<PrimitiveType>()) {
        throw InternalError("type must be numeric");
    }
    const PrimitiveType& primitive_type = type_signature.get<PrimitiveType>();
    if (primitive_type.type_name != "Int" && primitive_type.type_name != "Float") {
        throw InternalError("type must be numeric");
    }
}

void ensure_function_overload_was_successfully_retrieved(
    const FunctionCall& function_call,
    const std::optional<FunctionDefinition::Ref>& retrieved
) {
    if (!retrieved.has_value()) {
        throw InternalError("no function overload found");
    }
}

void ensure_function_has_a_return_type(FunctionDefinition::Ref function_definition) {
    if (!function_definition->return_type.has_value()) {
        throw InternalError("function has no return type");
    }
}

void ensure_operator_kind_was_found(
    const std::map<std::string, OperatorKind>::const_iterator& operator_kind_search_outcome,
    const std::map<std::string, OperatorKind>& operator_kinds
) {
    if (operator_kind_search_outcome == operator_kinds.end()) {
        throw InternalError("operator kind not found");
    }
}