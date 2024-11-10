//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <memory>
#include <string>
#include <map>
#include <variant>
#include "language/definitions.hpp"

struct CommonFeatureAdoptionPlanDescriptor;

struct RecursiveAdoptionPlan {
    size_t argument_index;
    std::vector<TypeSignature> alternatives;
    std::vector<CommonFeatureAdoptionPlanDescriptor> nested_plans;
};

struct CommonFeatureAdoptionPlanDescriptor 
    : public std::variant<RecursiveAdoptionPlan, FunctionDefinition::Ref>
{
    using std::variant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::variant;
    using std::variant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::operator=;

    bool is_direct_adoption() const {
        return std::holds_alternative<FunctionDefinition::Ref>(*this);
    }

    bool is_recursive_adoption() const {
        return std::holds_alternative<RecursiveAdoptionPlan>(*this);
    }

    FunctionDefinition::Ref get_direct_adoption() const {
        return std::get<FunctionDefinition::Ref>(*this);
    }

    RecursiveAdoptionPlan get_recursive_adoption() const {
        return std::get<RecursiveAdoptionPlan>(*this);
    }
};