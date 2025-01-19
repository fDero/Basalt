//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/preprocessing_errors.hpp"
#include "preprocessing/function_definitions_typechecker.hpp"
using FDTC = FunctionDefinitionsTypeChecker;

void FDTC::SingleFunctionTypeChecker::visit_expression(const Expression& expr) {
    switch (expr.expression_kind()) {
        break; case ExpressionBody::Kind::function_call:   visit_function_call(expr.get<FunctionCall>(), true);
        break; case ExpressionBody::Kind::type_operator:   visit_type_operator(expr.get<TypeOperator>());    
        break; case ExpressionBody::Kind::binary_operator: visit_binary_operator(expr.get<BinaryOperator>());
        break; case ExpressionBody::Kind::unary_operator:  visit_unary_operator(expr.get<UnaryOperator>());
        break; case ExpressionBody::Kind::array_literal:   visit_array_literal(expr.get<ArrayLiteral>());
        break; case ExpressionBody::Kind::dot_member_access:
            visit_dot_member_access(expr.get<DotMemberAccess>());
        break; case ExpressionBody::Kind::square_bracket_access:
            visit_square_bracket_access(expr.get<SquareBracketsAccess>());
        break; case ExpressionBody::Kind::identifier:      return;
        break; case ExpressionBody::Kind::string_literal:  return;
        break; case ExpressionBody::Kind::float_literal:   return;
        break; case ExpressionBody::Kind::char_literal:    return;
        break; case ExpressionBody::Kind::int_literal:     return;
        break; case ExpressionBody::Kind::bool_literal:    return;
    }
}

void FDTC::SingleFunctionTypeChecker::visit_dot_member_access(
    const DotMemberAccess& dot_member_access
) {
    visit_expression(dot_member_access.struct_value);
    auto struct_type = program_representation.resolve_expression_type(
        dot_member_access.struct_value, 
        scope_context
    );
    if (!struct_type.has_value()){
        return;
    }
    while (struct_type->typesiganture_kind() == TypeSignatureBody::Kind::pointer_type) {
        struct_type = struct_type->get<PointerType>().pointed_type;
    }
    if (struct_type->typesiganture_kind() == TypeSignatureBody::Kind::custom_type) {
        auto custom_type = struct_type->get<CustomType>();
        auto type_def = program_representation.retrieve_type_definition(custom_type);
        ensure_type_definition_is<StructDefinition>(type_def);
        auto struct_def = type_def.get<StructDefinition>();
        expect_struct_field(struct_def, dot_member_access);
    }
}

void FDTC::SingleFunctionTypeChecker::expect_struct_field(
    const StructDefinition& struct_def,
    const DotMemberAccess& dot_member_access
){
    for (const auto& field : struct_def.fields) {
        if (field.field_name == dot_member_access.member_name) {
            return;
        }
    }
    throw_no_such_struct_field(
        dot_member_access.member_name, 
        struct_def, 
        dot_member_access
    );
}

void FDTC::SingleFunctionTypeChecker::visit_square_bracket_access(
    const SquareBracketsAccess& square_brackets_access
) {
    visit_expression(square_brackets_access.storage);
    visit_expression(square_brackets_access.index);
    auto storage_type = program_representation.resolve_expression_type(
        square_brackets_access.storage, 
        scope_context
    );
    auto index_type = program_representation.resolve_expression_type(
        square_brackets_access.index, 
        scope_context
    );
    if (!index_type.has_value() || !storage_type.has_value()) {
        return;
    }
    ensure_typesignature_is_either_array_or_slice_for_square_brackets_access(*storage_type);
    ensure_typesignature_is_int(*index_type);
}

void FDTC::SingleFunctionTypeChecker::visit_array_literal(
    const ArrayLiteral& array_literal
) {
    ensure_array_literal_contains_the_right_number_of_elements(array_literal);
    for (const auto& element : array_literal.elements) {
        visit_expression(element);
        auto element_type = program_representation.resolve_expression_type(
            element, 
            scope_context
        );
        bool element_can_be_analyzed = element_type.has_value();
        element_can_be_analyzed &= !element_type->is_generic(); 
        element_can_be_analyzed &= !array_literal.stored_type.is_generic();
        bool element_is_compatible = element_can_be_analyzed;
        element_is_compatible &= program_representation.validate_assignment(
            *element_type,
            array_literal.stored_type 
        );
        ensure_element_is_compatible_with_array_literal(
            element_can_be_analyzed,
            element_is_compatible, 
            array_literal, 
            element
        );
    }
}

void FDTC::SingleFunctionTypeChecker::visit_type_operator(const TypeOperator& type_operator) {
    visit_expression(type_operator.expression);
    auto expression_type = program_representation.resolve_expression_type(type_operator.expression, scope_context);
    if (!expression_type.has_value()) {
        return;
    }
    bool type_operator_typesignature_is_union = program_representation.is_union(*expression_type);
    ensure_type_is_union_for_type_operator(type_operator_typesignature_is_union);
    bool can_assign = program_representation.validate_assignment(type_operator.typesignature, expression_type);
    ensure_type_operator_union_operand_can_be_queried_for_the_given_type(can_assign);
}

void FDTC::SingleFunctionTypeChecker::visit_binary_operator(const BinaryOperator& binary_operator) {
    visit_expression(binary_operator.left_operand);
    visit_expression(binary_operator.right_operand);
}

void FDTC::SingleFunctionTypeChecker::visit_unary_operator(const UnaryOperator& unary_operator) {
    visit_expression(unary_operator.operand);
}