//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <unordered_set>

#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

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
    const bool success, 
    const std::string& identifier
) {
    if (!success) {
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

void ensure_typesignature_is_int(const std::optional<TypeSignature>& type_signature_opt) {
    if (!type_signature_opt.has_value()) {
        return;
    }
    const TypeSignature& type_signature = type_signature_opt.value();
    if (!type_signature.is<PrimitiveType>() || type_signature.get<PrimitiveType>().type_name != "Int") {
        throw InternalError("type must be Int");
    }
}

void ensure_typesignature_is_either_array_or_slice_for_square_brackets_access(
    const TypeSignature& type_signature
) {
    if (!type_signature.is<ArrayType>() && !type_signature.is<SliceType>()) {
        throw InternalError("type must be either array or slice");
    }
}

void ensure_typesignatures_are_mutually_compatible_for_structure_comparison(
    bool lx_compatible_with_rx,
    bool rx_compatible_with_lx,
    const BinaryOperator& expression
) {
    if (!lx_compatible_with_rx || !rx_compatible_with_lx) {
        throw std::runtime_error("types are not compatible");
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

void ensure_template_types_are_equal(
    const TypeSignature& left_operand_type,
    const TypeSignature& right_operand_type
) {
    assert_typesignature_is<TemplateType>(left_operand_type);
    assert_typesignature_is<TemplateType>(right_operand_type);
    const TemplateType& left_primitive = left_operand_type.get<TemplateType>();
    const TemplateType& right_primitive = right_operand_type.get<TemplateType>();
    if (left_primitive.type_name != right_primitive.type_name) {
        throw InternalError("numeric types are not equal");
    }
}

void ensure_numeric_or_generics_types_are_equal(
    const std::optional<TypeSignature>& left_operand_type_opt,
    const std::optional<TypeSignature>& right_operand_type_opt
) { 
    if (!left_operand_type_opt.has_value() || !right_operand_type_opt.has_value()) {
        return;
    }
    const TypeSignature& left_operand_type = left_operand_type_opt.value();
    const TypeSignature& right_operand_type = right_operand_type_opt.value();   
    if (left_operand_type.is<PrimitiveType>() && right_operand_type.is<PrimitiveType>()) {
        ensure_numeric_types_are_equal(left_operand_type, right_operand_type);
        return;
    }
    if (left_operand_type.is<TemplateType>() && right_operand_type.is<TemplateType>()) {
        ensure_template_types_are_equal(left_operand_type, right_operand_type);
        return;
    }
    throw std::runtime_error("types are not equal");
}

void ensure_typesignature_is_boolean(const std::optional<TypeSignature>& type_signature_opt) {
    if (!type_signature_opt.has_value()) {
        return;
    }
    const TypeSignature& type_signature = type_signature_opt.value();
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

void ensure_typesignature_is_non_string_primitive_or_generic(const std::optional<TypeSignature>& type_signature_opt) {
    if (!type_signature_opt.has_value()) {
        return;
    }
    const TypeSignature& type_signature = type_signature_opt.value();
    if (!type_signature.is<PrimitiveType>() && !type_signature.is<TemplateType>()) {
        throw InternalError("type must be non-string primitive or generic");
    }
    if (type_signature.is<PrimitiveType>()) {
        const PrimitiveType& primitive_type = type_signature.get<PrimitiveType>();
        if (primitive_type.type_name == "String") {
            throw InternalError("type must be non-string primitive or generic");
        }
    }
}

void ensure_typesignature_is_either_numeric_or_generic(const std::optional<TypeSignature>& type_signature_opt) {
    if (!type_signature_opt.has_value()) {
        return;
    }
    const TypeSignature& type_signature = type_signature_opt.value();
    if (!type_signature.is<PrimitiveType>() && !type_signature.is<TemplateType>()) {
        throw InternalError("type must be numeric or generic");
    }
    if (type_signature.is<PrimitiveType>()) {
        const PrimitiveType& primitive_type = type_signature.get<PrimitiveType>();
        if (primitive_type.type_name != "Int" && primitive_type.type_name != "Float") {
            throw InternalError("type must be numeric or generic");
        }
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

void ensure_function_has_a_return_type(
    const FunctionCall& function_call,
    const std::optional<TypeSignature>& return_type
) {
    if (!return_type.has_value()) {
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