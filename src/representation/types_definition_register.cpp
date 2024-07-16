
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "language/generics.hpp"

TypeDefinitionsRegister::TypeDefinitionsRegister(ProjectFileStructure& project_file_structure) 
    : project_file_structure(project_file_structure) 
{ 
    for (const auto& [package_name, files] : project_file_structure.get_all_files_grouped_by_package()) {
        for (const auto& file : files) {
            for (const auto& type_definition : file.type_defs) {
                store_type_definition(type_definition, package_name);
            }
        }
    }
}

void TypeDefinitionsRegister::store_type_definition(
    const TypeDefinition& type_def, 
    const std::string& package_name
) {
    const std::string match_pattern = get_type_definition_match_pattern(package_name, type_def);
    const auto& insertion_outcome = type_definitions.insert({match_pattern, type_def});
    ensure_no_multiple_definition_of_the_same_type(insertion_outcome);
}

[[nodiscard]] TypeDefinition TypeDefinitionsRegister::retrieve_type_definition(const CustomType& type_signature) {
    const std::string& fully_qualified_name = get_fully_qualified_customtype_name(type_signature);
    auto search_outcome = type_definitions.find(fully_qualified_name);
    if (search_outcome != type_definitions.end()) {
        return search_outcome->second;
    }
    throw_no_type_definition_found(type_signature);
}

[[nodiscard]] TypeSignature TypeDefinitionsRegister::unalias_type(const TypeSignature& type_signature) {
    if (!type_signature.is<CustomType>()) {
        return type_signature;
    }
    const CustomType& custom_type = type_signature.get<CustomType>();
    TypeDefinition type_definition = retrieve_type_definition(custom_type);
    if (type_definition.is<TypeAlias>()) {
        const TypeAlias& alias = type_definition.get<TypeAlias>();
        return unalias_type(alias.aliased_type);
    }
    else {
        return type_signature;
    }
}

std::string TypeDefinitionsRegister::get_fully_qualified_customtype_name(const CustomType& type_signature) {
    if (!type_signature.package_prefix.empty()) {
        const std::string& package = type_signature.package_prefix;
        std::optional<std::string> retrieved = search_fully_qualified_typesignature_name(type_signature, package);
        ensure_type_was_successfully_retrieved(retrieved);
        return retrieved.value();
    }
    const std::string& target_package_name = project_file_structure.get_package_name_by_file_name(type_signature.filename);
    std::optional<std::string> retrieved = search_fully_qualified_typesignature_name(type_signature, target_package_name);
    if (retrieved.has_value()) {
        return retrieved.value();
    }
    for (const std::string& package : project_file_structure.get_imports_by_file(type_signature.filename)) {
        retrieved = search_fully_qualified_typesignature_name(type_signature, package);
        if (retrieved.has_value()) {
            return retrieved.value();
        }
    }
    throw_no_type_definition_found(type_signature);
}

std::string TypeDefinitionsRegister::get_fully_qualified_typesignature_name(const TypeSignature& type_signature) {
    if (type_signature.is<SliceType>()) {
        const TypeSignature& stored_type = type_signature.get<SliceType>().stored_type;
        return "$" + get_fully_qualified_typesignature_name(stored_type);
    }
    else if (type_signature.is<PointerType>()) {
        const TypeSignature& pointed_type = type_signature.get<PointerType>().pointed_type;
        return "#" + get_fully_qualified_typesignature_name(pointed_type);
    }
    else if (type_signature.is<ArrayType>()) {
        const TypeSignature& stored_type = type_signature.get<ArrayType>().stored_type;
        const ArrayType& array_type = type_signature.get<ArrayType>();
        std::string array_size_section = "[" + ((array_type.array_length == -1)? "" : std::to_string(array_type.array_length)) + "]";
        return array_size_section + get_fully_qualified_typesignature_name(stored_type);
    }
    else if (type_signature.is<PrimitiveType>()) {
        return type_signature.get<PrimitiveType>().type_name;
    }
    else if (type_signature.is<InlineUnion>()) {
        std::string inline_union_fully_qualified_name;
        for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives){
            inline_union_fully_qualified_name += get_fully_qualified_typesignature_name(alternative) + " | ";
        }
        inline_union_fully_qualified_name.pop_back();
        inline_union_fully_qualified_name.pop_back();
        inline_union_fully_qualified_name.pop_back();
        return inline_union_fully_qualified_name;
    }
    else if (type_signature.is<TemplateType>()) {
        return type_signature.get<TemplateType>().type_name;
    }
    else {
        assert_typesignature_is<CustomType>(type_signature);        
        const CustomType& custom_type = type_signature.get<CustomType>();
        return get_fully_qualified_customtype_name(custom_type);
    }
}

