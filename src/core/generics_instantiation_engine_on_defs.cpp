//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/typesignatures.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"

StructDefinition GenericsInstantiationEngine::instantiate_generic_struct(
    const StructDefinition& struct_definition,
    const std::string& new_struct_name
) {
    DebugInformationsAwareEntity debug_info = struct_definition.as_debug_informations_aware_entity();
    StructDefinition instantiated_struct_def(new_struct_name, debug_info);
    for (const StructDefinition::Field& field : struct_definition.fields) {
        TypeSignature instantiated_field_type = instantiate_generic_typesignature(field.field_type);
        instantiated_struct_def.fields.push_back({ field.field_name, instantiated_field_type });
    }
    return instantiated_struct_def;
}

UnionDefinition GenericsInstantiationEngine::instantiate_generic_union(
    const UnionDefinition& union_definition,
    const std::string& new_union_name
) {
    DebugInformationsAwareEntity debug_info = union_definition.as_debug_informations_aware_entity();
    UnionDefinition instantiated_union_def(new_union_name, debug_info);
    for (const TypeSignature& type : union_definition.types) {
        instantiated_union_def.types.push_back(instantiate_generic_typesignature(type));
    }
    return instantiated_union_def;
}

TypeAlias GenericsInstantiationEngine::instantiate_generic_alias(
    const TypeAlias& type_alias,
    const std::string& new_type_name
) {
    return TypeAlias(
        new_type_name,
        type_alias.as_debug_informations_aware_entity(),
        instantiate_generic_typesignature(type_alias.aliased_type)
    );
}

TypeDefinition GenericsInstantiationEngine::instantiate_generic_typedefinition(
    const TypeDefinition& type_definition,
    const std::string& new_type_name
) {
    if (type_definition.is<StructDefinition>()) {
        return instantiate_generic_struct(type_definition.get<StructDefinition>(), new_type_name);
    }
    if (type_definition.is<UnionDefinition>()) {
        return instantiate_generic_union(type_definition.get<UnionDefinition>(), new_type_name);
    }
    if (type_definition.is<TypeAlias>()) {
        return instantiate_generic_alias(type_definition.get<TypeAlias>(), new_type_name);
    }
    assert_unreachable();
}

FunctionDefinition::Ref GenericsInstantiationEngine::instantiate_generic_function(
    const FunctionDefinition& function_definition, 
    const std::string& new_function_name
) {
    DebugInformationsAwareEntity debug_info = function_definition.as_debug_informations_aware_entity();
    FunctionDefinition::Ref instantiated_function = std::make_shared<FunctionDefinition>(new_function_name, debug_info);
    if (function_definition.return_type.has_value()) {
        instantiated_function->return_type = instantiate_generic_typesignature(*function_definition.return_type);
    }
    for (const auto& arg : function_definition.arguments) {
        instantiated_function->arguments.push_back({ arg.arg_name, instantiate_generic_typesignature(arg.arg_type) });
    }
    for (const auto& statement : function_definition.code) {
        instantiated_function->code.push_back(instantiate_generic_statement(statement));
    }
    return instantiated_function;
};