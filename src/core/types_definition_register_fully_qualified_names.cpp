//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <regex>

#include "core/type_definitions_register.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"

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

std::string TypeDefinitionsRegister::get_fully_qualified_typedefinition_name(const TypeDefinition& type_defintion) {
    std::string package_where_typedef_is_located = project_file_structure.get_package_name_by_file_name(type_defintion.get_filename());
    return get_type_definition_match_pattern(package_where_typedef_is_located, type_defintion);
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

std::optional<std::string> TypeDefinitionsRegister::search_fully_qualified_typesignature_name(
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
        std::regex package_prefix("(.*?::)+");
        std::string new_name = std::regex_replace(instantiated_concrete_type_key, package_prefix, "");
        TypeDefinition instantiated = generic_instantiation_engine.instantiate_generic_typedefinition(to_be_instantiated, new_name);
        type_definitions.insert({instantiated_concrete_type_key, instantiated});
        return instantiated_concrete_type_key;
    }
}

std::string TypeDefinitionsRegister::infer_possible_fully_qualified_typesignature_name(
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

std::string TypeDefinitionsRegister::infer_possible_fully_qualified_customtype_name(
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

std::string TypeDefinitionsRegister::get_type_definition_match_pattern(
    const std::string& package_name,
    const TypeDefinition& type_definition
) {
    std::string pattern_tag_name = package_name + namespace_concatenation + type_definition.get_simple_name();
    size_t number_of_generics = type_definition.get_number_of_generics();
    std::string generics_section = (number_of_generics > 0)? "<" + std::to_string(number_of_generics) + ">" : "";
    return pattern_tag_name + generics_section;
}

std::string TypeDefinitionsRegister::get_type_signature_match_pattern(
    const std::string& package_name,
    const CustomType& type_signature
) {
    std::string pattern_tag_name = package_name + namespace_concatenation + type_signature.type_name;
    size_t number_of_generics = type_signature.type_parameters.size();
    std::string generics_section = (number_of_generics > 0)? "<" + std::to_string(number_of_generics) + ">" : "";
    return pattern_tag_name + generics_section;
}