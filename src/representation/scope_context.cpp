
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"

void ScopeContext::store_local_variable(const VariableDeclaration& var_declaration){
    bool search_outcome = contains(var_declaration.identifier_name);
    ensure_identifier_not_ambiguous_in_given_scope(var_declaration.identifier_name, search_outcome);
    local_objects.push_back({
        .identifier = var_declaration.identifier_name,
        .type_signature = var_declaration.typesignature,
        .is_const = false,
        .gets_modified = false,
        .gets_accessed = false
    });
}

void ScopeContext::store_local_constant(const ConstDeclaration& const_declaration){
    bool search_outcome = contains(const_declaration.identifier_name);
    ensure_identifier_not_ambiguous_in_given_scope(const_declaration.identifier_name, search_outcome);
    local_objects.push_back({
        .identifier = const_declaration.identifier_name,
        .type_signature = const_declaration.typesignature,
        .is_const = true,
        .gets_modified = false,
        .gets_accessed = false
    });
}

bool ScopeContext::contains(const std::string& identifier) {
    for (const ObjectDescriptor& object : local_objects){
        if (object.identifier == identifier){
            return true;
        }
    }
    return (parent_scope != nullptr) &&
        parent_scope->contains(identifier);
}

TypeSignature& ScopeContext::get_local_object_type(const std::string& identifier) {
    for (ObjectDescriptor& object : local_objects){
        if (object.identifier == identifier){
            object.gets_accessed = true;
            return object.type_signature;
        }
    }
    ensure_parent_scope_exists_for_further_local_object_search(parent_scope, identifier);
    return parent_scope->get_local_object_type(identifier);
}

TypeSignature& ScopeContext::get_local_mutable_object_type(const std::string& identifier) {
    for (ObjectDescriptor& object : local_objects){
        if (object.identifier == identifier){
            object.gets_accessed = true;
            object.gets_modified = true;
            ensure_object_is_mutable(object.is_const);
            return object.type_signature;
        }
    }
    ensure_parent_scope_exists_for_further_local_object_search(parent_scope, identifier);
    return parent_scope->get_local_object_type(identifier);
}

ScopeContext ScopeContext::create_nested_scope() {
    ScopeContext nested_scope;
    nested_scope.parent_scope = this;
    return nested_scope;
}