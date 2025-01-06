//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>

#include "language/definitions.hpp"

class ScopeContext {

    public:
        ScopeContext() = default;
        ScopeContext(const FunctionDefinition& function_definition);

        void store_local_variable(const VariableDeclaration& var_declaration);
        void store_local_constant(const ConstDeclaration& const_declaration);
        
        [[nodiscard]] bool contains(const std::string& identifier);
        [[nodiscard]] bool is_identifier_immutable(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_object_type(const std::string& identifier);
        [[nodiscard]] ScopeContext create_nested_scope();
        [[nodiscard]] std::string resolve_object_unique_id(const std::string& identifier);

    private:
        struct ObjectDescriptor {
            std::string unique_scope_insensitive_id;
            std::string identifier;
            TypeSignature type_signature;
            bool is_const = false; 
            bool is_arg = false;
        };

        using MaybeObjectRef = std::optional<std::reference_wrapper<ScopeContext::ObjectDescriptor>>;
        [[nodiscard]] MaybeObjectRef retrieve(const std::string& identifier);
    
        std::vector<ObjectDescriptor> local_objects;
        ScopeContext* parent_scope = nullptr;
};
