/**
 * @file common_feature_adoption_plan_generation_engine.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the CommonFeatureAdoptionResolutionEngine class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"
#include "model/type_definitions_register.hpp"
#include "model/project_file_structure.hpp"
#include "model/common_feature_adoption_plan_descriptor.hpp"

/**
 * @brief   Used to generate a common feature adoption plan generated for a given unmatched function call.
 * 
 * @details The CommonFeatureAdoptionPlanGenerationEngine class is used to generate a common feature adoption 
 *          plan generated for a given unmatched function call. Common feature adoption is a Basalt feature
 *          that allows to automatically "generate" a function overload based on the other already existing
 *          ones. The CFA-generated overload is generated in such a way that at runtime it will behave as if
 *          it was dispatching to the most appropriate existing overload based on the run-time type-informations
 *          of the arguments wich are passed to it as unions. 
 * 
 * @see     CommonFeatureAdoptionPlanDescriptor
 */
class CommonFeatureAdoptionPlanGenerationEngine {
    
    public:

        /** 
         * @brief   Construct a new Common Feature Adoption Resolution Engine object
         * 
         * @param   overloading_resolution_engine used to initialize the internal overloading_resolution_engine attribute.
         * 
         * @param   type_definitions_register used to initialize the internal type_definitions_register attribute.
         */
        CommonFeatureAdoptionPlanGenerationEngine(
            OverloadingResolutionEngine& overloading_resolution_engine,
            TypeDefinitionsRegister& type_definitions_register
        );

        /**
         * @brief   Used to generate a common feature adoption plan for an unmatched function call.
         * 
         * @details Internally, all that it does is to call the 'generate_common_feature_adoption_iterating_over_arg_types'
         *          method, passing the arguments types of the function call as an iterator.
         */
        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_plan(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    private:

        /**
         * @brief   Used to iterate over the types of the arguments of a function call
         *          searching for a union of some sort. 
         * 
         * @details While iterating over argument-types, once a union is found, the actual generation
         *          of the common-feature-adoption plan is delegated to the most appropriate method 
         *          based on the kind of the union found (inline/named).
         */
        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_iterating_over_arg_types(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type
        );

        /**
         * @brief    Used to generate a common-feature-adoption plan for a given function call
         *           for wich the current argument that is being processed is an inline-union.
         * 
         * @details  The actual generation of the common-feature-adoption plan is delegated to the 
         *           generate_common_feature_adoption_for_current_multicase_arg method, wich requires 
         *           the alternatives of the union to be passed as an argument. The purpuse of this method
         *           is to extract such alternatives and pass them to such method. 
         */
        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_for_inline_union(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type
        );

        /**
         * @brief   Used to generate a common-feature-adoption plan for a given function call
         *          for wich the current argument that is being processed is a custom-type.
         * 
         * @details Depending on the kind of the custom-type, this method will either 
         *          keep going with the iteration calling 'generate_common_feature_adoption_iterating_over_arg_types'
         *          with the next argument-type, or, in the case of a named-union, it will extract its
         *          alternatives and pass them  'generate_common_feature_adoption_for_current_multicase_arg'.
         */
        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_for_custom_type(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type
        );

        /**
         * @brief   Used to generate a common feature adoption plan for the current multicase argument,
         *          meaning for the current inline or named union.
         * 
         * @details for every alternative of the union, the method will generate a new argument-types vector
         *          by replacing the current argument-type with the alternative. Then, it will try to retrieve
         *          an overload that matches the new argument-types vector. If such an overload is found, it will
         *          be added to the common-feature-adoption plan. If not, the method will recursively call itself
         *          on the next argument-type searching for a valid set of argument-types.
         */
        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_for_current_multicase_arg(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type,
            const std::vector<TypeSignature>& alternatives
        );

        /**
         * @brief   Used to attempt the retrieval of a function definition from the function-overloads-register
         *          to end the common-feature-adoption plan generation process.
         */
        OverloadingResolutionEngine& overloading_resolution_engine;
        
        /**
         * @brief   Used to retrieve the fully-qualified name of a type-signature.
         *          Such fully-qualified name is used as a key in the common-feature-adoption tree.
         */
        TypeDefinitionsRegister& type_definitions_register;
};
