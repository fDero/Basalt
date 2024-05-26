
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"

void ProgramRepresentation::store_type_definition(
    const TypeDefinition& type_def, 
    const PackageName& package_name
){
    const std::string match_pattern = package_name + namespace_concatenation + type_def.generate_match_pattern();
    const auto& insertion_outcome = type_definitions.insert({match_pattern, type_def});
    ensure_no_multiple_definition_of_the_same_type(insertion_outcome);
}

TypeDefinition ProgramRepresentation::retrieve_type_definition(const BaseType& type_signature){    
    if (!type_signature.package_prefix.empty()){
        const std::string& package = type_signature.package_prefix;
        std::optional<TypeDefinition> retrieved = search_type_definition_in_package(type_signature, package);
        ensure_type_was_successfully_retrieved(retrieved);
        return retrieved.value();
    }
    const PackageName& target_package_name = package_name_by_file_name.at(type_signature.filename);
    std::optional<TypeDefinition> retrieved = search_type_definition_in_package(type_signature, target_package_name);
    if (retrieved.has_value()){
        return retrieved.value();
    }
    for (const PackageName& package : imports_by_file.at(type_signature.filename)){
        std::optional<TypeDefinition> retrieved = search_type_definition_in_package(type_signature, package);
        if (retrieved.has_value()){
            return retrieved.value();
        }
    }
    throw_no_type_definition_found(type_signature);
}

std::optional<TypeDefinition> ProgramRepresentation::search_type_definition_in_package(
    const BaseType& type_signature, 
    const PackageName& package_name
){
    const std::string instantiated_concrete_type_key = package_name + namespace_concatenation + type_signature.to_string();
    auto retrieved = type_definitions.find(instantiated_concrete_type_key);
    if (retrieved != type_definitions.end()){
        return retrieved->second;
    }
    const std::string template_generic_type_key = package_name + namespace_concatenation + type_signature.to_match_string();
    retrieved = type_definitions.find(template_generic_type_key);
    if (retrieved == type_definitions.end()){
        return std::nullopt;
    }
    else {
        TypeDefinition instantiated = retrieved->second;
        instantiated.instantiate_generics(type_signature);
        type_definitions.insert({instantiated_concrete_type_key, instantiated});
        return instantiated;
    }
}