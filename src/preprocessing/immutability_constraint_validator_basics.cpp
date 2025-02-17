//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/immutability_constraint_validator.hpp"
using CCV = ImmutabilityConstraintValidator;

ImmutabilityConstraintValidator::ImmutabilityConstraintValidator(ProgramRepresentation& program_representation)
    : program_representation(program_representation) {}

void ImmutabilityConstraintValidator::visit_all_function_definitions() {
    program_representation.foreach_function_definition([&](const FunctionDefinition::Ref& function_definition) {
        visit_function_definition(*function_definition);
    });
}

void ImmutabilityConstraintValidator::visit_function_definition(const FunctionDefinition& function_definition) {
    ScopeContext scope_context(function_definition);
    ImmutabilityDeducer immutability_deducer(scope_context, program_representation);
    BondInspector bond_inspector(scope_context, program_representation);
    SingleFunctionImmutabilityConstraintValidator single_function_immutability_constraint_validator(
        scope_context,
        program_representation,
        immutability_deducer,
        bond_inspector
    );
    single_function_immutability_constraint_validator.visit_function_definition(function_definition);
}

CCV::SingleFunctionImmutabilityConstraintValidator::SingleFunctionImmutabilityConstraintValidator(
    ScopeContext& scope_context,
    ProgramRepresentation& program_representation,
    ImmutabilityDeducer& immutability_deducer,
    BondInspector& bond_inspector
)
    : scope_context(scope_context) 
    , program_representation(program_representation)
    , immutability_deducer(immutability_deducer)
    , bond_inspector(bond_inspector)
{}


void CCV::SingleFunctionImmutabilityConstraintValidator::visit_function_definition(const FunctionDefinition& function_definition) {
    for (const Statement& statement : function_definition.code) {
        visit_statement(statement, scope_context);
    }
}