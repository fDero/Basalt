//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/function_definitions_typechecker.hpp"
using FDTC = FunctionDefinitionsTypeChecker;

FunctionDefinitionsTypeChecker::FunctionDefinitionsTypeChecker(
    ProgramRepresentation& program_representation
)
    : program_representation(program_representation)
{}

void FunctionDefinitionsTypeChecker::visit_all_function_definitions() {
    program_representation.foreach_function_definition(
        [this](const FunctionDefinition::Ref& function_definition) {
            visit_function_definition(*function_definition);
        }
    );
}

void FunctionDefinitionsTypeChecker::visit_function_definition(
    const FunctionDefinition& function_definition
) {
    ScopeContext scope_context(function_definition);
    SingleFunctionTypeChecker single_function_type_checker(
        function_definition.return_type,
        scope_context,
        program_representation
    );
    single_function_type_checker.visit_function_definition(function_definition);
}

FDTC::SingleFunctionTypeChecker::SingleFunctionTypeChecker(
    std::optional<TypeSignature> expected_return_type,
    ScopeContext& scope_context,
    ProgramRepresentation& program_representation
)
    : expected_return_type(expected_return_type)
    , scope_context(scope_context)
    , program_representation(program_representation)
{}

void FDTC::SingleFunctionTypeChecker::visit_function_definition(
    const FunctionDefinition& function_definition
) {
    if (function_definition.return_type.has_value()) {
        TypeSignature return_type = function_definition.return_type.value();
        program_representation.verify_that_the_type_exists(return_type);
    }
    for (const FunctionDefinition::Argument& argument : function_definition.arguments) {
        program_representation.verify_that_the_type_exists(argument.arg_type);
    }
    visit_code_block(function_definition.code);
}

void FDTC::SingleFunctionTypeChecker::visit_code_block(const std::vector<Statement>& code_block) {
    for (const Statement& statement : code_block) {
        visit_statement(statement);
    }
}

void FDTC::SingleFunctionTypeChecker::visit_function_call(
    const FunctionCall& fcall,
    const bool should_return_something
) {
    bool encountered_generics = false;
    for (const TypeSignature& concrete_type_parameter : fcall.instantiated_generics) {
        program_representation.verify_that_the_type_exists(concrete_type_parameter);
        encountered_generics |= concrete_type_parameter.is_generic();
    }
    for (const Expression& argument : fcall.arguments) {
        visit_expression(argument);
        auto expression_type = program_representation.resolve_expression_type(argument, scope_context);
        encountered_generics |= !expression_type.has_value() || expression_type.value().is_generic();
    }
    if (encountered_generics) {
        return;
    }
    bool is_void = program_representation.is_void_procedure(fcall, scope_context);
    ensure_return_value_of_function_is_treated_correctly_during_fcall(
        fcall, should_return_something, is_void
    );
}