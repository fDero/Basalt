
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "language/generics.hpp"

#include <iostream>

FunctionSpecificityDescriptor::FunctionSpecificityDescriptor(
    const FunctionDefinition& function_definition,
    TypeDefinitionsRegister& type_definitions_register
)
    : is_generic(function_definition.template_generics_names.size() > 0)
{ 
    for (const auto& arg : function_definition.arguments) {
        update_indices_based_on_argument_type(arg.arg_type, type_definitions_register, false);
    }
}

void FunctionSpecificityDescriptor::update_indices_based_on_argument_type(
    const TypeSignature& input_arg_type, 
    TypeDefinitionsRegister& type_definitions_register,
    bool current_type_is_fixed_and_cannot_be_an_upcasting_target
) { 
    TypeSignature arg_type = type_definitions_register.unalias_type(input_arg_type);
    if (arg_type.is<PrimitiveType>()){
        if (!current_type_is_fixed_and_cannot_be_an_upcasting_target) {
            const std::string& type_name = arg_type.get<PrimitiveType>().type_name;
            number_of_possible_type_conversions += (type_name == "RawString") * 3;
            number_of_possible_type_conversions += (type_name == "String") * 2;
        }
    }
    else if (arg_type.is<PointerType>() || arg_type.is<ArrayType>()){
        TypeSignature inner_type = (arg_type.is<PointerType>())
            ? arg_type.get<PointerType>().pointed_type
            : arg_type.get<ArrayType>().stored_type; 
        current_type_is_fixed_and_cannot_be_an_upcasting_target |= arg_type.is<PointerType>();
        update_indices_based_on_argument_type(
            inner_type, 
            type_definitions_register, 
            current_type_is_fixed_and_cannot_be_an_upcasting_target
        );
    }
    else if (arg_type.is<SliceType>()){
        arguments_types_complexity_score += !current_type_is_fixed_and_cannot_be_an_upcasting_target;
        TypeSignature inner_type = arg_type.get<SliceType>().stored_type; 
        update_indices_based_on_argument_type(inner_type, type_definitions_register, true);
    }
    else if (arg_type.is<TemplateType>()){
        number_of_uses_of_its_generic_parameters++;
    }
    else if (arg_type.is<InlineUnion>()){
        std::vector<TypeSignature>& alternatives = arg_type.get<InlineUnion>().alternatives;
        number_of_unions_in_arguments_types++;
        update_union_covered_cases(alternatives, type_definitions_register);
    }
    else if (arg_type.is<CustomType>()){
        const CustomType& custom_type = arg_type.get<CustomType>();
        arguments_types_complexity_score += custom_type.type_parameters.size();
        TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(custom_type);
        if (type_definition.is<UnionDefinition>()) {
            number_of_unions_in_arguments_types++;
            std::vector<TypeSignature>& alternatives = type_definition.get<UnionDefinition>().types;
            update_union_covered_cases(alternatives, type_definitions_register);
        }
        for (const auto& type_parameter : custom_type.type_parameters) {
            update_indices_based_on_argument_type(
                type_parameter, 
                type_definitions_register, 
                false
            );
        }
    }
}

void FunctionSpecificityDescriptor::update_union_covered_cases(
    const std::vector<TypeSignature>& alternatives, 
    TypeDefinitionsRegister& type_definitions_register
) { 
    for (const TypeSignature& alternative : alternatives){
        if (alternative.is<InlineUnion>()){
            const std::vector<TypeSignature>& inner_alternatives = alternative.get<InlineUnion>().alternatives;
            update_union_covered_cases(inner_alternatives, type_definitions_register);
            continue;
        }
        else if (alternative.is<CustomType>()){
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

FunctionSpecificityDescriptor::ComparisonResult 
FunctionSpecificityDescriptor::compare_with(const FunctionSpecificityDescriptor& other) const {
    if (is_generic != other.is_generic) {
        return (!is_generic)
            ? ComparisonResult::more_specific 
            : ComparisonResult::less_specific;
    }
    if (number_of_uses_of_its_generic_parameters != other.number_of_uses_of_its_generic_parameters) {
        return (number_of_uses_of_its_generic_parameters < other.number_of_uses_of_its_generic_parameters)
            ? ComparisonResult::more_specific 
            : ComparisonResult::less_specific;
    }
    if (arguments_types_complexity_score != other.arguments_types_complexity_score) {
        return (arguments_types_complexity_score > other.arguments_types_complexity_score)
            ? ComparisonResult::more_specific 
            : ComparisonResult::less_specific;
    }
    if (number_of_unions_in_arguments_types != other.number_of_unions_in_arguments_types) {
        return (number_of_unions_in_arguments_types < other.number_of_unions_in_arguments_types)
            ? ComparisonResult::more_specific 
            : ComparisonResult::less_specific;
    }
    if (number_of_cases_covered_by_unions_in_arguments_types != other.number_of_cases_covered_by_unions_in_arguments_types) {
        return (number_of_cases_covered_by_unions_in_arguments_types < other.number_of_cases_covered_by_unions_in_arguments_types)
            ? ComparisonResult::more_specific 
            : ComparisonResult::less_specific;
    }
    if (number_of_possible_type_conversions != other.number_of_possible_type_conversions) {
        return (number_of_possible_type_conversions < other.number_of_possible_type_conversions)
            ? ComparisonResult::more_specific 
            : ComparisonResult::less_specific;
    }
    return ComparisonResult::equally_specific;
}
