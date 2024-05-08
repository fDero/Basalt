
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
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, struct_definition.struct_name);
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
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
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, "MyStruct<Int>");
    EXPECT_TRUE(retrieved_struct.fields.empty());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_TRUE(retrieved_struct.template_generics_names.empty());
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
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, "Wrapper<Int>");
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_TRUE(retrieved_struct.template_generics_names.empty());
    ASSERT_EQ(retrieved_struct.fields.size(), 1);
    EXPECT_TRUE(retrieved_struct.fields.back().field_type.to_string() == "Int");
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
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, "Wrapper<Int>");
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_TRUE(retrieved_struct.template_generics_names.empty());
    ASSERT_EQ(retrieved_struct.fields.size(), 1);
    EXPECT_TRUE(retrieved_struct.fields.back().field_type.to_string() == "Int");
}