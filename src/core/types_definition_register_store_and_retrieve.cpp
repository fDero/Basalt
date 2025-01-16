//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/type_definitions_register.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"

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
    const std::string match_pattern = get_fully_qualified_typedefinition_name(type_def);
    const auto& insertion_outcome = type_definitions.insert({match_pattern, type_def});
    ensure_no_multiple_definition_of_the_same_type(insertion_outcome);
    type_definitions_ids.push_back(match_pattern);
}

TypeDefinition TypeDefinitionsRegister::retrieve_type_definition(const CustomType& type_signature) {
    const std::string fully_qualified_name = get_fully_qualified_customtype_name(type_signature);
    auto search_outcome = type_definitions.find(fully_qualified_name);
    if (search_outcome != type_definitions.end()) {
        return search_outcome->second;
    }
    throw_no_type_definition_found(type_signature);
}

TypeSignature TypeDefinitionsRegister::unalias_type(const TypeSignature& type_signature) {
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