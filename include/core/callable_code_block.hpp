//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/type_definitions_register.hpp"
#include "language/typesignatures.hpp"
#include "language/definitions.hpp"
#include "misc/smart_variant.hpp"
#include "misc/forward_declarations.hpp"
#include "core/common_feature_adoption_plan_descriptor.hpp"

struct CallableCodeBlock : public SmartVariant<
    FunctionDefinition::Ref,
    CommonFeatureAdoptionPlanDescriptor
> {
    enum class Kind {
        function_definition,
        common_feature_adoption_plan
    };

    using FRef = FunctionDefinition::Ref;
    using CFAPlan = CommonFeatureAdoptionPlanDescriptor;
    using ParentVariant = SmartVariant<FRef, CFAPlan>;

    using ParentVariant::operator=;
    using ParentVariant::is;
    using ParentVariant::get;
    
    [[nodiscard]] std::optional<TypeSignature> get_return_type() const;
    
    [[nodiscard]] Kind callable_codeblock_kind() const;

    [[nodiscard]] std::vector<TypeSignature> get_arg_types() const;

    const std::string unique_context_independent_id;

    CallableCodeBlock(const ParentVariant& variant, TypeDefinitionsRegister& type_definitions_register);
    CallableCodeBlock(const ParentVariant& variant, ProgramRepresentation& type_definitions_register);
};
