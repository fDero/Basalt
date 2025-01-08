//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/type_definitions_register.hpp"
#include "language/typesignatures.hpp"
#include "language/definitions.hpp"
#include "misc/smart_variant.hpp"
#include "core/common_feature_adoption_plan_descriptor.hpp"

struct CallableCodeBlock : public SmartVariant<
    FunctionDefinition::Ref,
    CommonFeatureAdoptionPlanDescriptor
> {
    using FRef = FunctionDefinition::Ref;
    using CFAPlan = CommonFeatureAdoptionPlanDescriptor;
    using ParentVariant = SmartVariant<FRef, CFAPlan>;

    using ParentVariant::operator=;
    using ParentVariant::is;
    using ParentVariant::get;
    
    const std::string unique_context_independent_id;

    CallableCodeBlock(const ParentVariant& variant, TypeDefinitionsRegister& type_definitions_register);
    [[nodiscard]] std::optional<TypeSignature> get_return_type();
};
