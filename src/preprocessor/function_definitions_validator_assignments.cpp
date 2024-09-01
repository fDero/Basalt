
#include "preprocessing/function_definition_validator.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "typesystem/assignment_type_checker.hpp"
#include "typesystem/expression_type_deducer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

void FunctionDefinitionValidator::validate_assignment(const Assignment& assignment, ScopeContext& scope_context) {
    const Expression& value = assignment.assigned_value;
    const Expression& target = assignment.assignment_target;
    switch (target.expression_kind()) {
        case ExpressionBody::Kind::identifier:            return validate_assignment_to_identifier(target.get<Identifier>(), value, scope_context);
        case ExpressionBody::Kind::unary_operator:        return validate_assignment_to_unary_operator(target.get<UnaryOperator>(), value, scope_context);
        case ExpressionBody::Kind::dot_member_access:     return validate_assignment_to_dot_member_access(target.get<DotMemberAccess>(), value, scope_context);
        case ExpressionBody::Kind::square_bracket_access: return validate_assignment_to_square_brackets_access(target.get<SquareBracketsAccess>(), value, scope_context);
        default: throw_bad_assignment_target(target);
    }
}

void FunctionDefinitionValidator::validate_assignment_to_identifier(const Identifier& identifier, const Expression& value, ScopeContext& scope_context) {
    const std::string& identifier_name = identifier.name;
    const TypeSignature& target_type = scope_context.get_local_mutable_object_type(identifier_name);
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    TypeSignature value_type = expression_type_deducer.deduce_expression_type(value);
    bool assignment_is_valid = assignment_type_checker.validate_assignment(value_type, target_type);
    ensure_assignment_is_valid(assignment_is_valid, target_type, identifier, value_type, value);
}

void FunctionDefinitionValidator::validate_assignment_to_unary_operator(
    const UnaryOperator& unary_operator, 
    const Expression& value, 
    ScopeContext& scope_context
) {
    auto operator_kind_search_outcome = operator_kinds.find(unary_operator.operator_text);
    ensure_operator_kind_was_found(operator_kind_search_outcome, operator_kinds);
    OperatorKind operator_kind = operator_kind_search_outcome->second;
    switch (operator_kind) {
        case OperatorKind::pointer_dereference_op: 
            return validate_assignment_to_dereference_operator(unary_operator, value, scope_context);
        default: throw_bad_assignment_target(unary_operator);
    }
}

void FunctionDefinitionValidator::validate_assignment_to_dereference_operator(
    const UnaryOperator& unary_operator, 
    const Expression& value, 
    ScopeContext& scope_context
) {
    assert_unary_operator_is(unary_operator, pointer_dereference_operator);
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    const TypeSignature& operand_type = expression_type_deducer.deduce_expression_type(unary_operator.operand);
    ensure_typesignature_is<PointerType>(operand_type);
    const TypeSignature& pointed_type = operand_type.get<PointerType>().pointed_type;
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    TypeSignature value_type = expression_type_deducer.deduce_expression_type(value);
    bool assignment_is_valid = assignment_type_checker.validate_assignment(value_type, pointed_type);
    ensure_assignment_is_valid(assignment_is_valid, pointed_type, unary_operator, value_type, value);
}

void FunctionDefinitionValidator::validate_assignment_to_square_brackets_access(
    const SquareBracketsAccess& square_brackets_access, 
    const Expression& value, 
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    const TypeSignature& left_operand_type = expression_type_deducer.deduce_expression_type(square_brackets_access.storage);
    const TypeSignature* stored_type_ptr = nullptr;
    switch (left_operand_type.typesiganture_kind()) {
        break; case TypeSignatureBody::Kind::array_type: stored_type_ptr = &left_operand_type.get<ArrayType>().stored_type;
        break; case TypeSignatureBody::Kind::slice_type: stored_type_ptr = &left_operand_type.get<SliceType>().stored_type;
        default: throw_bad_assignment_target(square_brackets_access);
    }
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    TypeSignature value_type = expression_type_deducer.deduce_expression_type(value);
    bool assignment_is_valid = assignment_type_checker.validate_assignment(value_type, *stored_type_ptr);
    ensure_assignment_is_valid(assignment_is_valid, *stored_type_ptr, square_brackets_access, value_type, value);
}

void FunctionDefinitionValidator::validate_assignment_to_dot_member_access(
    const DotMemberAccess& dot_member_access, 
    const Expression& value, 
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    TypeSignature struct_type =  expression_type_deducer.deduce_expression_type(dot_member_access.struct_value);
    struct_type = type_definitions_register.unalias_type(struct_type);
    ensure_typesignature_is<CustomType>(struct_type);
    const CustomType& custom_type = struct_type.get<CustomType>();
    TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(custom_type);
    ensure_type_definition_is<StructDefinition>(type_definition);
    const StructDefinition& struct_type_definition = type_definition.get<StructDefinition>();
    const std::string& member_name = dot_member_access.member_name;
    for (const auto& member : struct_type_definition.fields) {
        if (member.field_name == member_name) {
            AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
            TypeSignature value_type = expression_type_deducer.deduce_expression_type(value);
            bool assignment_is_valid =  assignment_type_checker.validate_assignment(value_type, member.field_type);
            ensure_assignment_is_valid(assignment_is_valid, member.field_type, dot_member_access, value_type, value);
            return;
        }
    }
    assert_unreachable();
}