//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cassert>
#include "core/assignment_type_checker.hpp"
#include "errors/preprocessing_errors.hpp"

bool AssignmentTypeChecker::validate_assignment_to_custom_type(const TypeSignature& source, const CustomType& dest) {
    bool direct_assignment = (source.is<CustomType>() && name_equivalence_assignment_validation(source.get<CustomType>(), dest));
    return direct_assignment || structural_equivalence_assignment_validation(source, dest);
}

bool AssignmentTypeChecker::validate_assignment_to_inline_union(const TypeSignature& source, const InlineUnion& inline_union) {
    return structural_equivalence_assignment_validation(source, inline_union);
}

bool AssignmentTypeChecker::validate_assignment_to_union_alternatives(const TypeSignature& source, const std::vector<TypeSignature>& alternatives) {
    for (const TypeSignature& alternative : alternatives) {
        if (validate_assignment(source, alternative)) {
            return true;
        }
    }
    return false;
}

bool AssignmentTypeChecker::structural_equivalence_assignment_validation(const TypeSignature& source, const TypeSignature& dest) {
    std::vector<TypeSignature> dest_union_alternatives = type_definitions_register.fetch_union_alternatives(dest);
    if (dest_union_alternatives.empty()) {
        return false;
    }
    if (validate_assignment_to_union_alternatives(source, dest_union_alternatives)) {
        return true;
    }
    std::vector<TypeSignature> source_union_alternatives = type_definitions_register.fetch_union_alternatives(source);
    if (source_union_alternatives.empty()) {
        return false;
    }
    for (const TypeSignature& alternative : source_union_alternatives) {
        if (!validate_assignment_to_union_alternatives(alternative, dest_union_alternatives)) {
            return false;
        }
    }
    return true;
}

bool AssignmentTypeChecker::name_equivalence_assignment_validation(const CustomType& source, const CustomType& dest) {
    if (source.type_name != dest.type_name || source.type_parameters.size() != dest.type_parameters.size()) {
        return false;
    }
    if (source.package_prefix != dest.package_prefix) {
        std::string source_package_name = (!source.package_prefix.empty())? 
            source.package_prefix : project_file_structure.get_package_name_by_file_name(source.filename);
        std::string dest_package_name = (!dest.package_prefix.empty())? 
            dest.package_prefix : project_file_structure.get_package_name_by_file_name(dest.filename);
        if (source_package_name != dest_package_name) {
            return false;
        }
    }
    return type_parameters_assignment_validation(source, dest);
}

bool AssignmentTypeChecker::type_parameters_assignment_validation(const CustomType &source, const CustomType &dest) {
    for (size_t i = 0; i < source.type_parameters.size(); i++) {
        const TypeSignature& source_generic = source.type_parameters[i];
        const TypeSignature& dest_generic = dest.type_parameters[i];
        if (dest_generic.is<TemplateType>() && !validate_assignment_to_generic_type_parameter(source_generic, dest_generic.get<TemplateType>())) {
            return false;
        }
        else if (!validate_assignment_very_strictly(source_generic, dest_generic)) {
            return false;
        }
    }
    return true;
}

bool AssignmentTypeChecker::validate_assignment_very_strictly(const TypeSignature& source_generic, const TypeSignature& dest_generic) {
    bool forward_assignable = validate_assignment(source_generic, dest_generic);
    if (!forward_assignable) {
        return false;
    }
    if (dest_generic.is_generic()) {
        return true;
    }
    assert(forward_assignable);
    bool mutually_assignable = validate_assignment(dest_generic, source_generic);
    bool both_primitive_types = (source_generic.is<PrimitiveType>() && dest_generic.is<PrimitiveType>());
    bool upcasted = both_primitive_types && source_generic.get<PrimitiveType>().type_name != dest_generic.get<PrimitiveType>().type_name;
    if (!mutually_assignable || upcasted) {
        return false;
    }
    return true;
}

bool AssignmentTypeChecker::validate_assignment_to_generic_type_parameter(const TypeSignature& source, const TemplateType& dest) {
    for (GenericSubstitutionRule& rule : *generic_substitution_rules) {
        if (rule.to_be_replaced == dest.type_name) {
            return validate_assignment_very_strictly(source, rule.replacement);
        }
    }
    generic_substitution_rules->push_back({dest.type_name, source});
    return true;
}