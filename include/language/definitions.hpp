/**
 * @file definitions.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of FunctionDefinition, StructDefinition, UnionDefinition, TypeAlias, TypeDefinition 
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

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

/**
 * @brief   Used to represent a function definition in the source-code.
 * 
 * @details The FunctionDefinition struct is used to represent a function definition in the source-code,
 *          it contains the name of the function, the return type, the arguments, the code of the function.
 *          It's supposed to be the root of an AST representing the function iteself.
 * 
 */
struct FunctionDefinition : public DebugInformationsAwareEntity {
    
    using Ref = std::shared_ptr<FunctionDefinition>;
    using OverloadSet = std::vector<Ref>;

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
};

/**
 * @brief   Used to represent a struct definition in the source-code.
 * 
 * @details The StructDefinition struct is used to represent a struct definition in the source-code,
 *          it contains the name of the struct, the fields of the struct, and the template generics names.
 *          It's supposed to be the root of an AST representing the struct itself.
 * 
 */
struct StructDefinition : public DebugInformationsAwareEntity  {

    StructDefinition(const Token& struct_token);

    struct Field {
        std::string field_name;
        TypeSignature field_type;
    };

    std::string def_name;
    std::vector<Field> fields;
    std::vector<std::string> template_generics_names;

    [[nodiscard]] std::string generate_struct_id() const;
};

/**
 * @brief   Used to represent a union definition in the source-code.
 * 
 * @details The UnionDefinition struct is used to represent a union definition in the source-code,
 *          it contains the name of the union, the types of the union, and the template generics names.
 *          It's supposed to be the root of an AST representing the union itself.
 */
struct UnionDefinition : public DebugInformationsAwareEntity  {

    UnionDefinition(const Token& union_token);

    std::string def_name;
    std::vector<TypeSignature> types;
    std::vector<std::string> template_generics_names;

    [[nodiscard]] std::string generate_union_id() const;
};

/**
 * @brief   Used to represent a type alias in the source-code.
 * 
 * @details The TypeAlias struct is used to represent a type alias in the source-code,
 *          it contains the name of the alias, the type that is being aliased, and the template generics names.
 *          It's supposed to be the root of an AST representing the alias itself.
 */
struct TypeAlias : public DebugInformationsAwareEntity {

    TypeAlias(
        const Token& alias_token, 
        const std::vector<std::string>& template_generics_names, 
        const TypeSignature& aliased_type
    );

    std::string def_name;
    std::vector<std::string> template_generics_names;
    TypeSignature aliased_type;

    [[nodiscard]] std::string generate_alias_id() const;
};

/**
 * @brief   Used to represent a type definition in the source-code.
 * 
 * @details It's a value semantics type that can be either a StructDefinition, UnionDefinition, or TypeAlias.
 *          It's used to represent a type definition in the source-code, wich you don't know anything about.
 */
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