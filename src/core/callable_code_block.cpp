//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/callable_code_block.hpp"

static std::string generate_function_definition_contexnt_independent_id(
    const FunctionDefinition::Ref& fref
) {
    std::string coordinates = fref->filename;
    coordinates += "@" + std::to_string(fref->line_number);
    coordinates += ":" + std::to_string(fref->tok_number);
    return fref->function_name + "(" + coordinates + ")";
}

static std::string generate_cfa_plan_context_independent_id(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan,
    TypeDefinitionsRegister& type_definitions_register
) {
    std::string unique_context_independent_id;
    unique_context_independent_id = "[CFA::" + cfa_plan.filename + "]";
    unique_context_independent_id += cfa_plan.function_name + "("; 
    for (const TypeSignature& ts : cfa_plan.arg_types) {
        unique_context_independent_id += type_definitions_register
            .get_fully_qualified_typesignature_name(ts) + ",";
    }
    if (!cfa_plan.arg_types.empty()) {
        unique_context_independent_id.pop_back();
    }
    unique_context_independent_id += ")";
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

[[nodiscard]] std::optional<TypeSignature> 
CallableCodeBlock::get_return_type() {
    return (ParentVariant::is<FunctionDefinition::Ref>())
        ? ParentVariant::get<FunctionDefinition::Ref>()->return_type
        : ParentVariant::get<CommonFeatureAdoptionPlanDescriptor>().return_type;
}