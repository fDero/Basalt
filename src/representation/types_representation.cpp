
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "language/generics.hpp"

void ProgramRepresentation::store_type_definition(
    const TypeDefinition& type_def, 
    const PackageName& package_name
){
    const std::string match_pattern = get_type_definition_match_pattern(package_name, type_def);
    const auto& insertion_outcome = type_definitions.insert({match_pattern, type_def});
    ensure_no_multiple_definition_of_the_same_type(insertion_outcome);
}


[[nodiscard]] TypeDefinition ProgramRepresentation::retrieve_type_definition(const BaseType& type_signature){
    const std::string& fully_qualified_name = get_fully_quilified_typesignature_name(type_signature);
    return type_definitions.at(fully_qualified_name);
}

std::string ProgramRepresentation::get_fully_quilified_typesignature_name(const BaseType& type_signature){    
    if (!type_signature.package_prefix.empty()){
        const std::string& package = type_signature.package_prefix;
        std::optional<std::string> retrieved = search_fully_qualified_typesignature_name(type_signature, package);
        ensure_type_was_successfully_retrieved(retrieved);
        return retrieved.value();
    }
    const PackageName& target_package_name = package_name_by_file_name.at(type_signature.filename);
    std::optional<std::string> retrieved = search_fully_qualified_typesignature_name(type_signature, target_package_name);
    if (retrieved.has_value()){
        return retrieved.value();
    }
    for (const PackageName& package : imports_by_file.at(type_signature.filename)){
        std::optional<std::string> retrieved = search_fully_qualified_typesignature_name(type_signature, package);
        if (retrieved.has_value()){
            return retrieved.value();
        }
    }
    throw_no_type_definition_found(type_signature);
}

[[nodiscard]] std::optional<std::string> ProgramRepresentation::search_fully_qualified_typesignature_name(
    const BaseType& type_signature, 
    const PackageName& package_name
){
    const std::string instantiated_concrete_type_key = infer_possible_fully_qualified_name(package_name, type_signature);
    auto retrieved = type_definitions.find(instantiated_concrete_type_key);
    if (retrieved != type_definitions.end()){
        return instantiated_concrete_type_key;
    }
    const std::string template_generic_type_key = get_type_signature_match_pattern(package_name, type_signature);
    retrieved = type_definitions.find(template_generic_type_key);
    if (retrieved == type_definitions.end()){
        return std::nullopt;
    }
    else {
        TypeDefinition instantiated = retrieved->second;
        instantiated.instantiate_generics(type_signature);
        type_definitions.insert({instantiated_concrete_type_key, instantiated});
        return instantiated_concrete_type_key;
    }
}

[[nodiscard]] std::string ProgramRepresentation::get_type_definition_match_pattern(
    const PackageName& packageName, 
    const TypeDefinition& type_definition
){
    std::string pattern_tag_name = packageName + namespace_concatenation + type_definition.get_simple_name();
    size_t number_of_generics = type_definition.get_number_of_generics();
    if (number_of_generics > 0) {
        pattern_tag_name += "<";
        for (int i = 0; i < number_of_generics; i++) { 
            pattern_tag_name += "?,";
        }
        pattern_tag_name.back() = '>';
    }
    return pattern_tag_name;
}

[[nodiscard]] std::string ProgramRepresentation::get_type_signature_match_pattern(
    const PackageName& packageName, 
    const BaseType& type_signature
){
    std::string pattern_tag_name = packageName + namespace_concatenation + type_signature.type_name;
    size_t number_of_generics = type_signature.instantiationd_generics.size();
    if (number_of_generics > 0) {
        pattern_tag_name += "<";
        for (int i = 0; i < number_of_generics; i++) { 
            pattern_tag_name += "?,";
        }
        pattern_tag_name.back() = '>';
    }
    return pattern_tag_name;
}

[[nodiscard]] std::string ProgramRepresentation::infer_possible_fully_qualified_name(
    const PackageName& packageName, 
    const TypeSignature& type_signature
){
    if (type_signature.is<SliceType>()) {
        const TypeSignature& pointed_type = type_signature.get<PointerType>().pointed_type;
        return "$" + infer_possible_fully_qualified_name(packageName, pointed_type);
    }
    else if (type_signature.is<PointerType>()) {
        const TypeSignature& pointed_type = type_signature.get<PointerType>().pointed_type;
        return "#" + infer_possible_fully_qualified_name(packageName, pointed_type);
    }
    else if (type_signature.is<ArrayType>()) {
        const TypeSignature& stored_type = type_signature.get<ArrayType>().stored_type;
        const ArrayType& array_type = type_signature.get<ArrayType>();
        std::string array_size_section = "[" + ((array_type.array_length == -1)? "" : std::to_string(array_type.array_length)) + "]";
        return array_size_section + infer_possible_fully_qualified_name(packageName, stored_type);
    }
    else if (type_signature.is<PrimitiveType>()) {
        return type_signature.get<PrimitiveType>().type_name;
    }
    else if (type_signature.is<TemplateType>()) {
        return type_signature.get<TemplateType>().type_name;
    }
    else {
        assert_typesignature_is<BaseType>(type_signature);        
        const BaseType& base_type = type_signature.get<BaseType>();
        std::string non_generic_aware_name = packageName + namespace_concatenation + base_type.type_name;
        if (base_type.instantiationd_generics.empty()){
            return non_generic_aware_name;
        }
        std::string generics_section = "<";
        for (const TypeSignature& generic : base_type.instantiationd_generics){
            if (generic.is<BaseType>()){
                generics_section += get_fully_quilified_typesignature_name(generic.get<BaseType>());
            }
            else {
                generics_section += infer_possible_fully_qualified_name(packageName, generic);
            }
            generics_section += ",";
        }
        generics_section.back() = '>';
        return non_generic_aware_name + generics_section;
    }
}