[[nodiscard]] std::optional<std::string> TypeDefinitionsRegister::search_fully_qualified_typesignature_name(
    const CustomType& type_signature, 
    const std::string& package_name
) {
    std::string instantiated_concrete_type_key = infer_possible_fully_qualified_typesignature_name(package_name, type_signature);
    auto retrieved = type_definitions.find(instantiated_concrete_type_key);
    if (retrieved != type_definitions.end()) {
        return instantiated_concrete_type_key;
    }
    const std::string template_generic_type_key = get_type_signature_match_pattern(package_name, type_signature);
    retrieved = type_definitions.find(template_generic_type_key);
    if (retrieved == type_definitions.end()) {
        return std::nullopt;
    }
    else {
        TypeDefinition& to_be_instantiated = retrieved->second;
        GenericSubstitutionRuleSet rules = GenericSubstitutionRuleSet::zip_components_vectors(
            to_be_instantiated.get_template_generics(), 
            type_signature.type_parameters
        );
        GenericsInstantiationEngine generic_instantiation_engine(rules);
        TypeDefinition instantiated = generic_instantiation_engine.instantiate_generic_typedefinition(to_be_instantiated);
        instantiated.set_name(instantiated_concrete_type_key);
        type_definitions.insert({instantiated_concrete_type_key, instantiated});
        return instantiated_concrete_type_key;
    }
}

[[nodiscard]] std::string TypeDefinitionsRegister::get_type_definition_match_pattern(
    const std::string& package_name,
    const TypeDefinition& type_definition
) {
    std::string pattern_tag_name = package_name + namespace_concatenation + type_definition.get_simple_name();
    size_t number_of_generics = type_definition.get_number_of_generics();
    std::string generics_section = (number_of_generics > 0)? "<" + std::to_string(number_of_generics) + ">" : "";
    return pattern_tag_name + generics_section;
}

[[nodiscard]] std::string TypeDefinitionsRegister::get_type_signature_match_pattern(
    const std::string& package_name,
    const CustomType& type_signature
) {
    std::string pattern_tag_name = package_name + namespace_concatenation + type_signature.type_name;
    size_t number_of_generics = type_signature.type_parameters.size();
    std::string generics_section = (number_of_generics > 0)? "<" + std::to_string(number_of_generics) + ">" : "";
    return pattern_tag_name + generics_section;
}

[[nodiscard]] std::string TypeDefinitionsRegister::infer_possible_fully_qualified_typesignature_name(
    const std::string& package_name,
    const TypeSignature& type_signature
) {
    if (type_signature.is<SliceType>()) {
        const TypeSignature& stored_type = type_signature.get<SliceType>().stored_type;
        return "$" + infer_possible_fully_qualified_typesignature_name(package_name, stored_type);
    }
    else if (type_signature.is<PointerType>()) {
        const TypeSignature& pointed_type = type_signature.get<PointerType>().pointed_type;
        return "#" + infer_possible_fully_qualified_typesignature_name(package_name, pointed_type);
    }
    else if (type_signature.is<ArrayType>()) {
        const TypeSignature& stored_type = type_signature.get<ArrayType>().stored_type;
        const ArrayType& array_type = type_signature.get<ArrayType>();
        std::string array_size_section = "[" + ((array_type.array_length == -1)? "" : std::to_string(array_type.array_length)) + "]";
        return array_size_section + infer_possible_fully_qualified_typesignature_name(package_name, stored_type);
    }
    else if (type_signature.is<InlineUnion>()) {
        std::string inline_union_fully_qualified_name;
        for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives){
            inline_union_fully_qualified_name += infer_possible_fully_qualified_typesignature_name(package_name, alternative) + " | ";
        }
        inline_union_fully_qualified_name.pop_back();
        inline_union_fully_qualified_name.pop_back();
        inline_union_fully_qualified_name.pop_back();
        return inline_union_fully_qualified_name;
    }
    else if (type_signature.is<PrimitiveType>()) {
        return type_signature.get<PrimitiveType>().type_name;
    }
    else if (type_signature.is<TemplateType>()) {
        return type_signature.get<TemplateType>().type_name;
    }
    else {
        assert_typesignature_is<CustomType>(type_signature);        
        const CustomType& custom_type = type_signature.get<CustomType>();
        return infer_possible_fully_qualified_customtype_name(package_name, custom_type);
    }
}

[[nodiscard]] std::string TypeDefinitionsRegister::infer_possible_fully_qualified_customtype_name(
    const std::string& packageName, 
    const CustomType& custom_type
) {
    std::string non_generic_aware_name = packageName + namespace_concatenation + custom_type.type_name;
    if (custom_type.type_parameters.empty()) {
        return non_generic_aware_name;
    }
    std::string generics_section = "<";
    for (const TypeSignature& generic : custom_type.type_parameters) {
        if (generic.is<CustomType>()) {
            generics_section += get_fully_qualified_customtype_name(generic.get<CustomType>());
        }
        else {
            generics_section += infer_possible_fully_qualified_typesignature_name(packageName, generic);
        }
        generics_section += ",";
    }
    generics_section.back() = '>';
    return non_generic_aware_name + generics_section;
}
