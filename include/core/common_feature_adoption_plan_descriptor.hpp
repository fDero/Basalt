//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <memory>
#include <string>
#include <map>
#include <variant>

#include "misc/forward_declarations.hpp"
#include "language/definitions.hpp"

struct RecursiveAdoptionPlan {
    size_t argument_index;
    std::vector<TypeSignature> alternatives;
    std::vector<CommonFeatureAdoptionPlan> nested_plans;
};

struct CommonFeatureAdoptionPlan 
    : public std::variant<RecursiveAdoptionPlan, FunctionDefinition::Ref>
{
    using std::variant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::variant;
    using std::variant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::operator=;

    [[nodiscard]] bool is_direct_adoption() const {
        return std::holds_alternative<FunctionDefinition::Ref>(*this);
    }

    [[nodiscard]] bool is_recursive_adoption() const {
        return std::holds_alternative<RecursiveAdoptionPlan>(*this);
    }

    [[nodiscard]] FunctionDefinition::Ref get_direct_adoption() const {
        return std::get<FunctionDefinition::Ref>(*this);
    }

    [[nodiscard]] RecursiveAdoptionPlan get_recursive_adoption() const {
        return std::get<RecursiveAdoptionPlan>(*this);
    }
};

struct CommonFeatureAdoptionPlanDescriptor {
    std::string function_name;
    std::vector<TypeSignature> arg_types;
    std::optional<TypeSignature> return_type;
    std::string filename;
    CommonFeatureAdoptionPlan plan;
};