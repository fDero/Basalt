/**
 * @file function_specificity_descriptor.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the FunctionSpecificityDescriptor class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <vector>

#include "language/definitions.hpp"
#include "model/type_definitions_register.hpp"

/**
 * @brief   Used to encode the specificity of a given function definition, to later
 *          compare it with other function definitions and select the most specific one
 *          during overloading resolution.
 * 
 * @details The FunctionSpecificityDescriptor class is used to encode the specificity of
 *          a given function definition, to later compare it with other function definitions
 *          and select the most specific one during overloading resolution. It does so by
 *          analyzing the types of the arguments of the function and the possible type conversions
 *          that can be applied to them. It also considers the number of uses of the generic parameters
 *          of the function, the number of unions in the arguments types and the number of cases covered
 *          by such unions.
 * 
 * @note    The FunctionSpecificityDescriptor class needs to be constructed from a TypeDefinitionsRegister
 *          in order to be able to resolve the types of the arguments of the function. The TypeDefinitionsRegister
 *          must be already checked for correctness, otherwise the FunctionSpecificityDescriptor could fall in 
 *          an infinite loop while trying to resolve the definitions of such types contains a direct cyclic dependency.
 * 
 * @see     TypeDependencyNavigator
 * @see     OverloadingResolutionEngine
 */
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

        ComparisonResult compare_with(const FunctionSpecificityDescriptor& other) const;

        static FunctionSpecificityDescriptor worst_possible_specificity();

    protected:

        void update_indices_based_on_argument_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_primitive_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_pointer_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_array_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );
        
        void update_indicies_based_on_slice_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_inline_union_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_custom_type(
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