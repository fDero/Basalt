//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "function_definitions_typechecker.hpp"
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
    ScopeContext scope_context(function_definition.arguments);
    SingleFunctionTypeChecker single_function_type_checker(
        scope_context,
        program_representation
    );
    single_function_type_checker.visit_function_definition(function_definition);
}

FDTC::SingleFunctionTypeChecker::SingleFunctionTypeChecker(
    ScopeContext& scope_context,
    ProgramRepresentation& program_representation
)
    : scope_context(scope_context)
    , program_representation(program_representation)
{}

void FDTC::SingleFunctionTypeChecker::visit_function_definition(
    const FunctionDefinition& function_definition
) {
    // temporary stub
}