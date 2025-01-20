//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/expression_type_deducer.hpp"
#include "core/assignment_type_checker.hpp"
#include "frontend/syntax.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_address_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, address_operator);
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    return (operand_type.has_value())
        ? std::optional<TypeSignature>( PointerType { expression.as_debug_informations_aware_entity(), operand_type.value() } )
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_pointer_dereference_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, pointer_dereference_operator);
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is<PointerType>(operand_type.value());
    return operand_type.value().get<PointerType>().pointed_type;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_boolean_not_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, "!");
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is_boolean(operand_type);
    return operand_type.value();
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_math_prefix_operator_type(const UnaryOperator& expression) {
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is_either_numeric_or_generic(operand_type.value());
    return operand_type.value(); 
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_eq_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_non_string_primitive_or_generic(left_operand_type);
    ensure_typesignature_is_non_string_primitive_or_generic(right_operand_type);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type);
    return deduced
        ? deduce_primtive_type("Bool", expression) 
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_ord_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_either_numeric_or_generic(left_operand_type);
    ensure_typesignature_is_either_numeric_or_generic(right_operand_type);
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    return deduced
        ? deduce_primtive_type("Bool", expression) 
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_logical_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    if (!left_operand_type.has_value() || !right_operand_type.has_value()) {
        return deduce_primtive_type("Bool", expression);    
    }
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    ensure_typesignature_is_boolean(left_operand_type);
    ensure_typesignature_is_boolean(left_operand_type);
    return deduce_primtive_type("Bool", expression);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_math_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_either_numeric_or_generic(left_operand_type);
    ensure_typesignature_is_either_numeric_or_generic(right_operand_type);
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    return deduced
        ? std::optional<TypeSignature>(left_operand_type.value()) 
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_square_brackets_access(const Expression& square_brackets_access_expr) {
    assert_expression_is<SquareBracketsAccess>(square_brackets_access_expr);
    const SquareBracketsAccess& square_brackets_access = square_brackets_access_expr.get<SquareBracketsAccess>();
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(square_brackets_access.storage);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(square_brackets_access.index);
    ensure_typesignature_is_int(right_operand_type);
    if (!left_operand_type.has_value()) {
        return std::nullopt;
    }
    switch(left_operand_type.value().typesiganture_kind()) {
        case TypeSignatureBody::Kind::array_type: return left_operand_type.value().get<ArrayType>().stored_type;
        case TypeSignatureBody::Kind::slice_type: return left_operand_type.value().get<SliceType>().stored_type;
        default: throw_cannot_access_square_brackets_on_type(left_operand_type.value(), square_brackets_access);
    }
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access(const Expression& dot_member_access_expr) {
    assert_expression_is<DotMemberAccess>(dot_member_access_expr);
    const DotMemberAccess& dot_member_access = dot_member_access_expr.get<DotMemberAccess>();
    std::optional<TypeSignature> left_operand_type_opt = deduce_expression_type(dot_member_access.struct_value);
    if (!left_operand_type_opt.has_value()) {
        return std::nullopt;
    }
    TypeSignature left_operand_type = type_definitions_register.unalias_type(left_operand_type_opt.value());
    while (left_operand_type.is<PointerType>()) {
        left_operand_type = left_operand_type.get<PointerType>().pointed_type;
        left_operand_type = type_definitions_register.unalias_type(left_operand_type);
    }
    ensure_typesignature_is<CustomType>(left_operand_type);
    const CustomType& custom_type = left_operand_type.get<CustomType>();
    TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(custom_type);
    ensure_type_definition_is<StructDefinition>(type_definition);
    const StructDefinition& struct_type_definition = type_definition.get<StructDefinition>();
    const std::string& member_name = dot_member_access.member_name;
    for (const auto& member : struct_type_definition.fields) {
        if (member.field_name == member_name) {
            return member.field_type;
        }
    }
    throw_no_such_struct_field(member_name, struct_type_definition, dot_member_access);
}