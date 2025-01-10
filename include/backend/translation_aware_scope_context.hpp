//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/scope_context.hpp"
#include "errors/internal_errors.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

struct TranslationAwareScopeContext {
    TranslationAwareScopeContext(
        ScopeContext raw_scope_context,
        std::shared_ptr<std::map<std::string, llvm::AllocaInst*>> local_variables
    ) 
        : raw_scope_context(raw_scope_context) 
        , local_variables(local_variables)
    { }

    TranslationAwareScopeContext create_nested_scope() {
        return TranslationAwareScopeContext(
            this->raw_scope_context.create_nested_scope(),
            this->local_variables
        );
    }

    void store_local_variable(const VariableDeclaration& variable_declaration, llvm::AllocaInst* alloca_inst) {
        raw_scope_context.store_local_variable(variable_declaration);
        std::string llvm_identifier_name = raw_scope_context.resolve_object_unique_id(variable_declaration.identifier_name);
        local_variables->insert({llvm_identifier_name, alloca_inst});
    }

    void store_local_constant(const ConstDeclaration& const_declaration, llvm::AllocaInst* alloca_inst) {
        raw_scope_context.store_local_constant(const_declaration);
        std::string llvm_identifier_name = raw_scope_context.resolve_object_unique_id(const_declaration.identifier_name);
        local_variables->insert({llvm_identifier_name, nullptr});
    }

    [[nodiscard]] llvm::AllocaInst* resolve_object_allocation(const std::string& identifier) {
        std::string unique_id = raw_scope_context.resolve_object_unique_id(identifier);
        auto search_outcome = local_variables->find(unique_id);
        bool search_success = search_outcome != local_variables->end();
        assert_local_variable_was_found_in_translation_aware_scope_context(search_success);
        return search_outcome->second;
    }

    ScopeContext raw_scope_context;
    std::shared_ptr<std::map<std::string, llvm::AllocaInst*>> local_variables = 
        std::make_shared<std::map<std::string, llvm::AllocaInst*>>();
};