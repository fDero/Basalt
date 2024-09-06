
#include "preprocessing/function_definition_validator.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "typesystem/assignment_type_checker.hpp"
#include "typesystem/expression_type_deducer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

FunctionDefinitionValidator::FunctionDefinitionValidator(
    ProjectFileStructure& project_file_structure,
    TypeDefinitionsRegister& type_definitions_register,
    FunctionOverloadsRegister& function_overloads_register,
    OverloadingResolutionEngine& overloading_resolution_engine
) 
    : project_file_structure(project_file_structure)
    , type_definitions_register(type_definitions_register)
    , function_overloads_register(function_overloads_register)
    , overloading_resolution_engine(overloading_resolution_engine) 
{}

void FunctionDefinitionValidator::validate_function_definition(const FunctionDefinition& function_definition) {
    for (const auto& argument : function_definition.arguments) {
        type_definitions_register.verify_that_the_type_exists(argument.arg_type);
    }
    ScopeContext scope_context(function_definition.arguments);
    for (const auto& statement : function_definition.code) {
        validate_function_body_statement(statement, function_definition, scope_context);
    }
}

void FunctionDefinitionValidator::validate_function_body_statement(
    const Statement& statement, 
    const FunctionDefinition& function_definition,
    ScopeContext& scope_context
) {
    switch (statement.statement_kind()) {
        case StatementBody::Kind::assignment:           return validate_assignment(statement.get<Assignment>(), scope_context);
        case StatementBody::Kind::break_statement:      return ensure_in_loop_scope(statement, scope_context);
        case StatementBody::Kind::continue_statement:   return ensure_in_loop_scope(statement, scope_context);
        case StatementBody::Kind::return_statement:     return validate_return_statement(statement.get<Return>(), function_definition, scope_context);
        case StatementBody::Kind::conditional:          return validate_conditional(statement.get<Conditional>(), function_definition, scope_context);
        case StatementBody::Kind::const_declaration:    return validate_const_declaratuion(statement.get<ConstDeclaration>(), scope_context);
        case StatementBody::Kind::variable_declaration: return validate_variable_declaration(statement.get<VariableDeclaration>(), scope_context);
        case StatementBody::Kind::function_call:        return validate_function_call(statement.get<FunctionCall>(), scope_context);
        case StatementBody::Kind::until_loop:           return validate_until_loop(statement.get<UntilLoop>(), function_definition, scope_context);
        case StatementBody::Kind::while_loop:           return validate_while_loop(statement.get<WhileLoop>(), function_definition, scope_context);
    }
    assert_unreachable();
}

void FunctionDefinitionValidator::validate_return_statement(
    const Return& return_statement, 
    const FunctionDefinition& function_definition,
    ScopeContext& scope_context
) {
    bool is_returning_function = function_definition.return_type.has_value();
    bool return_statement_has_value = return_statement.return_value.has_value();
    ensure_return_statement_is_congruent_to_function_definition(is_returning_function, return_statement_has_value);
    if (!is_returning_function){
        return;
    }
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    std::optional<TypeSignature> returned_value_type_opt = expression_type_deducer.deduce_expression_type(*return_statement.return_value);
    if (!returned_value_type_opt.has_value()) {
        return;
    }
    const TypeSignature& returned_value_type = returned_value_type_opt.value();
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    bool assignment_is_valid = assignment_type_checker.validate_assignment(returned_value_type, *function_definition.return_type);
    ensure_return_value_type_is_congruent_to_function_definition(assignment_is_valid, returned_value_type, function_definition, return_statement);
}

void FunctionDefinitionValidator::validate_conditional(
    const Conditional& conditional, 
    const FunctionDefinition& function_definition,
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    std::optional<TypeSignature> condition_type_opt = expression_type_deducer.deduce_expression_type(conditional.condition);
    if (!condition_type_opt.has_value()) {
        const TypeSignature& condition_type = condition_type_opt.value();
        ensure_typesignature_is_boolean(condition_type);
    }
    ScopeContext then_scope = scope_context.create_nested_scope(ScopeContext::ScopeKind::conditional_scope);
    ScopeContext else_scope = scope_context.create_nested_scope(ScopeContext::ScopeKind::conditional_scope);
    for (const auto& statement : conditional.then_brench) {
        validate_function_body_statement(statement, function_definition, then_scope);
    }
    for (const auto& statement : conditional.else_brench) {
        validate_function_body_statement(statement, function_definition, else_scope);
    }
}

void FunctionDefinitionValidator::validate_while_loop(
    const WhileLoop& while_loop, 
    const FunctionDefinition& function_definition,
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    std::optional<TypeSignature> condition_type_opt = expression_type_deducer.deduce_expression_type(while_loop.condition);
    if (!condition_type_opt.has_value()) {
        const TypeSignature& condition_type = condition_type_opt.value();
        ensure_typesignature_is_boolean(condition_type);
    }
    ScopeContext loop_scope = scope_context.create_nested_scope(ScopeContext::ScopeKind::loop_scope);
    for (const auto& statement : while_loop.loop_body) {
        validate_function_body_statement(statement, function_definition, loop_scope);
    }
}

void FunctionDefinitionValidator::validate_until_loop(
    const UntilLoop& until_loop, 
    const FunctionDefinition& function_definition,
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    std::optional<TypeSignature> condition_type_opt = expression_type_deducer.deduce_expression_type(until_loop.condition);
    if (!condition_type_opt.has_value()) {
        const TypeSignature& condition_type = condition_type_opt.value();
        ensure_typesignature_is_boolean(condition_type);
    }
    ScopeContext loop_scope = scope_context.create_nested_scope(ScopeContext::ScopeKind::loop_scope);
    for (const auto& statement : until_loop.loop_body) {
        validate_function_body_statement(statement, function_definition, loop_scope);
    }
}

void FunctionDefinitionValidator::validate_function_call(
    const FunctionCall& function_call,
    ScopeContext& scope_context
) {
    OverloadingResolutionEngine overloading_resolution_engine(function_overloads_register, type_definitions_register, project_file_structure);
    ExpressionTypeDeducer expression_type_deducer(type_definitions_register, overloading_resolution_engine, project_file_structure, scope_context);
    const std::vector<Expression>& arguments = function_call.arguments;
    std::vector<TypeSignature> arguments_types;
    for (const Expression& argument : arguments) {
        std::optional<TypeSignature> argument_type_opt = expression_type_deducer.deduce_expression_type(argument);
        if (!argument_type_opt.has_value() || argument_type_opt.value().is<TemplateType>()) {
            return;
        }
        const TypeSignature& argument_type = argument_type_opt.value();
        arguments_types.push_back(argument_type);
    }
    FunctionDefinition::Ref retrieved = overloading_resolution_engine.retrieve_function_definition(function_call, arguments_types);
    ensure_function_overload_was_successfully_retrieved(function_call, retrieved);
}

void FunctionDefinitionValidator::validate_variable_declaration(
    const VariableDeclaration& variable_declaration,
    ScopeContext& scope_context
) {
    type_definitions_register.verify_that_the_type_exists(variable_declaration.typesignature);
    scope_context.store_local_variable(variable_declaration);
}

void FunctionDefinitionValidator::validate_const_declaratuion(
    const ConstDeclaration& const_declaration,
    ScopeContext& scope_context
) {
    type_definitions_register.verify_that_the_type_exists(const_declaration.typesignature);
    scope_context.store_local_constant(const_declaration);
}