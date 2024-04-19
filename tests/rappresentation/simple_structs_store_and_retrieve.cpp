
#include <gtest/gtest.h>
#include "toolchain/rappresentation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/rappresentation_inline_procs.hpp"

TEST(Rappresentation, Trivial_Struct_Store_And_Retrieval_From_Global_Scope) {
    TypeDefinitionsRegister struct_register;
    StructDefinition struct_definition = make_struct_definition("MyStruct");
    struct_register.store(struct_definition);
    TypeSignature struct_type = CustomType { "MyStruct" , {} };
    TypeDefinition retrieved = struct_register.retrieve(struct_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, struct_definition.struct_name);
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
}

TEST(Rappresentation, Struct_With_One_Generic_Store_And_Retrieval_From_Global_Scope) {
    TypeDefinitionsRegister struct_register;
    StructDefinition struct_definition = make_struct_definition("MyStruct");
    struct_definition.template_generics_names.push_back("T");
    struct_register.store(struct_definition);
    TypeSignature struct_type = CustomType { "MyStruct" , { CustomType { "Int", {} } } };
    TypeDefinition retrieved = struct_register.retrieve(struct_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.struct_name, struct_definition.struct_name);
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
}

TEST(Rappresentation, Instanciated_Struct_With_One_Generic_Store_And_Retrieval_From_Global_Scope) {
    TypeDefinitionsRegister struct_register;
    StructDefinition generic_struct_definition = make_struct_definition("MyStruct");
    generic_struct_definition.template_generics_names.push_back("T");
    StructDefinition struct_definition = make_struct_definition("MyStruct<Int>");
    struct_register.store(generic_struct_definition);
    struct_register.store(struct_definition);
    TypeSignature struct_type = CustomType { "MyStruct" , { CustomType { "Int", {} } } };
    TypeDefinition retrieved = struct_register.retrieve(struct_type);
    ASSERT_TRUE(retrieved.is<StructDefinition>());
    StructDefinition retrieved_struct = retrieved.get<StructDefinition>();
    EXPECT_EQ(retrieved_struct.fields.size(), struct_definition.fields.size());
    EXPECT_EQ(retrieved_struct.filename, struct_definition.filename);
    EXPECT_EQ(retrieved_struct.template_generics_names, struct_definition.template_generics_names);
}