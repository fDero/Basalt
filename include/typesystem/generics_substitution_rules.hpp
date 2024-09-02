/**
 * @file generics_substitution_rules.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of GenericSubstitutionRule, GenericSubstitutionRule::Set, GenericSubstitutionRule::Set::Ref
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "language/typesignatures.hpp"

/**
 * @brief   Used to encode how a generic type parameter should be substituted
 * 
 * @details The GenericSubstitutionRule struct is used to encode how a generic type parameter should be substituted.
 *          It contains the name of the generic type parameter to be replaced, and the type signature that should replace it.
 *          It also contains the definition of the inner class GenericSubstitutionRule::Set, which is a set of GenericSubstitutionRule.
 *          It also contains the definition of the inner class GenericSubstitutionRule::Set::Ref, which is a shared pointer to a GenericSubstitutionRule::Set.
 *          It also contains the static method GenericSubstitutionRule::Set::zip_components_vectors, which is used to create a GenericSubstitutionRule::Set
 *          from two vectors of strings, representing type-parameters names, and TypeSignatures, representing type-parameters value.
 *          This class is used by the GenericsInstantiationEngine to perform type-inference on generic types, and by the AssignmentTypeChecker to
 *          generate type-inference constraints.
 * 
 * @see     GenericsInstantiationEngine
 * @see     AssignmentTypeChecker
 */
struct GenericSubstitutionRule {

    std::string to_be_replaced;
    TypeSignature replacement;

    struct Set : public std::vector<GenericSubstitutionRule> {
    
        using Ref = std::shared_ptr<GenericSubstitutionRule::Set>;

        using vector<GenericSubstitutionRule>::vector;
        using vector<GenericSubstitutionRule>::operator=;
        using vector<GenericSubstitutionRule>::operator[];
        using vector<GenericSubstitutionRule>::push_back;

        [[nodiscard]] static GenericSubstitutionRule::Set zip_components_vectors (
            const std::vector<std::string>& template_generics,
            const std::vector<TypeSignature>& type_parameters
        );
    };
};