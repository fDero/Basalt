//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "language/definitions.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "misc/forward_declarations.hpp"
#include "misc/smart_variant.hpp"


struct RecursiveAdoptionPlan {
    size_t argument_index;
    std::vector<TypeSignature> alternatives;
    std::vector<CommonFeatureAdoptionPlan> nested_plans;
};

struct CommonFeatureAdoptionPlan
    : public SmartVariant<RecursiveAdoptionPlan, FunctionDefinition::Ref>
{
    using SmartVariant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::is;
    using SmartVariant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::get;
    using SmartVariant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::SmartVariant;
    using SmartVariant<RecursiveAdoptionPlan, FunctionDefinition::Ref>::operator=;

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
    DebugInformationsAwareEntity debug_info;
    CommonFeatureAdoptionPlan plan;
};
