
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"

GenericSubstitutionRule ts_are_ints = { BaseType { "T" }, BaseType{"Int"} };
GenericSubstitutionRule us_are_list_of_strings = { BaseType { "U" }, BaseType { "List", { BaseType { "String" } } }};
GenericSubstitutionRule vs_are_floats = { BaseType { "V" }, BaseType { "Float" } };
GenericSubstitutionRuleSet rules = { ts_are_ints, us_are_list_of_strings, vs_are_floats };



TEST(TypeSystem, Non_Generic_Typesignature_Instantiations) {
    TypeSignature integer = BaseType { "Int" };
    TypeSignature list_of_strings = BaseType { "List", { BaseType { "String" } } };
    TypeSignature ptr_to_float = PointerType { BaseType { "Float" } };
    TypeSignature ptr_to_slice_of_floats = PointerType { SliceType { BaseType { "Float" } } };
    TypeSignature array_of_strings = ArrayType { 10,  BaseType { "String" } };

    integer.instantiate_generics(rules);
    list_of_strings.instantiate_generics(rules);
    ptr_to_float.instantiate_generics(rules);
    ptr_to_slice_of_floats.instantiate_generics(rules);
    array_of_strings.instantiate_generics(rules);
    
    EXPECT_EQ(integer.to_string(), "Int");
    EXPECT_EQ(list_of_strings.to_string(), "List<String>");
    EXPECT_EQ(ptr_to_float.to_string(), "#Float");
    EXPECT_EQ(ptr_to_slice_of_floats.to_string(), "#$Float");
    EXPECT_EQ(array_of_strings.to_string(), "[10]String");
}

TEST(TypeSystem, Base_Type_With_Two_Generics_instantiationd) {
    TypeSignature Pair = BaseType { "Pair", { BaseType{ "T", {} }, BaseType{ "U", {} } } };
    Pair.instantiate_generics(rules);
    EXPECT_EQ(Pair.to_string(), "Pair<Int,List<String>>");
}

TEST(TypeSystem, Base_Type_With_One_Conrete_Generic_And_One_Template_Generic_instantiationd) {
    TypeSignature Pair = BaseType { "Pair", { BaseType{ "T", {} }, BaseType{ "Char", {} } } };
    Pair.instantiate_generics(rules);
    EXPECT_EQ(Pair.to_string(), "Pair<Int,Char>");
}

TEST(TypeSystem, Pointer_To_Pointer_To_Array_Of_Generic_Type_Instanc_Test) {
    TypeSignature array_of_t = ArrayType { 10, BaseType { "T" } };
    TypeSignature ptr_to_array_of_t = PointerType { array_of_t };
    TypeSignature ptr_to_ptr_to_array_of_t = PointerType { ptr_to_array_of_t };
    ptr_to_ptr_to_array_of_t.instantiate_generics(rules);
    EXPECT_EQ(ptr_to_ptr_to_array_of_t.to_string(), "##[10]Int");
}

TEST(TypeSystem, Pointer_To_Pointer_To_Slice_Of_Generic_Type_Instanc_Test) {
    TypeSignature array_of_t = SliceType { BaseType { "T" } };
    TypeSignature ptr_to_array_of_t = PointerType { array_of_t };
    TypeSignature ptr_to_ptr_to_array_of_t = PointerType { ptr_to_array_of_t };
    ptr_to_ptr_to_array_of_t.instantiate_generics(rules);
    EXPECT_EQ(ptr_to_ptr_to_array_of_t.to_string(), "##$Int");
}