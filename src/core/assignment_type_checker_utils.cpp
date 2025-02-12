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

bool AssignmentTypeChecker::validate_type_alias_unaware_assignment(
    const TypeSignature& source, 
    const TypeSignature& dest, 
    bool strict_mode
) {
    TypeSignature true_source = source;
    if (source.is<TemplateType>()) {
        for (const auto& rule : *generic_substitution_rules) {
            if (rule.to_be_replaced == source.get<TemplateType>().type_name) {
                true_source = rule.replacement;
            }
        }
    }
    switch (dest.typesiganture_kind()) {
        case TypeSignatureBody::Kind::template_type:  return validate_assignment_to_template_generic(true_source, dest.get<TemplateType>(), strict_mode);
        case TypeSignatureBody::Kind::custom_type:    return validate_assignment_to_custom_type(true_source, dest.get<CustomType>(), strict_mode);
        case TypeSignatureBody::Kind::pointer_type:   return validate_assignment_to_pointer_type(true_source, dest.get<PointerType>(), strict_mode);
        case TypeSignatureBody::Kind::array_type:     return validate_assignment_to_array_type(true_source, dest.get<ArrayType>(), strict_mode);
        case TypeSignatureBody::Kind::slice_type:     return validate_assignment_to_slice_type(true_source, dest.get<SliceType>(), strict_mode);
        case TypeSignatureBody::Kind::inline_union:   return validate_assignment_to_inline_union(true_source, dest.get<InlineUnion>(), strict_mode); 
        case TypeSignatureBody::Kind::primitive_type: return validate_assignment_to_primitive_type(true_source, dest.get<PrimitiveType>(), strict_mode);
    }
    assert_unreachable();
}

bool AssignmentTypeChecker::validate_assignment(
    const TypeSignature& source, 
    const TypeSignature& dest,
    bool strict_mode
) {
    const TypeSignature unaliased_source_type = type_definitions_register.unalias_type(source);
    const TypeSignature unaliased_dest_type = type_definitions_register.unalias_type(dest);
    if (!strict_mode) {
        return validate_type_alias_unaware_assignment(unaliased_source_type, unaliased_dest_type, false);
    }
    bool forward_assignable = validate_type_alias_unaware_assignment(unaliased_source_type, unaliased_dest_type, true);
    bool mutually_assignable = forward_assignable && validate_type_alias_unaware_assignment(unaliased_dest_type, unaliased_source_type, true);
    if (!forward_assignable) {
        return false;
    }
    bool both_primitive_types = (source.is<PrimitiveType>() && dest.is<PrimitiveType>());
    bool upcasted = both_primitive_types && source.get<PrimitiveType>().type_name != dest.get<PrimitiveType>().type_name;
    return mutually_assignable && !upcasted;
}

bool AssignmentTypeChecker::validate_assignment_to_template_generic(
    const TypeSignature& source, 
    const TemplateType& dest,
    bool strict_mode
) {
    bool already_locked = strict_type_inference_deductions.contains(dest.type_name);
    if (strict_mode) {
        strict_type_inference_deductions.insert(dest.type_name);
    }
    for (GenericSubstitutionRule& rule : *generic_substitution_rules) {
        if (rule.to_be_replaced == dest.type_name) {
            if (validate_assignment(source, rule.replacement, strict_mode)) {   
                return true;
            }
            else if (validate_assignment(rule.replacement, source, false)) {
                rule.replacement = source;
                return !already_locked;
            }
            else if (rule.replacement.is<InlineUnion>()) {
                rule.replacement.get<InlineUnion>().alternatives.push_back(source);
                return !already_locked;
            }
            else {
                rule.replacement = InlineUnion { dest, { rule.replacement, source } };
                return !already_locked;
            }
        }
    }
    generic_substitution_rules->push_back({dest.type_name, source});
    return !already_locked;
}