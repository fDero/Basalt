//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"
#include "misc/debug_informations_aware_entity.hpp"


static const DebugInformationsAwareEntity& location_of(const TypeDefinition& type_definition) {
    if (type_definition.is<StructDefinition>()) {
        return type_definition.get<StructDefinition>().as_debug_informations_aware_entity();
    }
    if (type_definition.is<UnionDefinition>()) {
        return type_definition.get<UnionDefinition>().as_debug_informations_aware_entity();
    }
    return type_definition.get<TypeAlias>().as_debug_informations_aware_entity();
}

static std::string describe_custom_type(const CustomType& custom_type) {
    return backticked(TypeSignature(custom_type).to_source_string());
}

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& insertion_outcome,
    const TypeDefinition& type_definition
) {
    if (insertion_outcome.second) {
        return;
    }
    const TypeDefinition& previous_definition = insertion_outcome.first->second;
    throw IndexingError(
        "type " + backticked(type_definition.get_simple_name()) +
        " is defined more than once in the same package",
        location_of(type_definition), "previously defined at " + describe_location(
            location_of(previous_definition)
        )
    );
}

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome,
    const TypeDefinition& type_definition
) {
    if (!insertion_outcome.second) {
        throw IndexingError(
            "type " + backticked(type_definition.get_simple_name()) +
            " conflicts with another type of the same name that is visible from the same file",
            location_of(type_definition),
            "a type with this name (and the same number of generics) is also defined "
            "in this package or in one of the packages imported alongside it"
        );
    }
}

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved,
    const CustomType& type_signature
) {
    if (!retrieved.has_value()) {
        throw IndexingError(
            "unknown type " + describe_custom_type(type_signature) + ": package " +
            backticked(type_signature.package_prefix) + " defines no type named " +
            backticked(type_signature.type_name),
            type_signature.as_debug_informations_aware_entity()
        );
    }
}

void throw_no_type_definition_found(const CustomType& type_signature) {
    throw IndexingError(
        "unknown type " + describe_custom_type(type_signature),
        type_signature.as_debug_informations_aware_entity(),
        "make sure the package that defines it is imported, "
        "and that its files are passed to the compiler with `-i`"
    );
}

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name,
    const std::unordered_set<std::string>& visited_definitions,
    const std::string& type_name,
    const DebugInformationsAwareEntity& type_definition
) {
    if (visited_definitions.find(target_tag_name) != visited_definitions.end()) {
        throw ValidationError(
            "type " + backticked(type_name) +
            " contains itself (directly or through other types), "
            "so it would have infinite size", type_definition,
            "store a pointer (`#" + type_name + "`) or a slice (`$" +
            type_name + "`) instead, to break the cycle"
        );
    }
}
