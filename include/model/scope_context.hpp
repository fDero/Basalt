//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>

#include "model/program_representation.hpp"
#include "language/definitions.hpp"

class ScopeContext {

    public:
        enum class ScopeKind {
            function_scope,
            loop_scope,
            conditional_scope
        };

        ScopeContext(const ScopeKind& scope_kind);
        ScopeContext(const std::vector<FunctionDefinition::Argument>& arguments);

        void store_local_variable(const VariableDeclaration& var_declaration);
        void store_local_constant(const ConstDeclaration& const_declaration);
        
        [[nodiscard]] bool contains(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_object_type(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_mutable_object_type(const std::string& identifier);
        [[nodiscard]] ScopeContext   create_nested_scope(const ScopeKind& scope_kind);
        
        [[nodiscard]] ScopeKind get_scope_kind() const;

    private:
        struct ObjectDescriptor {
            std::string identifier;
            TypeSignature type_signature;
            bool is_const = false; 
            bool is_arg = false;
            bool gets_modified = false;
            bool gets_accessed = false;
        };

        std::vector<ObjectDescriptor> local_objects;
        ScopeContext* parent_scope = nullptr;
        ScopeKind scope_kind;
};
