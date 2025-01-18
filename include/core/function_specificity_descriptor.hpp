//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>

#include "language/definitions.hpp"
#include "core/type_definitions_register.hpp"

class FunctionSpecificityDescriptor {

    public:
        FunctionSpecificityDescriptor(
            const FunctionDefinition& function_definition,
            TypeDefinitionsRegister& type_definitions_register
        );

        FunctionSpecificityDescriptor(
            bool is_generic, 
            size_t number_of_uses_of_its_generic_parameters,
            size_t arguments_types_complexity_score,
            size_t number_of_unions_in_arguments_types,
            size_t number_of_cases_covered_by_unions_in_arguments_types,
            size_t number_of_possible_type_conversions
        );

        enum class ComparisonResult {
            less_specific,
            equally_specific,
            more_specific
        };

        [[nodiscard]] ComparisonResult compare_with(const FunctionSpecificityDescriptor& other) const;

        static FunctionSpecificityDescriptor worst_possible_specificity();

    protected:
        void update_metrics_based_on_argument_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_metrics_based_on_primitive_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_metrics_based_on_pointer_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_metrics_based_on_inline_union_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_metrics_based_on_custom_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_union_covered_cases(
            const std::vector<TypeSignature>& alternatives, 
            TypeDefinitionsRegister& type_definitions_register
        );

    private:
        bool   is_generic = false;
        size_t number_of_uses_of_its_generic_parameters = 0;
        size_t arguments_types_complexity_score = 0;
        size_t number_of_unions_in_arguments_types = 0;
        size_t number_of_cases_covered_by_unions_in_arguments_types = 0;
        size_t number_of_possible_type_conversions = 0;
};