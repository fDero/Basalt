
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "errors/internal_errors.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_queries.hpp"

TEST(Core, Generic_Union_Instantiation) {
    UnionDefinition generic_union_definition = UnionDefinitionFactory::make_union_definition(
        "MyUnion", 
        "test.basalt",
        { "T", "U" }, 
        { 
            TypeSignatureFactory::make_ptr_type(TypeSignatureFactory::T),
            TypeSignatureFactory::make_custom_type("Pair", { TypeSignatureFactory::T, TypeSignatureFactory::U }),
            TypeSignatureFactory::make_slice_type(TypeSignatureFactory::U),
            TypeSignatureFactory::make_array_type(TypeSignatureFactory::T, 10)
        }
    );
    GenericsInstantiationEngine engine({
        GenericSubstitutionRule { "T", TypeSignatureFactory::Int },
        GenericSubstitutionRule { "U", TypeSignatureFactory::Float }
    });
    UnionDefinition instantiated_union_definition = engine.instantiate_generic_union(generic_union_definition, "InstantiatedMyUnion");
    ASSERT_EQ(instantiated_union_definition.types.size(), 4);
    EXPECT_TRUE(is_pointer_to_int(instantiated_union_definition.types[0]));
    CustomType pair_type = instantiated_union_definition.types[1].get<CustomType>();
    EXPECT_EQ(pair_type.type_name, "Pair");
    ASSERT_EQ(pair_type.type_parameters.size(), 2);
    EXPECT_TRUE(is_int(pair_type.type_parameters[0]));
    EXPECT_TRUE(is_float(pair_type.type_parameters[1]));
    EXPECT_TRUE(is_slice_of_floats(instantiated_union_definition.types[2]));
    EXPECT_TRUE(is_array_of_ints_of_size_n(instantiated_union_definition.types[3], 10));
}

TEST(Core, Non_Generic_Union_NoOp_Instantiation) {
    UnionDefinition generic_union_definition = UnionDefinitionFactory::make_union_definition(
        "MyUnion",
        "test.basalt", 
        {},
        { 
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        }
    );
    GenericsInstantiationEngine engine({});
    UnionDefinition instantiated_union_definition = engine.instantiate_generic_union(generic_union_definition, "InstantiatedMyUnion");
    ASSERT_EQ(instantiated_union_definition.types.size(), 2);
    EXPECT_TRUE(is_int(instantiated_union_definition.types[0]));
    EXPECT_TRUE(is_float(instantiated_union_definition.types[1]));
}

TEST(Core, Fake_Generic_Union_NoOp_Instantiation) {
    UnionDefinition generic_union_definition = UnionDefinitionFactory::make_union_definition(
        "MyUnion", 
        "test.basalt",
        { "T" },
        { 
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        }
    );
    GenericsInstantiationEngine engine({
        GenericSubstitutionRule { "T", TypeSignatureFactory::Int }
    });
    UnionDefinition instantiated_union_definition = engine.instantiate_generic_union(generic_union_definition, "InstantiatedMyUnion");
    ASSERT_EQ(instantiated_union_definition.types.size(), 2);
    EXPECT_TRUE(is_int(instantiated_union_definition.types[0]));
    EXPECT_TRUE(is_float(instantiated_union_definition.types[1]));
}