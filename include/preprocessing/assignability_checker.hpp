//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "errors/internal_errors.hpp"

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "language/definitions.hpp"
#include "language/statements.hpp"
#include "language/functions.hpp"
#include "errors/preprocessing_errors.hpp"
#include "preprocessing/bond_inspector.hpp"
#include "preprocessing/immutability_checker.hpp"

class AssignabilityChecker {

    public:
        AssignabilityChecker(
            ScopeContext& scope_context, 
            ProgramRepresentation& program_representation,
            BondInspector& bond_inspector,
            ImmutabilityChecker& immutability_checker
        );
        
        [[nodiscard]] bool is_expression_assignable_to_var(const Expression& expression);
        [[nodiscard]] bool is_potentially_bonding_expression_assignable_to_var(const Expression& expression);
        
    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
        BondInspector& bond_inspector;
        ImmutabilityChecker immutability_checker;
};