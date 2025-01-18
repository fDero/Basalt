//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/assignment_type_checker.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

AssignmentTypeChecker::AssignmentTypeChecker(
    TypeDefinitionsRegister& type_definitions_register, 
    ProjectFileStructure& project_file_structure
)
    : type_definitions_register(type_definitions_register), 
    project_file_structure(project_file_structure) 
{}

GenericSubstitutionRule::Set::Ref AssignmentTypeChecker::get_generic_substitution_rules() {
    return generic_substitution_rules;
}

bool AssignmentTypeChecker::validate_assignment(const TypeSignature& source, const TypeSignature& dest) {
    const TypeSignature unaliased_source_type = type_definitions_register.unalias_type(source);
    const TypeSignature unaliased_dest_type = type_definitions_register.unalias_type(dest);
    return validate_type_alias_unaware_assignment(unaliased_source_type, unaliased_dest_type);
}

bool AssignmentTypeChecker::validate_type_alias_unaware_assignment(const TypeSignature& source, const TypeSignature& dest) {
    switch (dest.typesiganture_kind()) {
        case TypeSignatureBody::Kind::template_type:  return validate_assignment_to_template_generic(source, dest.get<TemplateType>());
        case TypeSignatureBody::Kind::custom_type:    return validate_assignment_to_custom_type(source, dest.get<CustomType>());
        case TypeSignatureBody::Kind::pointer_type:   return validate_assignment_to_pointer_type(source, dest.get<PointerType>());
        case TypeSignatureBody::Kind::array_type:     return validate_assignment_to_array_type(source, dest.get<ArrayType>());
        case TypeSignatureBody::Kind::slice_type:     return validate_assignment_to_slice_type(source, dest.get<SliceType>());
        case TypeSignatureBody::Kind::inline_union:   return validate_assignment_to_inline_union(source, dest.get<InlineUnion>()); 
        case TypeSignatureBody::Kind::primitive_type: return validate_assignment_to_primitive_type(source, dest.get<PrimitiveType>());
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

bool AssignmentTypeChecker::validate_assignment_to_array_type(const TypeSignature& source, const ArrayType& dest) {
    bool assignment_makes_sense = (source.is<ArrayType>());
    assignment_makes_sense &= validate_assignment(source.get<ArrayType>().stored_type, dest.stored_type);
    assignment_makes_sense &= source.get<ArrayType>().array_length == dest.array_length;
    return assignment_makes_sense;
}

bool AssignmentTypeChecker::validate_assignment_to_pointer_type(const TypeSignature& source, const PointerType& dest) {
    bool assignment_makes_sense = (source.is<PointerType>());
    return assignment_makes_sense && validate_assignment_very_strictly(source.get<PointerType>().pointed_type, dest.pointed_type);
}