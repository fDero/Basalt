//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/const_constraint_validator.hpp"
using CCV = ConstConstraintValidator;

ConstConstraintValidator::ConstConstraintValidator(ProgramRepresentation& program_representation)
    : program_representation(program_representation) {}

void ConstConstraintValidator::visit_all_function_definitions() {
    program_representation.foreach_function_definition([&](const FunctionDefinition::Ref& function_definition) {
        visit_function_definition(*function_definition);
    });
}

void ConstConstraintValidator::visit_function_definition(const FunctionDefinition& function_definition) {
    ScopeContext scope_context(function_definition);
    ImmutabilityChecker immutability_checker(scope_context, program_representation);
    BondInspector bond_inspector(scope_context, program_representation);
    SingleFunctionConstConstraintValidator single_function_const_constraint_validator(
        scope_context,
        program_representation,
        immutability_checker,
        bond_inspector
    );
    single_function_const_constraint_validator.visit_function_definition(function_definition);
}

CCV::SingleFunctionConstConstraintValidator::SingleFunctionConstConstraintValidator(
    ScopeContext& scope_context,
    ProgramRepresentation& program_representation,
    ImmutabilityChecker& immutability_checker,
    BondInspector& bond_inspector
)
    : scope_context(scope_context) 
    , program_representation(program_representation)
    , immutability_checker(immutability_checker)
    , bond_inspector(bond_inspector)
{}


void CCV::SingleFunctionConstConstraintValidator::visit_function_definition(const FunctionDefinition& function_definition) {
    for (const Statement& statement : function_definition.code) {
        visit_statement(statement, scope_context);
    }
}