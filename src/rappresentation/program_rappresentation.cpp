
#include "toolchain/rappresentation.hpp"
#include "errors/preprocessing_errors.hpp"

TypeDefinition ProgramRappresentation::retrieve_type_definition(const BaseType& type_signature){
    if (!type_signature.package_prefix.empty()){
        const std::string& package = type_signature.package_prefix;
        TypeDefinition type_definition = retrieve_type_definition_from_package(type_signature, package);
        type_definition.instantiate_generics(type_signature);
        return type_definition;
    }
    const PackageName& target_package_name = package_name_by_file_name.at(type_signature.filename);
    std::optional<TypeDefinition> retrieved = attempt_to_retrieve_type_definition_from_package(type_signature, target_package_name);
    if (retrieved.has_value()){
        TypeDefinition type_definition = retrieved.value();
        type_definition.instantiate_generics(type_signature);
        return type_definition;
    }
    for (const PackageName& package : imports_by_file.at(type_signature.filename)){
        std::optional<TypeDefinition> retrieved = attempt_to_retrieve_type_definition_from_package(type_signature, package);
        if (retrieved.has_value()){
            TypeDefinition type_definition = retrieved.value();
            type_definition.instantiate_generics(type_signature);
            return type_definition;
        }
    }
    throw_no_type_definition_found(type_signature);
}

void ProgramRappresentation::store_definitions_from_file(const FileRappresentation& file_rappresentation){
    const std::string& package_name = file_rappresentation.file_metadata.packagename;
    files_by_package[package_name].push_back(file_rappresentation);
    package_name_by_file_name[file_rappresentation.file_metadata.filename] = package_name;
    for (const TypeDefinition& type_def : file_rappresentation.type_defs){
        store_type_definition(type_def, package_name);
    }
    const std::string& filename = file_rappresentation.file_metadata.filename;
    const std::vector<PackageName>& imports = file_rappresentation.file_metadata.imports;
    imports_by_file[filename] = imports;
}

void ProgramRappresentation::store_type_definition(
    const TypeDefinition& type_def, 
    const PackageName& package_name
){
    const std::string match_pattern = type_def.generate_match_pattern();
    PackageTypeDefinitions& definitions = types_by_package[package_name];
    const auto& insertion_outcome = definitions.insert({match_pattern, type_def});
    ensure_no_multiple_definition_of_the_same_type(insertion_outcome);
}

std::optional<TypeDefinition> ProgramRappresentation::attempt_to_retrieve_type_definition_from_package(
    const BaseType& type_signature, 
    const PackageName& package_name
){
    PackageTypeDefinitions& definitions = types_by_package[package_name];
    const std::string instantiated_concrete_type_key = type_signature.to_string();
    auto retrieved = definitions.find(instantiated_concrete_type_key);
    if (retrieved != definitions.end()){
        return retrieved->second;
    }
    const std::string template_generic_type_key = type_signature.to_match_string();
    retrieved = definitions.find(template_generic_type_key);
    return (retrieved != definitions.end()) 
        ? std::optional<TypeDefinition>(retrieved->second) 
        : std::nullopt;
}

TypeDefinition ProgramRappresentation::retrieve_type_definition_from_package(
    const BaseType& type_signature, 
    const PackageName& package
){
    std::optional<TypeDefinition> retrieved = 
        attempt_to_retrieve_type_definition_from_package(type_signature, package);
    ensure_type_was_successfully_retrieved(retrieved);
    return retrieved.value();
}
