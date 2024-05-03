
#pragma once
#include "toolchain/tokenizer.hpp"
#include "language/typesystem.hpp"
#include "language/statements.hpp"
#include "language/expressions.hpp"
#include "language/syntax.hpp"
#include "misc/debug_informations_aware_entity.h"
#include <vector>
#include <string>
#include <iostream>
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

struct TypeDefinition : public std::variant<StructDefinition, UnionDefinition> {

    using std::variant<StructDefinition, UnionDefinition>::variant;
    using std::variant<StructDefinition, UnionDefinition>::operator=;
    using std::variant<StructDefinition, UnionDefinition>::index;
    
    template <typename T> [[nodiscard]] bool is() const {
        return std::holds_alternative<T>(*this);
    }

    template <typename T> [[nodiscard]] const T& get() const {
        return std::get<T>(*this);
    }

    template <typename T> [[nodiscard]] T& get() {
        return std::get<T>(*this);
    }
};