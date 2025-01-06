//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/address_sanitizer.hpp"
using AS = AddressSanitizer;

AddressSanitizer::AddressSanitizer(
    ProgramRepresentation& program_representation
)
    : program_representation(program_representation)
{}

void AddressSanitizer::visit_all_function_definitions() {
    program_representation.foreach_function_definition(
        [this](const FunctionDefinition::Ref& function_definition) {
            visit_function_definition(*function_definition);
        }
    );
}

void AddressSanitizer::visit_function_definition(
    const FunctionDefinition& function_definition
) {
    ScopeContext scope_context(function_definition);
    SingleFunctionAddressSanitizer single_function_address_sanitizer(
        scope_context,
        program_representation
    );
    single_function_address_sanitizer.visit_function_definition(function_definition);
}

AS::SingleFunctionAddressSanitizer::SingleFunctionAddressSanitizer(
    ScopeContext& scope_context,
    ProgramRepresentation& program_representation
)
    : scope_context(scope_context)
    , program_representation(program_representation)
{}

void AS::SingleFunctionAddressSanitizer::visit_function_definition(
    const FunctionDefinition& function_definition
) {
    visit_code_block(function_definition.code);
}

void AS::SingleFunctionAddressSanitizer::visit_code_block(const std::vector<Statement>& code_block) {
    for (const Statement& statement : code_block) {
        visit_statement(statement);
    }
}

void AS::SingleFunctionAddressSanitizer::visit_function_call(
    const FunctionCall& fcall
) {
    for (const Expression& argument : fcall.arguments) {
        visit_expression(argument);
    }
}