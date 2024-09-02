/**
 * @file common_feature_adoption_plan_descriptor.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the CommonFeatureAdoptionPlanDescriptor struct
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <memory>
#include <string>
#include <map>

#include "language/definitions.hpp"

/**
 * @brief   Used to represent a common feature adoption plan generated for a given unmatched function call.
 * 
 * @details The CommonFeatureAdoptionPlanDescriptor struct is used to represent a common feature adoption 
 *          plan generated for a given unmatched function call. Common feature adoption is a Basalt feature
 *          that allows to automatically "generate" a function overload based on the other already existing
 *          ones. The CFA-generated overload is generated in such a way that at runtime it will behave as if
 *          it was dispatching to the most appropriate existing overload based on the run-time type-informations
 *          of the arguments wich are passed to it as unions. 
 * 
 * @see     CommonFeatureAdoptionPlanGenerationEngine
 */
struct CommonFeatureAdoptionPlanDescriptor {
    
    using DirectAdoption = std::pair<std::string, FunctionDefinition::Ref>;
    using RecursiveAdoption = std::pair<std::string, CommonFeatureAdoptionPlanDescriptor>;

    std::vector<RecursiveAdoption> recursive_adoptions;
    std::vector<DirectAdoption> direct_adoptions;
};