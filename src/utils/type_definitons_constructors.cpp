//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/definitions.hpp"

FunctionDefinition::FunctionDefinition(const Token& func_token)
    : DebugInformationsAwareEntity(func_token) 
    , function_name(func_token.sourcetext)
{}

FunctionDefinition::FunctionDefinition(
    const std::string& function_name, 
    const DebugInformationsAwareEntity& debug_info
)
    : DebugInformationsAwareEntity(debug_info)
    , function_name(function_name)
{}

StructDefinition::StructDefinition(const Token& struct_token)
    : DebugInformationsAwareEntity(struct_token)
    , def_name(struct_token.sourcetext)
{}

StructDefinition::StructDefinition(
    const std::string& struct_name, 
    const DebugInformationsAwareEntity& debug_info
)
    : DebugInformationsAwareEntity(debug_info)
    , def_name(struct_name)
{}

UnionDefinition::UnionDefinition(const Token& union_token)
    : DebugInformationsAwareEntity(union_token)
    , def_name(union_token.sourcetext)
{}

UnionDefinition::UnionDefinition(
    const std::string& union_name, 
    const DebugInformationsAwareEntity& debug_info
)
    : DebugInformationsAwareEntity(debug_info)
    , def_name(union_name)
{}

TypeAlias::TypeAlias(
    const Token& alias_token, 
    const std::vector<std::string>& template_generics_names, 
    const TypeSignature& aliased_type
)
    : DebugInformationsAwareEntity(alias_token)
    , def_name(alias_token.sourcetext)
    , template_generics_names(template_generics_names)
    , aliased_type(aliased_type)
{}

TypeAlias::TypeAlias(
    const std::string& alias_name, 
    DebugInformationsAwareEntity debug_info,
    const TypeSignature& aliased_type
)
    : DebugInformationsAwareEntity(debug_info)
    , def_name(alias_name)
    , template_generics_names()
    , aliased_type(aliased_type)
{}