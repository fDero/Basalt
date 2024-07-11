
#include "toolchain/representation.hpp"
#include "toolchain/preprocessor.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

AssignmentTypeChecker::AssignmentTypeChecker(
    TypeDefinitionsRegister& type_definitions_register, 
    ProjectFileStructure& project_file_structure
)
    : type_definitions_register(type_definitions_register), 
    project_file_structure(project_file_structure) 
{}

GenericSubstitutionRuleSet::Ref AssignmentTypeChecker::get_generic_substitution_rules() {
    return generic_substitution_rules;
}

bool AssignmentTypeChecker::validate_assignment(const TypeSignature& source, const TypeSignature& dest) {
    const TypeSignature unaliased_source_type = type_definitions_register.unalias_type(source);
    const TypeSignature unaliased_dest_type = type_definitions_register.unalias_type(dest);
    return validate_type_alias_unaware_assignment(unaliased_source_type, unaliased_dest_type);
}

bool AssignmentTypeChecker::validate_type_alias_unaware_assignment(const TypeSignature& source, const TypeSignature& dest) {
    if (dest.is<TemplateType>()) {
        return validate_assignment_to_template_generic(source, dest.get<TemplateType>());
    }
    else if (dest.is<CustomType>()) {
        return validate_assignment_to_custom_type(source, dest.get<CustomType>());
    }
    else if (dest.is<InlineUnion>()) {
        return validate_assignment_to_inline_union(source, dest.get<InlineUnion>());
    }
    else if (dest.is<PrimitiveType>()) {
        return validate_assignment_to_primitive_type(source, dest.get<PrimitiveType>());
    }
    else if (dest.is<ArrayType>()) {
        return validate_assignment_to_array_type(source, dest.get<ArrayType>());
    }
    else if (dest.is<PointerType>()) {
        return validate_assignment_to_pointer_type(source, dest.get<PointerType>());
    }
    else if (dest.is<SliceType>()) {
        return validate_assignment_to_slice_type(source, dest.get<SliceType>());
    }
    assert_unreachable();
}

bool AssignmentTypeChecker::validate_assignment_to_template_generic(const TypeSignature& source, const TemplateType& dest) {
    for (GenericSubstitutionRule& rule : *generic_substitution_rules) {
        if (rule.to_be_replaced == dest.type_name) {
            if (validate_assignment(rule.replacement, source)) {
                rule.replacement = source;
            }
            else if (validate_assignment(source, rule.replacement)) {
                return true;
            }
            else if (rule.replacement.is<InlineUnion>()) {
                rule.replacement.get<InlineUnion>().alternatives.push_back(source);
            }
            else {
                rule.replacement = InlineUnion { dest, { rule.replacement, source } };
            }
            return true;
        }
    }
    generic_substitution_rules->push_back({dest.type_name, source});
    return true;
}

bool AssignmentTypeChecker::validate_assignment_to_primitive_type(const TypeSignature& source, const PrimitiveType& dest) {
    if (dest.type_name == "String" || dest.type_name == "RawString") {
        return validate_assignment_to_string(source, dest);
    }
    else if (source.is<PrimitiveType>()) {
        return source.get<PrimitiveType>().type_name == dest.type_name;
    }
    else {
        return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_array_type(const TypeSignature& source, const ArrayType& dest) {
    if (source.is<ArrayType>()) {
        return validate_assignment(source.get<ArrayType>().stored_type, dest.stored_type);
    }
    else {
        return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_pointer_type(const TypeSignature& source, const PointerType& dest) {
    if (source.is<PointerType>()) {
        return validate_assignment_very_strictly(source.get<PointerType>().pointed_type, dest.pointed_type);
    }
    else {
        return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_slice_type(const TypeSignature& source, const SliceType& dest) {
    if (source.is<ArrayType>()) {
        return validate_assignment(source.get<ArrayType>().stored_type, dest.stored_type);
    }
    else if (source.is<SliceType>()) {
        return validate_assignment_very_strictly(source.get<SliceType>().stored_type, dest.stored_type);
    }
    return false;
}

bool AssignmentTypeChecker::validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest) {
    if (source.is<PrimitiveType>()) {
        if (source.get<PrimitiveType>().type_name == "RawString")
            return dest.type_name == "RawString";
        else
            return (source.get<PrimitiveType>().type_name == "String");
    }
    else if (source.is<SliceType>()) {
        return source.get<SliceType>().stored_type.is<PrimitiveType>() &&
            source.get<SliceType>().stored_type.get<PrimitiveType>().type_name == "Char";
    }
    else if (source.is<ArrayType>()) {
        return source.get<ArrayType>().stored_type.is<PrimitiveType>() &&
            source.get<ArrayType>().stored_type.get<PrimitiveType>().type_name == "Char";
    }
    else {
        return false;
    }
}