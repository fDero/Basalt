//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/scope_context.hpp"
#include "errors/preprocessing_errors.hpp"

ScopeContext::ScopeContext(const FunctionDefinition& function_definition) {
    for (const FunctionDefinition::Argument& argument : function_definition.arguments) {
        std::string unique_scope_insensitive_id = function_definition.unique_string_id() + "/ARG:" + argument.arg_name;
        local_objects.push_back({
            .unique_scope_insensitive_id = unique_scope_insensitive_id,
            .identifier = argument.arg_name,
            .type_signature = argument.arg_type,
            .is_const = false,
            .is_arg = true
        });
    }
}

void ScopeContext::store_local_variable(const VariableDeclaration& var_declaration) {
    bool search_outcome = contains(var_declaration.identifier_name);
    ensure_identifier_not_ambiguous_in_given_scope(var_declaration.identifier_name, search_outcome);
    local_objects.push_back({
        .unique_scope_insensitive_id = var_declaration.as_debug_informations_aware_entity().unique_string_id(),
        .identifier = var_declaration.identifier_name,
        .type_signature = var_declaration.typesignature,
        .is_const = false,
        .is_arg = false
    });
}

void ScopeContext::store_local_constant(const ConstDeclaration& const_declaration) {
    bool search_outcome = contains(const_declaration.identifier_name);
    ensure_identifier_not_ambiguous_in_given_scope(const_declaration.identifier_name, search_outcome);
    local_objects.push_back({
        .unique_scope_insensitive_id = const_declaration.as_debug_informations_aware_entity().unique_string_id(),
        .identifier = const_declaration.identifier_name,
        .type_signature = const_declaration.typesignature,
        .is_const = true,
        .is_arg = false
    });
}

bool ScopeContext::contains(const std::string& identifier) {
    MaybeObjectRef retrieved = retrieve(identifier);
    return retrieved.has_value();
}

bool ScopeContext::is_identifier_immutable(const std::string& identifier) {
    MaybeObjectRef retrieved = retrieve(identifier);
    return !retrieved.has_value() || retrieved->get().is_const;
}

TypeSignature& ScopeContext::get_local_object_type(const std::string& identifier) {
    MaybeObjectRef retrieved = retrieve(identifier);
    ensure_parent_scope_exists_for_further_local_object_search(retrieved.has_value(), identifier);
    return retrieved->get().type_signature;
}

std::string ScopeContext::resolve_object_unique_id(const std::string& identifier) {
    MaybeObjectRef retrieved = retrieve(identifier);
    return retrieved->get().unique_scope_insensitive_id;
}

ScopeContext::MaybeObjectRef ScopeContext::retrieve(const std::string& identifier) {
    for (ObjectDescriptor& object : local_objects) {
        if (object.identifier == identifier) {
            return std::ref(object);
        }
    }
    return (parent_scope == nullptr)
        ? std::nullopt
        : parent_scope->retrieve(identifier);
}

ScopeContext ScopeContext::create_nested_scope() {
    ScopeContext nested_scope;
    nested_scope.parent_scope = this;
    return nested_scope;
}