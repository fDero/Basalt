//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "core/program_representation.hpp"
#include "language/definitions.hpp"
#include "language/statements.hpp"
#include "language/functions.hpp"
#include "errors/preprocessing_errors.hpp"

class FunctionExitPathNavigator {

    public:
        FunctionExitPathNavigator(ProgramRepresentation& program_representation);

        void visit_all_function_definitions();
        void visit_function_definition(const FunctionDefinition& function_definition);

    private:
        enum class ScopeKind {
            function,
            loop,
            conditional
        };

        [[nodiscard]] bool visit_code_block(const std::vector<Statement>& code_block, ScopeKind scope_kind);
        [[nodiscard]] bool visit_statement(const Statement& statement, ScopeKind scope_kind);
        [[nodiscard]] bool visit_loop_blocking_statement(ScopeKind scope_kind);
        [[nodiscard]] bool visit_if_statement(const Conditional& if_statement);
        [[nodiscard]] bool visit_while_loop(const WhileLoop& while_loop);
        [[nodiscard]] bool visit_until_loop(const UntilLoop& until_loop);

        ProgramRepresentation& program_representation;
};