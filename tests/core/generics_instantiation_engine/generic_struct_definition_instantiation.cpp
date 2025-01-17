
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "errors/internal_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_queries.hpp"

TEST(Core, Generic_Struct_With_CustomType_Fields_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "Pair",
        "test.basalt",  
        { "T", "U" }, 
        { 
            StructDefinition::Field { "first",  TypeSignatureFactory::T },
            StructDefinition::Field { "second", TypeSignatureFactory::U }
        }
    );
    GenericsInstantiationEngine engine({
        GenericSubstitutionRule { "T", TypeSignatureFactory::Int },
        GenericSubstitutionRule { "U", TypeSignatureFactory::ListOfStrings }
    });
    StructDefinition instantiated_struct_definition = engine.instantiate_generic_struct(generic_struct_definition, "InstantiatedPairWrapper");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 2);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "first");
    EXPECT_EQ(instantiated_struct_definition.fields[1].field_name, "second");
    EXPECT_TRUE(is_int(instantiated_struct_definition.fields[0].field_type));
    EXPECT_TRUE(is_list_of_strings(instantiated_struct_definition.fields[1].field_type));
}

TEST(Core, Generic_Struct_With_PointerType_Field_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "PointerWrapper",
        "test.basalt",   
        { "T" }, 
        { 
            StructDefinition::Field { "poited_object", 
                TypeSignatureFactory::make_ptr_type( TypeSignatureFactory::T ) 
            }
        }
    );
    GenericsInstantiationEngine engine({ GenericSubstitutionRule { "T", TypeSignatureFactory::Int } });
    StructDefinition instantiated_struct_definition = engine.instantiate_generic_struct(generic_struct_definition, "InstantiatedPointerWrapper");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 1);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "poited_object");
    ASSERT_TRUE(instantiated_struct_definition.fields[0].field_type.is<PointerType>());
    PointerType pointed_type = instantiated_struct_definition.fields[0].field_type.get<PointerType>();
    ASSERT_TRUE(pointed_type.pointed_type.is<PrimitiveType>());
    EXPECT_EQ(pointed_type.pointed_type.get<PrimitiveType>().type_name, "Int");
}

TEST(Core, Generic_Struct_With_Slice_Type_Field_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "SliceWrapper", 
        "test.basalt",  
        { "T" }, 
        { 
            StructDefinition::Field { "slice_object", 
                TypeSignatureFactory::make_slice_type( TypeSignatureFactory::T ) 
            }
        }
    );
    GenericsInstantiationEngine engine({ GenericSubstitutionRule { "T", TypeSignatureFactory::Int } });
    StructDefinition instantiated_struct_definition = engine.instantiate_generic_struct(generic_struct_definition, "InstantiatedSliceWrapper");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 1);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "slice_object");
    ASSERT_TRUE(instantiated_struct_definition.fields[0].field_type.is<SliceType>());
    SliceType slice_type = instantiated_struct_definition.fields[0].field_type.get<SliceType>();
    ASSERT_TRUE(slice_type.stored_type.is<PrimitiveType>());
    EXPECT_EQ(slice_type.stored_type.get<PrimitiveType>().type_name, "Int");
}

TEST(Core, Generic_Struct_With_Array_Type_Field_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "ArrayWrapper", 
        "test.basalt",  
        { "T" }, 
        { 
            StructDefinition::Field { "array_object", 
                TypeSignatureFactory::make_array_type( TypeSignatureFactory::T, 10 ) 
            }
        }
    );
    GenericsInstantiationEngine engine({ GenericSubstitutionRule { "T", TypeSignatureFactory::Int } });
    StructDefinition instantiated_struct_definition = engine.instantiate_generic_struct(generic_struct_definition, "InstantiatedArrayWrapper");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 1);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "array_object");
    ASSERT_TRUE(instantiated_struct_definition.fields[0].field_type.is<ArrayType>());
    ArrayType array_type = instantiated_struct_definition.fields[0].field_type.get<ArrayType>();
    ASSERT_TRUE(array_type.stored_type.is<PrimitiveType>());
    EXPECT_EQ(array_type.stored_type.get<PrimitiveType>().type_name, "Int");
    EXPECT_EQ(array_type.array_length, 10);
}