//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/function_specificity_descriptor.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"

FunctionSpecificityDescriptor::FunctionSpecificityDescriptor(
    const FunctionDefinition& function_definition,
    TypeDefinitionsRegister& type_definitions_register
)
    : is_generic(function_definition.template_generics_names.size() > 0)
{ 
    for (const auto& arg : function_definition.arguments) {
        update_metrics_based_on_argument_type(arg.arg_type, type_definitions_register, false);
    }
}

FunctionSpecificityDescriptor::FunctionSpecificityDescriptor(
    bool is_generic, 
    size_t number_of_uses_of_its_generic_parameters,
    size_t arguments_types_complexity_score,
    size_t number_of_unions_in_arguments_types,
    size_t number_of_cases_covered_by_unions_in_arguments_types,
    size_t number_of_possible_type_conversions
)
    : is_generic(is_generic)
    , number_of_uses_of_its_generic_parameters(number_of_uses_of_its_generic_parameters)
    , arguments_types_complexity_score(arguments_types_complexity_score)
    , number_of_unions_in_arguments_types(number_of_unions_in_arguments_types)
    , number_of_cases_covered_by_unions_in_arguments_types(number_of_cases_covered_by_unions_in_arguments_types)
    , number_of_possible_type_conversions(number_of_possible_type_conversions)
{ }

FunctionSpecificityDescriptor FunctionSpecificityDescriptor::worst_possible_specificity() {
    return FunctionSpecificityDescriptor(
        true, 
        std::numeric_limits<size_t>::max(),
        std::numeric_limits<size_t>::min(),
        std::numeric_limits<size_t>::max(),
        std::numeric_limits<size_t>::max(),
        std::numeric_limits<size_t>::max()
    );
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
