//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "preprocessing/immutability_checker.hpp"
#include "preprocessing/bond_inspector.hpp"
#include "errors/preprocessing_errors.hpp"

class FunctionDefinitionsTypeChecker {

    public:
        FunctionDefinitionsTypeChecker(ProgramRepresentation& program_representation);

        void visit_all_function_definitions();
        void visit_function_definition(const FunctionDefinition& function_definition);

    protected:
        struct SingleFunctionTypeChecker {
            SingleFunctionTypeChecker(
                ScopeContext& scope_context,
                ProgramRepresentation& program_representation
            );

            void visit_function_definition(const FunctionDefinition& function_definition);

            ScopeContext& scope_context;
            ProgramRepresentation& program_representation;
        };

    private:
        ProgramRepresentation& program_representation;
};