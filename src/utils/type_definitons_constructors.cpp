
#include "language/definitions.hpp"

FunctionDefinition::FunctionDefinition(const Token& func_token)
    : DebugInformationsAwareEntity(func_token) 
    , function_name(func_token.sourcetext)
{}

StructDefinition::StructDefinition(const Token& struct_token)
    : DebugInformationsAwareEntity(struct_token)
    , struct_name(struct_token.sourcetext)
{}

UnionDefinition::UnionDefinition(const Token& union_token)
    : DebugInformationsAwareEntity(union_token)
    , union_name(union_token.sourcetext)
{}

TypeAlias::TypeAlias(
    const Token& alias_token, 
    const std::vector<std::string>& template_generics_names, 
    const TypeSignature& aliased_type
)
    : DebugInformationsAwareEntity(alias_token)
    , alias_name(alias_token.sourcetext)
    , template_generics_names(template_generics_names)
    , aliased_type(aliased_type)
{}