//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>

#include "frontend/tokenizer.hpp"
#include "language/typesignatures.hpp"
#include "language/statements.hpp"
#include "language/expressions.hpp"
#include "frontend/syntax.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "misc/smart_variant.hpp"

struct FunctionDefinition : public DebugInformationsAwareEntity {
    
    using Ref = std::shared_ptr<FunctionDefinition>;
    using OverloadSet = std::vector<Ref>;

    FunctionDefinition(const Token& func_token);
    
    FunctionDefinition(
        const std::string& function_name, 
        const DebugInformationsAwareEntity& debug_info
    );

    struct Argument {
        std::string arg_name;
        TypeSignature arg_type;
    };

    std::string function_name;
    std::optional<TypeSignature> return_type;
    std::vector<std::string> template_generics_names;
    std::vector<Argument> arguments;
    std::vector<Statement> code;
};

struct StructDefinition : public DebugInformationsAwareEntity  {

    StructDefinition(const Token& struct_token);

    StructDefinition(
        const std::string& struct_name, 
        const DebugInformationsAwareEntity& debug_info
    );

    struct Field {
        std::string field_name;
        TypeSignature field_type;
    };

    std::string def_name;
    std::vector<Field> fields;
    std::vector<std::string> template_generics_names;

    [[nodiscard]] std::string generate_struct_id() const;
};

struct UnionDefinition : public DebugInformationsAwareEntity  {

    UnionDefinition(const Token& union_token);

    UnionDefinition(
        const std::string& union_name, 
        const DebugInformationsAwareEntity& debug_info
    );

    std::string def_name;
    std::vector<TypeSignature> types;
    std::vector<std::string> template_generics_names;

    [[nodiscard]] std::string generate_union_id() const;
};

struct TypeAlias : public DebugInformationsAwareEntity {

    TypeAlias(
        const Token& alias_token, 
        const std::vector<std::string>& template_generics_names, 
        const TypeSignature& aliased_type
    );

    TypeAlias(
        const std::string& alias_name, 
        DebugInformationsAwareEntity debug_info,
        const TypeSignature& aliased_type
    );

    std::string def_name;
    std::vector<std::string> template_generics_names;
    TypeSignature aliased_type;

    [[nodiscard]] std::string generate_alias_id() const;
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
    [[nodiscard]] const std::string& get_filename() const;

    [[nodiscard]] const std::vector<std::string>& get_template_generics() const;
    void set_name(const std::string& name);
};