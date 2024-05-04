
#include <gtest/gtest.h>
#include "toolchain/rappresentation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Rappresentation, Trivial_Struct_Store_And_Retrieval_From_Global_Scope) {
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "MyStruct", 
        StructDefinitionFactory::no_generics, 
        StructDefinitionFactory::no_fields
    );
    TypeDefinitionsRegister struct_register;
    struct_register.store(struct_definition);
    TypeSignature struct_type = TypeSignatureFactory::make_base_type("MyStruct", {});
    TypeDefinition retrieved = struct_register.retrieve(struct_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, struct_definition.struct_name);
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
}

TEST(Rappresentation, Struct_With_One_Generic_Store_And_Retrieval_From_Global_Scope) {
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "MyStruct", 
        { "T" }, 
        StructDefinitionFactory::no_fields
    );
    TypeDefinitionsRegister struct_register;
    struct_register.store(struct_definition);
    TypeSignature struct_type = TypeSignatureFactory::make_base_type("MyStruct", { TypeSignatureFactory::Int });
    TypeDefinition retrieved = struct_register.retrieve(struct_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, struct_definition.struct_name);
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
}

TEST(Rappresentation, instantiationd_Struct_With_One_Generic_Store_And_Retrieval_From_Global_Scope) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "MyStruct<T>", 
        { "T" }, 
        StructDefinitionFactory::no_fields
    );
    StructDefinition struct_definition = StructDefinitionFactory::make_struct_definition(
        "MyStruct<Int>", 
        StructDefinitionFactory::no_generics, 
        StructDefinitionFactory::no_fields
    );
    TypeDefinitionsRegister struct_register;
    struct_register.store(generic_struct_definition);
    struct_register.store(struct_definition);
    TypeSignature struct_type = TypeSignatureFactory::make_base_type("MyStruct", { TypeSignatureFactory::Int });
    TypeDefinition retrieved = struct_register.retrieve(struct_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
}