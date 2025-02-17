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
#include "preprocessing/immutability_deducer.hpp"
#include "preprocessing/observability_deducer.hpp"

class AssignmentImmutabilityChecker {

    public:
        AssignmentImmutabilityChecker(
            ScopeContext& scope_context, 
            ProgramRepresentation& program_representation
        );
        
        [[nodiscard]] bool is_expression_assignable_to_var(const Expression& expression);
        [[nodiscard]] bool is_potentially_bonding_expression_assignable_to_var(const Expression& expression);
        [[nodiscard]] bool does_assignment_discard_qualifiers(const Expression& source, const Expression& dest);
        [[nodiscard]] bool does_assignment_discard_qualifiers(const Assignment& assignment);

    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
        BondInspector bond_inspector;
        ImmutabilityDeducer immutability_deducer;
        ObservabilityDeducer observability_deducer;
};