//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/function_specificity_descriptor.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"

void FunctionSpecificityDescriptor::update_metrics_based_on_argument_type(
    const TypeSignature& input_arg_type, 
    TypeDefinitionsRegister& type_definitions_register,
    bool current_type_is_fixed_and_cannot_be_an_upcasting_target
) { 
    TypeSignature arg_type = type_definitions_register.unalias_type(input_arg_type);
    TypeDefinitionsRegister& reg = type_definitions_register;
    const bool flag = current_type_is_fixed_and_cannot_be_an_upcasting_target; 
    switch (arg_type.typesiganture_kind()) {
        break; case TypeSignatureBody::Kind::primitive_type: update_metrics_based_on_primitive_type(input_arg_type, reg, flag);
        break; case TypeSignatureBody::Kind::pointer_type: update_metrics_based_on_pointer_type(input_arg_type, reg, flag);
        break; case TypeSignatureBody::Kind::template_type: number_of_uses_of_its_generic_parameters++;
        break; case TypeSignatureBody::Kind::inline_union: update_metrics_based_on_inline_union_type(input_arg_type, reg, flag);
        break; case TypeSignatureBody::Kind::custom_type: update_metrics_based_on_custom_type(input_arg_type, reg, flag);
        break; case TypeSignatureBody::Kind::array_type: return;
        break; case TypeSignatureBody::Kind::slice_type: return;
    }
}

void FunctionSpecificityDescriptor::update_metrics_based_on_primitive_type(
    const TypeSignature& typesignature, 
    TypeDefinitionsRegister& type_definitions_register,
    bool current_type_is_fixed_and_cannot_be_an_upcasting_target
) {
    assert_typesignature_is<PrimitiveType>(typesignature);
    const PrimitiveType& primitive_type = typesignature.get<PrimitiveType>();
    if (!current_type_is_fixed_and_cannot_be_an_upcasting_target) {
        const std::string& type_name = primitive_type.type_name;
        number_of_possible_type_conversions += (type_name == "RawString") * 3;
        number_of_possible_type_conversions += (type_name == "String") * 2;
    }
}

void FunctionSpecificityDescriptor::update_metrics_based_on_pointer_type(
    const TypeSignature& typesignature, 
    TypeDefinitionsRegister& type_definitions_register,
    bool current_type_is_fixed_and_cannot_be_an_upcasting_target
) {
    assert_typesignature_is<PointerType>(typesignature);
    const PointerType& inner_type = typesignature.get<PointerType>();
    current_type_is_fixed_and_cannot_be_an_upcasting_target = true;
    update_metrics_based_on_argument_type(
        inner_type, 
        type_definitions_register, 
        current_type_is_fixed_and_cannot_be_an_upcasting_target
    );
}

void FunctionSpecificityDescriptor::update_metrics_based_on_inline_union_type(
    const TypeSignature& typesignature, 
    TypeDefinitionsRegister& type_definitions_register,
    bool current_type_is_fixed_and_cannot_be_an_upcasting_target
) {
    assert_typesignature_is<InlineUnion>(typesignature);
    const InlineUnion& inline_union = typesignature.get<InlineUnion>();
    const std::vector<TypeSignature>& alternatives = inline_union.alternatives;
    number_of_unions_in_arguments_types++;
    update_union_covered_cases(alternatives, type_definitions_register);
}

void FunctionSpecificityDescriptor::update_union_covered_cases(
    const std::vector<TypeSignature>& alternatives, 
    TypeDefinitionsRegister& type_definitions_register
) { 
    for (const TypeSignature& alternative : alternatives) {
        if (alternative.is<InlineUnion>()) {
            const std::vector<TypeSignature>& inner_alternatives = alternative.get<InlineUnion>().alternatives;
            update_union_covered_cases(inner_alternatives, type_definitions_register);
            continue;
        }
        else if (alternative.is<CustomType>()) {
            const CustomType& custom_type = alternative.get<CustomType>();
            TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(custom_type);
            if (type_definition.is<UnionDefinition>()) {
                std::vector<TypeSignature>& inner_alternatives = type_definition.get<UnionDefinition>().types;
                update_union_covered_cases(inner_alternatives, type_definitions_register);
                continue;
            }
        }
        number_of_cases_covered_by_unions_in_arguments_types++;
    }
}

void FunctionSpecificityDescriptor::update_metrics_based_on_custom_type(
    const TypeSignature& typesignature, 
    TypeDefinitionsRegister& type_definitions_register,
    bool current_type_is_fixed_and_cannot_be_an_upcasting_target
) {
    assert_typesignature_is<CustomType>(typesignature);
    
    const CustomType& custom_type = typesignature.get<CustomType>();
    arguments_types_complexity_score += custom_type.type_parameters.size();
    TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(custom_type);
    if (type_definition.is<UnionDefinition>()) {
        number_of_unions_in_arguments_types++;
        std::vector<TypeSignature>& alternatives = type_definition.get<UnionDefinition>().types;
        update_union_covered_cases(alternatives, type_definitions_register);
    }
    for (const auto& type_parameter : custom_type.type_parameters) {
        update_metrics_based_on_argument_type(
            type_parameter, 
            type_definitions_register, 
            false
        );
    }
}