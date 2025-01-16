//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/callable_code_block.hpp"
#include "core/program_representation.hpp"

static std::string generate_function_definition_contexnt_independent_id(
    const FunctionDefinition::Ref& fref
) {
    std::string coordinates = fref->filename;
    coordinates += ":" + std::to_string(fref->line_number);
    coordinates += ":" + std::to_string(fref->tok_number);
    return fref->function_name + "@" + coordinates;
}

template <class FullyQualifiedTypeSignatureNameGenerator>
static std::string generate_cfa_plan_context_independent_id(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan,
    FullyQualifiedTypeSignatureNameGenerator& type_definitions_register
) {
    std::string unique_context_independent_id;
    unique_context_independent_id = "[CFA::" + cfa_plan.filename + "::";
    for (const TypeSignature& ts : cfa_plan.arg_types) {
        unique_context_independent_id += type_definitions_register
            .get_fully_qualified_typesignature_name(ts) + ",";
    }
    if (!cfa_plan.arg_types.empty()) {
        unique_context_independent_id.pop_back();
    }
    return unique_context_independent_id;
}

CallableCodeBlock::CallableCodeBlock(
    const ParentVariant& variant, 
    TypeDefinitionsRegister& type_definitions_register
)
    : ParentVariant(variant)
    , unique_context_independent_id(
        (variant.is<FunctionDefinition::Ref>())
        ? generate_function_definition_contexnt_independent_id(variant.get<FunctionDefinition::Ref>())
        : generate_cfa_plan_context_independent_id(
            variant.get<CommonFeatureAdoptionPlanDescriptor>(), 
            type_definitions_register
        )
    )
{}

CallableCodeBlock::CallableCodeBlock(
    const ParentVariant& variant, 
    ProgramRepresentation& program_representation
)
    : ParentVariant(variant)
    , unique_context_independent_id(
        (variant.is<FunctionDefinition::Ref>())
        ? generate_function_definition_contexnt_independent_id(variant.get<FunctionDefinition::Ref>())
        : generate_cfa_plan_context_independent_id(
            variant.get<CommonFeatureAdoptionPlanDescriptor>(), 
            program_representation
        )
    )
{}

[[nodiscard]] CallableCodeBlock::Kind 
CallableCodeBlock::callable_codeblock_kind() const {
    return (ParentVariant::is<FunctionDefinition::Ref>())
        ? Kind::function_definition
        : Kind::common_feature_adoption_plan;
}

[[nodiscard]] std::optional<TypeSignature>
CallableCodeBlock::get_return_type() const {
    return (callable_codeblock_kind() == Kind::function_definition)
        ? ParentVariant::get<FunctionDefinition::Ref>()->return_type
        : ParentVariant::get<CommonFeatureAdoptionPlanDescriptor>().return_type;
}

[[nodiscard]] std::vector<TypeSignature>
CallableCodeBlock::get_arg_types() const {
    if (callable_codeblock_kind() == Kind::common_feature_adoption_plan) {
        return ParentVariant::get<CommonFeatureAdoptionPlanDescriptor>().arg_types;
    }
    std::vector<TypeSignature> arg_types;
    const FunctionDefinition::Ref& fref = ParentVariant::get<FunctionDefinition::Ref>();
    for (const FunctionDefinition::Argument& arg : fref->arguments) {
        arg_types.push_back(arg.arg_type);
    }
    return arg_types;
}