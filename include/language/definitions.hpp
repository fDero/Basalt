
#pragma once
#include "toolchain/tokenizer.hpp"
#include "language/typesystem.hpp"
#include "language/statements.hpp"
#include "language/expressions.hpp"
#include "language/syntax.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "misc/smart_variant.hpp"
#include <vector>
#include <string>
#include <variant>

struct FunctionDefinition : public DebugInformationsAwareEntity {
    
    struct Argument {
        std::string arg_name;
        TypeSignature arg_type;
    };

    std::string function_name;
    std::optional<TypeSignature> return_type;
    std::vector<std::string> template_generics_names;
    std::vector<Argument> arguments;
    std::vector<Statement> code;

    FunctionDefinition(const Token& func_token)
        : DebugInformationsAwareEntity(func_token) 
        , function_name(func_token.sourcetext)
    {}
};


struct StructDefinition : public DebugInformationsAwareEntity  {

    struct Field {
        std::string field_name;
        TypeSignature field_type;
    };

    std::string struct_name;
    std::vector<Field> fields;
    std::vector<std::string> template_generics_names;

    StructDefinition(const Token& struct_token)
        : DebugInformationsAwareEntity(struct_token)
        , struct_name(struct_token.sourcetext)
    {}

    [[nodiscard]] std::string generate_struct_id() const;
    [[nodiscard]] std::string generate_match_pattern() const;
    void instantiate_generics(const BaseType& concrete_type);
};

struct UnionDefinition : public DebugInformationsAwareEntity  {

    std::string union_name;
    std::vector<TypeSignature> types;
    std::vector<std::string> template_generics_names;

    UnionDefinition(const Token& union_token)
        : DebugInformationsAwareEntity(union_token)
        , union_name(union_token.sourcetext)
    {}

    [[nodiscard]] std::string generate_union_id() const;
    [[nodiscard]] std::string generate_match_pattern() const;
    void instantiate_generics(const BaseType& concrete_type);
};

struct TypeAlias : public DebugInformationsAwareEntity {

    std::string alias_name;
    std::vector<std::string> template_generics_names;
    TypeSignature aliased_type;

    TypeAlias(
        const Token& alias_token, 
        const std::vector<std::string>& template_generics_names, 
        const TypeSignature& aliased_type
    )
        : DebugInformationsAwareEntity(alias_token)
        , alias_name(alias_token.sourcetext)
        , template_generics_names(template_generics_names)
        , aliased_type(aliased_type)
    {}

    [[nodiscard]] std::string generate_alias_id() const;
    [[nodiscard]] std::string generate_match_pattern() const;
    void instantiate_generics(const BaseType& concrete_type);
};

struct TypeDefinition 
    : public SmartVariant<StructDefinition,UnionDefinition,TypeAlias> 
{
    using SmartVariant::SmartVariant;
    using SmartVariant::operator=;
    using SmartVariant::index;
    
    using SmartVariant::get;
    using SmartVariant::is;

    [[nodiscard]] std::string generate_id() const;
    [[nodiscard]] std::string generate_match_pattern() const;
    void instantiate_generics(const BaseType& concrete_type);
};