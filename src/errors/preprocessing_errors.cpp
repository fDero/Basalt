
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "toolchain/representation.hpp"
#include <unordered_set>

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name, 
    const std::unordered_set<std::string>& visited_definitions
) {
    auto search_outcome = visited_definitions.find(target_tag_name);
    if (search_outcome != visited_definitions.end()) {
        throw std::runtime_error ("Cyclic dependency!!");
    }
}

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
) {
    if (!(type_definition_insertion_outcome.second)) {
        throw std::runtime_error {
            "Multiple definition of the same type in different files\n"
        };
    }
}

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome
) {
    if (!(insertion_outcome.second)) {
        throw std::runtime_error {
            "Multiple definition of the same type in different packages, both included by the same file\n"
        };
    }
}

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
) {
    throw std::runtime_error {
        "No type definition found for type " + type_signature.type_name + "\n"
    };
}

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved
) {
    if (!retrieved.has_value()) {
        throw std::runtime_error {
            "No type definition found\n"
        };
    }
}

void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier, 
    const bool search_outcome
) {
    if (search_outcome) {
        throw std::runtime_error {
            "Identifier " + identifier + " already exists in the current scope\n"
        };
    }
}

void ensure_parent_scope_exists_for_further_local_object_search(
    const ScopeContext* parent_scope, 
    const std::string& identifier
) {
    if (parent_scope == nullptr) {
        throw std::runtime_error {
            "Cannot find object: " + identifier + "\n"
        };
    }
}

void ensure_object_is_mutable(
    const bool is_const
) {
    if (is_const) {
        throw std::runtime_error {
            "Cannot modify a constant object\n"
        };
    }
}

[[noreturn]] void throw_no_function_definition_found(
    const PrecompiledFunctionCall& function_call
) {
    throw std::runtime_error("no function defintion found!");
}