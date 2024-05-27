
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(TypeSystem, Generic_Union_Instantiation) {
    UnionDefinition generic_union_definition = UnionDefinitionFactory::make_union_definition(
        "MyUnion", 
        { "T", "U" }, 
        { 
            TypeSignatureFactory::make_ptr_type(TypeSignatureFactory::T),
            TypeSignatureFactory::make_custom_type("Pair", { TypeSignatureFactory::T, TypeSignatureFactory::U }),
            TypeSignatureFactory::make_slice_type(TypeSignatureFactory::U),
            TypeSignatureFactory::make_array_type(TypeSignatureFactory::T, 10)
        }
    );
    UnionDefinition instantiated_union_definition = generic_union_definition;
    instantiated_union_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("MyUnion", { 
            TypeSignatureFactory::Int, 
            TypeSignatureFactory::Float 
        })
            .get<CustomType>()
    );
    EXPECT_EQ(instantiated_union_definition.union_name, "MyUnion<Int,Float>");
    ASSERT_EQ(instantiated_union_definition.types.size(), 4);
    EXPECT_EQ(instantiated_union_definition.types[0].to_string(), "#Int");
    EXPECT_EQ(instantiated_union_definition.types[1].to_string(), "Pair<Int,Float>");
    EXPECT_EQ(instantiated_union_definition.types[2].to_string(), "$Float");
    EXPECT_EQ(instantiated_union_definition.types[3].to_string(), "[10]Int");
}

TEST(TypeSystem, Non_Generic_Union_NoOp_Instantiation) {
    UnionDefinition generic_union_definition = UnionDefinitionFactory::make_union_definition(
        "MyUnion", 
        {},
        { 
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        }
    );
    UnionDefinition instantiated_union_definition = generic_union_definition;
    instantiated_union_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("MyUnion", {}).get<CustomType>()
    );
    EXPECT_EQ(instantiated_union_definition.union_name, "MyUnion");
    ASSERT_EQ(instantiated_union_definition.types.size(), 2);
    EXPECT_EQ(instantiated_union_definition.types[0].to_string(), "Int");
    EXPECT_EQ(instantiated_union_definition.types[1].to_string(), "Float");
}

TEST(TypeSystem, Fake_Generic_Union_NoOp_Instantiation) {
    UnionDefinition generic_union_definition = UnionDefinitionFactory::make_union_definition(
        "MyUnion", 
        { "T" },
        { 
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        }
    );
    UnionDefinition instantiated_union_definition = generic_union_definition;
    instantiated_union_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("MyUnion", { 
            TypeSignatureFactory::Int
        })
            .get<CustomType>()
    );
    EXPECT_EQ(instantiated_union_definition.union_name, "MyUnion<Int>");
    ASSERT_EQ(instantiated_union_definition.types.size(), 2);
    EXPECT_EQ(instantiated_union_definition.types[0].to_string(), "Int");
    EXPECT_EQ(instantiated_union_definition.types[1].to_string(), "Float");
}