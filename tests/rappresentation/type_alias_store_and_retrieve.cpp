
#include <gtest/gtest.h>
#include "toolchain/rappresentation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(Rappresentation, Simple_Type_Alias_Store_And_Retrieve) {
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "MyStruct", 
        StructDefinitionFactory::no_generics, 
        StructDefinitionFactory::no_fields
    );
    
    TypeAlias type_alias = TypeAliasFactory::make_type_alias(
        "MyAlias", 
        TypeAliasFactory::no_generics, 
        TypeSignatureFactory::make_base_type("MyStruct", {})
    );

    TypeDefinitionsRegister type_register;
    type_register.store(struct_definition);
    type_register.store(type_alias);

    TypeSignature alias_type = TypeSignatureFactory::make_base_type("MyAlias", {});
    TypeDefinition retrieved = type_register.retrieve(alias_type);
    ASSERT_TRUE(retrieved.is<TypeAlias>());
    EXPECT_EQ(retrieved.get<TypeAlias>().alias_name, type_alias.alias_name);
    EXPECT_EQ(retrieved.get<TypeAlias>().aliased_type.to_string(), type_alias.aliased_type.to_string());
    EXPECT_EQ(retrieved.get<TypeAlias>().template_generics_names, type_alias.template_generics_names);
    retrieved = type_register.retrieve(retrieved.get<TypeAlias>().aliased_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    EXPECT_EQ(retrieved.get<StructDefinition>().struct_name, struct_definition.struct_name);
    EXPECT_EQ(retrieved.get<StructDefinition>().fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved.get<StructDefinition>().filename, struct_definition.filename);
    EXPECT_EQ(retrieved.get<StructDefinition>().template_generics_names, struct_definition.template_generics_names);
}

TEST(Rappresentation, Type_Alias_Of_Instantiated_Generic_Struct_Store_And_Retrieve) {
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "MyStruct", 
        { "T"}, 
        StructDefinitionFactory::no_fields
    );
    
    TypeAlias type_alias = TypeAliasFactory::make_type_alias(
        "MyAlias", 
        TypeAliasFactory::no_generics, 
        TypeSignatureFactory::make_base_type("MyStruct", { TypeSignatureFactory::Int })
    );

    TypeDefinitionsRegister type_register;
    type_register.store(struct_definition);
    type_register.store(type_alias);

    TypeSignature alias_type = TypeSignatureFactory::make_base_type("MyAlias", {});
    TypeDefinition retrieved = type_register.retrieve(alias_type);
    ASSERT_TRUE(retrieved.is<TypeAlias>());
    EXPECT_EQ(retrieved.get<TypeAlias>().alias_name, type_alias.alias_name);
    EXPECT_EQ(retrieved.get<TypeAlias>().aliased_type.to_string(), type_alias.aliased_type.to_string());
    EXPECT_EQ(retrieved.get<TypeAlias>().template_generics_names, type_alias.template_generics_names);
    retrieved = type_register.retrieve(retrieved.get<TypeAlias>().aliased_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    EXPECT_EQ(retrieved.get<StructDefinition>().struct_name, "MyStruct<Int>");
    EXPECT_TRUE(retrieved.get<StructDefinition>().fields.empty());
    EXPECT_TRUE(retrieved.get<StructDefinition>().template_generics_names.empty());
}

TEST(Rappresentation, Type_Alias_Of_Instantiated_Generic_Struct_With_Fields_Store_And_Retrieve) {
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "Wrapper", 
        { "T"}, 
        { StructDefinition::Field { "wrapped", TypeSignatureFactory::T } }
    );
    
    TypeAlias type_alias = TypeAliasFactory::make_type_alias(
        "IntWrapper", 
        TypeAliasFactory::no_generics, 
        TypeSignatureFactory::make_base_type("Wrapper", { TypeSignatureFactory::Int })
    );

    TypeDefinitionsRegister type_register;
    type_register.store(struct_definition);
    type_register.store(type_alias);

    TypeSignature alias_type = TypeSignatureFactory::make_base_type("IntWrapper", {});
    TypeDefinition retrieved = type_register.retrieve(alias_type);
    ASSERT_TRUE(retrieved.is<TypeAlias>());
    EXPECT_EQ(retrieved.get<TypeAlias>().alias_name, type_alias.alias_name);
    EXPECT_EQ(retrieved.get<TypeAlias>().aliased_type.to_string(), type_alias.aliased_type.to_string());
    EXPECT_TRUE(retrieved.get<TypeAlias>().template_generics_names.empty());
    retrieved = type_register.retrieve(retrieved.get<TypeAlias>().aliased_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    EXPECT_EQ(retrieved.get<StructDefinition>().struct_name, "Wrapper<Int>");
    EXPECT_TRUE(retrieved.get<StructDefinition>().template_generics_names.empty());
    ASSERT_EQ(retrieved.get<StructDefinition>().fields.size(), 1);
    EXPECT_EQ(retrieved.get<StructDefinition>().fields[0].field_name, "wrapped");
    EXPECT_EQ(retrieved.get<StructDefinition>().fields[0].field_type.to_string(), "Int");
}

TEST(Rappresentation, Generic_Type_Alias_Of_Template_Generic_Struct_With_Fields_Store_And_Retrieve) {
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "Wrapper", 
        { "T"}, 
        { StructDefinition::Field { "wrapped", TypeSignatureFactory::T } }
    );
    
    TypeAlias type_alias = TypeAliasFactory::make_type_alias(
        "MetaWrapper", 
        { "T" }, 
        TypeSignatureFactory::make_base_type("Wrapper", { TypeSignatureFactory::T })
    );

    TypeDefinitionsRegister type_register;
    type_register.store(struct_definition);
    type_register.store(type_alias);

    TypeSignature alias_type = TypeSignatureFactory::make_base_type("MetaWrapper", { TypeSignatureFactory::Int });
    TypeDefinition retrieved = type_register.retrieve(alias_type);
    ASSERT_TRUE(retrieved.is<TypeAlias>());
    EXPECT_EQ(retrieved.get<TypeAlias>().alias_name, "MetaWrapper<Int>");
    EXPECT_EQ(retrieved.get<TypeAlias>().aliased_type.to_string(), type_alias.aliased_type.to_string());
    EXPECT_TRUE(retrieved.get<TypeAlias>().template_generics_names.empty());
    retrieved = type_register.retrieve(retrieved.get<TypeAlias>().aliased_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    EXPECT_EQ(retrieved.get<StructDefinition>().struct_name, "Wrapper<Int>");
    EXPECT_TRUE(retrieved.get<StructDefinition>().template_generics_names.empty());
    ASSERT_EQ(retrieved.get<StructDefinition>().fields.size(), 1);
    EXPECT_EQ(retrieved.get<StructDefinition>().fields[0].field_name, "wrapped");
    EXPECT_EQ(retrieved.get<StructDefinition>().fields[0].field_type.to_string(), "Int");
}