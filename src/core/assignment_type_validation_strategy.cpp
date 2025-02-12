//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/assignment_type_checker.hpp"
#include "errors/preprocessing_errors.hpp"

#include <iostream>

AssignmentTypeChecker::Strategy 
AssignmentTypeChecker::Strategy::create_default_strategy(AssignmentTypeChecker& type_checker) {
    return Strategy {
        .validate_assignment_all_intrnal = [&type_checker](const TypeSignature& source, const TypeSignature& dest) {
            const TypeSignature unaliased_source_type = type_checker.type_definitions_register.unalias_type(source);
            const TypeSignature unaliased_dest_type = type_checker.type_definitions_register.unalias_type(dest);
            return type_checker.validate_type_alias_unaware_assignment(unaliased_source_type, unaliased_dest_type);
        },
        .validate_assignment_generics_internal = [&type_checker](const TypeSignature& source, const TemplateType& dest) {
            for (GenericSubstitutionRule& rule : *type_checker.generic_substitution_rules) {
                if (rule.to_be_replaced == dest.type_name) {
                    if (type_checker.validate_assignment(source, rule.replacement)) {
                        return true;
                    }
                    else if (type_checker.validate_assignment(rule.replacement, source)) {
                        rule.replacement = source;
                        return !type_checker.strict_type_inference_deductions->contains(dest.type_name);
                    }
                    else if (rule.replacement.is<InlineUnion>()) {
                        rule.replacement.get<InlineUnion>().alternatives.push_back(source);
                        return !type_checker.strict_type_inference_deductions->contains(dest.type_name);
                    }
                    else {
                        rule.replacement = InlineUnion { dest, { rule.replacement, source } };
                        return !type_checker.strict_type_inference_deductions->contains(dest.type_name);
                    }
                }
            }
            type_checker.generic_substitution_rules->push_back({dest.type_name, source});
            return true;
        }
    };
}

AssignmentTypeChecker::Strategy 
AssignmentTypeChecker::Strategy::create_strict_strategy(AssignmentTypeChecker& type_checker) {
    return Strategy {
        .validate_assignment_all_intrnal = [&type_checker](const TypeSignature& source, const TypeSignature& dest) {
            const TypeSignature unaliased_source_type = type_checker.type_definitions_register.unalias_type(source);
            const TypeSignature unaliased_dest_type = type_checker.type_definitions_register.unalias_type(dest);
            bool forward_assignable = type_checker.validate_type_alias_unaware_assignment(unaliased_source_type, unaliased_dest_type);
            bool mutually_assignable = forward_assignable && type_checker.validate_type_alias_unaware_assignment(unaliased_dest_type, unaliased_source_type);
            if (!forward_assignable) {
                return false;
            }
            if (dest.is_generic()) {
                return true;
            }
            bool both_primitive_types = (source.is<PrimitiveType>() && dest.is<PrimitiveType>());
            bool upcasted = both_primitive_types && source.get<PrimitiveType>().type_name != dest.get<PrimitiveType>().type_name;
            if (!mutually_assignable || upcasted) {
                return false;
            }
            return true;
        },
        .validate_assignment_generics_internal = [&type_checker](const TypeSignature& source, const TemplateType& dest) {
            for (GenericSubstitutionRule& rule : *type_checker.generic_substitution_rules) {
                if (rule.to_be_replaced == dest.type_name) {
                    type_checker.strict_type_inference_deductions->insert(dest.type_name);
                    return type_checker.validate_assignment(source, rule.replacement);
                }
            }
            type_checker.generic_substitution_rules->push_back({dest.type_name, source});
            type_checker.strict_type_inference_deductions->insert(dest.type_name);
            return true;
        }
    };
}