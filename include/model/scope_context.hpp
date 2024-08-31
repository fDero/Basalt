/**
 * @file scope_context.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the ScopeContext class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>

#include "model/program_representation.hpp"
#include "language/definitions.hpp"

/**
 * @brief   Used to keep track of a scope, storing the local variables and constants, and 
 *          providing a way to create nested scopes.
 * 
 * @details The ScopeContext class is used to keep track of a scope, it's used by all the
 *          classes that inspect the body of a function definition as a way to keep track of
 *          the symbols encountered and their state.
 * 
 */
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
