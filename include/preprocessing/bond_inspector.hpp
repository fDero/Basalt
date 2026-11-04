//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "misc/forward_declarations.hpp"


class BondInspector {

    public:
        BondInspector(
            ScopeContext& scope_context,
            ProgramRepresentation& program_representation
        );

        [[nodiscard]] bool does_the_type_of_this_expr_imply_a_bond(const TypeSignature& expression_type);
        [[nodiscard]] bool does_this_inline_union_imply_a_bond(const InlineUnion& inline_union);
        [[nodiscard]] bool does_this_custom_type_imply_a_bond(const CustomType& custom_type);
        [[nodiscard]] bool does_this_struct_imply_a_bond(const StructDefinition& struct_def);
        [[nodiscard]] bool does_this_union_imply_a_bond(const UnionDefinition& union_def);
        [[nodiscard]] bool does_this_type_alias_imply_a_bond(const TypeAlias& type_alias);

    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
};
