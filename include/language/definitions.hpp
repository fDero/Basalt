
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
    
    using Ref = std::shared_ptr<FunctionDefinition>;

    FunctionDefinition(const Token& func_token);

    struct Argument {
        std::string arg_name;
        TypeSignature arg_type;
    };

    std::string function_name;
    std::optional<TypeSignature> return_type;
    std::vector<std::string> template_generics_names;
    std::vector<Argument> arguments;
    std::vector<Statement> code;

    void instantiate_generics(const GenericSubstitutionRuleSet& generics_substitutions) {
        for (Argument& arg : arguments) {
            arg.arg_type.instantiate_generics(generics_substitutions);
        }
    }
};

struct StructDefinition : public DebugInformationsAwareEntity  {

    StructDefinition(const Token& struct_token);

    struct Field {
        std::string field_name;
        TypeSignature field_type;
    };

    std::string struct_name;
    std::vector<Field> fields;
    std::vector<std::string> template_generics_names;

    [[nodiscard]] std::string generate_struct_id() const;
    void instantiate_generics(const CustomType& concrete_type);
};

struct UnionDefinition : public DebugInformationsAwareEntity  {

    UnionDefinition(const Token& union_token);

    std::string union_name;
    std::vector<TypeSignature> types;
    std::vector<std::string> template_generics_names;

    [[nodiscard]] std::string generate_union_id() const;
    void instantiate_generics(const CustomType& concrete_type);
};

struct TypeAlias : public DebugInformationsAwareEntity {

    TypeAlias(
        const Token& alias_token, 
        const std::vector<std::string>& template_generics_names, 
        const TypeSignature& aliased_type
    );

    std::string alias_name;
    std::vector<std::string> template_generics_names;
    TypeSignature aliased_type;

    [[nodiscard]] std::string generate_alias_id() const;
    void instantiate_generics(const CustomType& concrete_type);
};

struct TypeDefinition 
    : public SmartVariant<StructDefinition,UnionDefinition,TypeAlias> 
{
    using SmartVariant::SmartVariant;
    using SmartVariant::operator=;
    using SmartVariant::index;
    
    using SmartVariant::get;
    using SmartVariant::is;

    [[nodiscard]] std::string get_simple_name() const;
    [[nodiscard]] size_t get_number_of_generics() const;

    void instantiate_generics(const CustomType& concrete_type);
    void set_name(const std::string& name);
};