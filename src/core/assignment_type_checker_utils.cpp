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

bool AssignmentTypeChecker::validate_assignment(
    const TypeSignature& source, 
    const TypeSignature& dest
) {
    return validation_strategy
        .validate_assignment_all_intrnal(source, dest);
}

bool AssignmentTypeChecker::validate_assignment_to_template_generic(
    const TypeSignature& source, 
    const TemplateType& dest
) {
    return validation_strategy
        .validate_assignment_generics_internal(source, dest);
}

bool AssignmentTypeChecker::validate_assignment_very_strictly(
    const TypeSignature& source, 
    const TypeSignature& dest
) {
    AssignmentTypeChecker checker(
        type_definitions_register, 
        project_file_structure
    );
    checker.validation_strategy = Strategy::create_strict_strategy(checker);
    checker.generic_substitution_rules = generic_substitution_rules;
    checker.strict_type_inference_deductions = strict_type_inference_deductions;
    return checker.validate_assignment(source, dest);
}