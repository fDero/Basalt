
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(TypeSystem, Generic_Struct_With_CustomType_Fields_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "Pair", 
        { "T", "U" }, 
        { 
            StructDefinition::Field { "first", TypeSignatureFactory::T },
            StructDefinition::Field { "second", TypeSignatureFactory::U }
        }
    );
    StructDefinition instantiated_struct_definition = generic_struct_definition;
    instantiated_struct_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("Pair", { 
            TypeSignatureFactory::Int, 
            TypeSignatureFactory::ListOfStrings 
        })
            .get<CustomType>()
    );
    EXPECT_EQ(instantiated_struct_definition.struct_name, "Pair<Int,List<String>>");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 2);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "first");
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_type.to_string(), "Int");
    EXPECT_EQ(instantiated_struct_definition.fields[1].field_name, "second");
    EXPECT_EQ(instantiated_struct_definition.fields[1].field_type.to_string(), "List<String>");
}

TEST(TypeSystem, Generic_Struct_With_PointerType_Field_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "PointerWrapper", 
        { "T" }, 
        { 
            StructDefinition::Field { "poited_object", 
                TypeSignatureFactory::make_ptr_type( TypeSignatureFactory::T ) 
            }
        }
    );
    StructDefinition instantiated_struct_definition = generic_struct_definition;
    instantiated_struct_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("PointerWrapper", { TypeSignatureFactory::Int })
            .get<CustomType>()
    );
    EXPECT_EQ(instantiated_struct_definition.struct_name, "PointerWrapper<Int>");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 1);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "poited_object");
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_type.to_string(), "#Int");
}

TEST(TypeSystem, Generic_Struct_With_Slice_Type_Field_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "SliceWrapper", 
        { "T" }, 
        { 
            StructDefinition::Field { "slice_object", 
                TypeSignatureFactory::make_slice_type( TypeSignatureFactory::T ) 
            }
        }
    );
    StructDefinition instantiated_struct_definition = generic_struct_definition;
    instantiated_struct_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("SliceWrapper", { TypeSignatureFactory::Int })
            .get<CustomType>()
    );
    EXPECT_EQ(instantiated_struct_definition.struct_name, "SliceWrapper<Int>");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 1);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "slice_object");
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_type.to_string(), "$Int");
}

TEST(TypeSystem, Generic_Struct_With_Array_Type_Field_Instantiation) {
    StructDefinition generic_struct_definition = StructDefinitionFactory::make_struct_definition(
        "ArrayWrapper", 
        { "T" }, 
        { 
            StructDefinition::Field { "array_object", 
                TypeSignatureFactory::make_array_type( TypeSignatureFactory::T, 10 ) 
            }
        }
    );
    StructDefinition instantiated_struct_definition = generic_struct_definition;
    instantiated_struct_definition.instantiate_generics(
        TypeSignatureFactory::make_custom_type("ArrayWrapper", { TypeSignatureFactory::Int })
            .get<CustomType>()
    );
    EXPECT_EQ(instantiated_struct_definition.struct_name, "ArrayWrapper<Int>");
    ASSERT_EQ(instantiated_struct_definition.fields.size(), 1);
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_name, "array_object");
    EXPECT_EQ(instantiated_struct_definition.fields[0].field_type.to_string(), "[10]Int");
}