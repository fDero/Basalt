//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "model/type_definitions_register.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "typesystem/generics_substitution_rules.hpp"

TypeDefinitionsRegister::TypeDefinitionsRegister(ProjectFileStructure& project_file_structure) 
    : project_file_structure(project_file_structure) 
{ 
    project_file_structure.foreach_file([&](const FileRepresentation& file_representation) {
        for (const auto& type_definition : file_representation.type_defs) {
            store_type_definition(type_definition);
        }
    });
}

void TypeDefinitionsRegister::foreach_type_definition(
    std::function<void(const TypeDefinition&)> visitor
) {
    for (const std::string& type_definition_id : type_definitions_ids) {
        const TypeDefinition& type_definition = type_definitions.at(type_definition_id);
        visitor(type_definition);
    }
}

void TypeDefinitionsRegister::verify_that_the_type_exists(const TypeSignature& type_signature) {
    switch (type_signature.typesiganture_kind()) {
        break; case TypeSignatureBody::Kind::pointer_type:   verify_that_the_type_exists(type_signature.get<PointerType>().pointed_type);
        break; case TypeSignatureBody::Kind::array_type:     verify_that_the_type_exists(type_signature.get<ArrayType>().stored_type);
        break; case TypeSignatureBody::Kind::slice_type:     verify_that_the_type_exists(type_signature.get<SliceType>().stored_type);
        break; case TypeSignatureBody::Kind::custom_type:    std::ignore = retrieve_type_definition(type_signature.get<CustomType>());
        break; case TypeSignatureBody::Kind::template_type:  return;
        break; case TypeSignatureBody::Kind::primitive_type: return;
        break; case TypeSignatureBody::Kind::inline_union: {    
            for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives) {
                verify_that_the_type_exists(alternative);
            }
        }
    }
}

void TypeDefinitionsRegister::store_type_definition(const TypeDefinition& type_def) {
    const std::string package_name = project_file_structure.get_package_name_by_file_name(type_def.get_filename());
    const std::string match_pattern = get_type_definition_match_pattern(package_name, type_def);
    const auto& insertion_outcome = type_definitions.insert({match_pattern, type_def});
    ensure_no_multiple_definition_of_the_same_type(insertion_outcome);
    type_definitions_ids.push_back(match_pattern);
}

[[nodiscard]] TypeDefinition TypeDefinitionsRegister::retrieve_type_definition(const CustomType& type_signature) {
    const std::string fully_qualified_name = get_fully_qualified_customtype_name(type_signature);
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
    switch (type_signature.typesiganture_kind()) {
        case TypeSignatureBody::Kind::slice_type: {
            return "$" + get_fully_qualified_typesignature_name(type_signature.get<SliceType>().stored_type);
        }
        case TypeSignatureBody::Kind::pointer_type: {
            return "#" + get_fully_qualified_typesignature_name(type_signature.get<PointerType>().pointed_type);
        }
        case TypeSignatureBody::Kind::array_type: {
            const TypeSignature& stored_type = type_signature.get<ArrayType>().stored_type;
            const ArrayType& array_type = type_signature.get<ArrayType>();
            std::string array_size_section = "[" + std::to_string(array_type.array_length) + "]";
            return array_size_section + get_fully_qualified_typesignature_name(stored_type);
        }
        case TypeSignatureBody::Kind::primitive_type: return type_signature.get<PrimitiveType>().type_name;
        case TypeSignatureBody::Kind::template_type: return type_signature.get<TemplateType>().type_name;
        case TypeSignatureBody::Kind::custom_type: return get_fully_qualified_customtype_name(type_signature.get<CustomType>());    
        case TypeSignatureBody::Kind::inline_union: {
            std::string inline_union_fully_qualified_name;
            for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives) {
                inline_union_fully_qualified_name += get_fully_qualified_typesignature_name(alternative) + " | ";
            }
            inline_union_fully_qualified_name.pop_back();
            inline_union_fully_qualified_name.pop_back();
            inline_union_fully_qualified_name.pop_back();
            return inline_union_fully_qualified_name;
        }
    }
    assert_unreachable();
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
        GenericSubstitutionRule::Set rules = GenericSubstitutionRule::Set::zip_components_vectors(
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
    switch (type_signature.typesiganture_kind()) {
        case TypeSignatureBody::Kind::slice_type: {
            return "$" + infer_possible_fully_qualified_typesignature_name(package_name, type_signature.get<SliceType>().stored_type);
        }
        case TypeSignatureBody::Kind::pointer_type: {
            return "#" + infer_possible_fully_qualified_typesignature_name(package_name, type_signature.get<PointerType>().pointed_type);
        }
        case TypeSignatureBody::Kind::array_type: {
            const TypeSignature& stored_type = type_signature.get<ArrayType>().stored_type;
            const ArrayType& array_type = type_signature.get<ArrayType>();
            std::string array_size_section = "[" + std::to_string(array_type.array_length) + "]";
            return array_size_section + infer_possible_fully_qualified_typesignature_name(package_name, stored_type);
        }
        case TypeSignatureBody::Kind::primitive_type: return type_signature.get<PrimitiveType>().type_name;
        case TypeSignatureBody::Kind::template_type: return type_signature.get<TemplateType>().type_name;
        case TypeSignatureBody::Kind::custom_type: {
            return infer_possible_fully_qualified_customtype_name(package_name, type_signature.get<CustomType>());    
        }
        case TypeSignatureBody::Kind::inline_union: {
            std::string inline_union_fully_qualified_name;
            for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives) {
                inline_union_fully_qualified_name += infer_possible_fully_qualified_typesignature_name(package_name, alternative) + " | ";
            }
            inline_union_fully_qualified_name.pop_back();
            inline_union_fully_qualified_name.pop_back();
            inline_union_fully_qualified_name.pop_back();
            return inline_union_fully_qualified_name;
        }
    }
    assert_unreachable();
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
