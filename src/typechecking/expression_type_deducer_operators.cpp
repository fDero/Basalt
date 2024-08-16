
#include "toolchain/typechecking.hpp"
#include "language/syntax.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_address_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, address_operator);
    const TypeSignature& operand_type = deduce_expression_type(expression.operand);
    return PointerType { expression.as_debug_informations_aware_entity(), operand_type };
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_pointer_dereference_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, pointer_dereference_operator);
    const TypeSignature& operand_type = deduce_expression_type(expression.operand);
    ensure_typesignature_is<PointerType>(operand_type);
    return operand_type.get<PointerType>().pointed_type;
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_boolean_not_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, "!");
    const TypeSignature& operand_type = deduce_expression_type(expression.operand);
    ensure_typesignature_is_boolean(operand_type);
    return operand_type;
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_math_prefix_operator_type(const UnaryOperator& expression) {
    const TypeSignature& operand_type = deduce_expression_type(expression.operand);
    ensure_typesignature_is_numeric(operand_type);
    return operand_type; 
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_comparison_operator(const BinaryOperator& expression) {
    const TypeSignature& left_operand_type = deduce_expression_type(expression.left_operand);
    const TypeSignature& right_operand_type = deduce_expression_type(expression.right_operand);
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    bool lx_compatible_with_rx = assignment_type_checker.validate_assignment(left_operand_type, right_operand_type);
    bool rx_compatible_with_lx = assignment_type_checker.validate_assignment(right_operand_type, left_operand_type);
    ensure_typesignatures_are_mutually_compatibile_for_structure_comparison(lx_compatible_with_rx, rx_compatible_with_lx, expression);
    return deduce_primtive_type("Bool", expression);
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_square_brackets_access(const Expression& square_brackets_access_expr) {
    assert_expression_is<SquareBracketsAccess>(square_brackets_access_expr);
    const SquareBracketsAccess& square_brackets_access = square_brackets_access_expr.get<SquareBracketsAccess>();
    const TypeSignature& left_operand_type = deduce_expression_type(square_brackets_access.storage);
    const TypeSignature& right_operand_type = deduce_expression_type(square_brackets_access.index);
    ensure_typesignature_is_int(right_operand_type);
    switch(left_operand_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::array_type: return left_operand_type.get<ArrayType>().stored_type;
        case TypeSignatureBody::Kind::slice_type: return left_operand_type.get<SliceType>().stored_type;
        default: throw_cannot_access_square_brackets_on_type(left_operand_type, square_brackets_access);
    }
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_dot_member_access(const Expression& dot_member_access_expr) {
    assert_expression_is<DotMemberAccess>(dot_member_access_expr);
    const DotMemberAccess& dot_member_access = dot_member_access_expr.get<DotMemberAccess>();
    TypeSignature left_operand_type = deduce_expression_type(dot_member_access.struct_value);
    left_operand_type = type_definitions_register.unalias_type(left_operand_type);
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

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_math_binary_operator(const BinaryOperator& expression) {
    const TypeSignature& left_operand_type = deduce_expression_type(expression.left_operand);
    const TypeSignature& right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_numeric(left_operand_type);
    ensure_numeric_types_are_equal(left_operand_type, right_operand_type);
    return deduce_primtive_type("Bool", expression);
}