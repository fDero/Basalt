
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_queries.hpp"

inline GenericSubstitutionRule ts_are_ints = { "T", TypeSignatureFactory::Int };
inline GenericSubstitutionRule us_are_list_of_strings = { "U", TypeSignatureFactory::ListOfStrings };
inline GenericSubstitutionRule vs_are_floats = { "V", TypeSignatureFactory::Float };
inline GenericSubstitutionRuleSet rules = { ts_are_ints, us_are_list_of_strings, vs_are_floats };

TEST(TypeSystem, Non_Generic_Typesignature_Instantiations) {
    TypeSignature integer = TypeSignatureFactory::Int;
    TypeSignature list_of_strings = TypeSignatureFactory::ListOfStrings;
    TypeSignature ptr_to_float = TypeSignatureFactory::PointerToFloat;
    TypeSignature ptr_to_slice_of_floats = TypeSignatureFactory::make_ptr_type(TypeSignatureFactory::SliceOfFloats);
    TypeSignature array_of_strings = TypeSignatureFactory::ArrayOfStrings;

    integer.instantiate_generics(rules);
    list_of_strings.instantiate_generics(rules);
    ptr_to_float.instantiate_generics(rules);
    ptr_to_slice_of_floats.instantiate_generics(rules);
    array_of_strings.instantiate_generics(rules);
    
    EXPECT_TRUE(is_int(integer));
    EXPECT_TRUE(is_list_of_strings(list_of_strings));
    EXPECT_TRUE(is_pointer_to_float(ptr_to_float));
    EXPECT_TRUE(is_pointer_to_slice_of_floats(ptr_to_slice_of_floats));
    EXPECT_TRUE(is_array_of_strings_of_size_n(array_of_strings, 10));
}

TEST(TypeSystem, CustomType_With_Two_Generics_instantiation) {
    TypeSignature Pair = TypeSignatureFactory::make_custom_type("Pair", { TypeSignatureFactory::T, TypeSignatureFactory::U });
    Pair.instantiate_generics(rules);
    EXPECT_TRUE(Pair.is<CustomType>());
    CustomType pair = Pair.get<CustomType>();
    EXPECT_EQ(pair.type_name, "Pair");
    ASSERT_EQ(pair.instantiation_generics.size(), 2);
    EXPECT_TRUE(is_int(pair.instantiation_generics[0]));
    EXPECT_TRUE(is_list_of_strings(pair.instantiation_generics[1]));
}

TEST(TypeSystem, CustomType_With_One_Conrete_Generic_And_One_Template_Generic_instantiation) {
    TypeSignature Pair = TypeSignatureFactory::make_custom_type("Pair", { TypeSignatureFactory::T, TypeSignatureFactory::Char });
    Pair.instantiate_generics(rules);
    EXPECT_TRUE(Pair.is<CustomType>());
    CustomType pair = Pair.get<CustomType>();
    EXPECT_EQ(pair.type_name, "Pair");
    ASSERT_EQ(pair.instantiation_generics.size(), 2);
    EXPECT_TRUE(is_int(pair.instantiation_generics[0]));
    EXPECT_TRUE(is_char(pair.instantiation_generics[1]));
}

TEST(TypeSystem, Pointer_To_Pointer_To_Array_Of_Generic_Type_Instanc_Test) {
    TypeSignature ptr_to_ptr_to_array_of_t = TypeSignatureFactory::make_ptr_type(
        TypeSignatureFactory::make_ptr_type(
            TypeSignatureFactory::make_array_type(TypeSignatureFactory::T, 10)
        )
    );
    ptr_to_ptr_to_array_of_t.instantiate_generics(rules);
    EXPECT_TRUE(is_pointer_to_pointer_to_array_of_ints_of_size_n(ptr_to_ptr_to_array_of_t, 10));
}

TEST(TypeSystem, Pointer_To_Pointer_To_Slice_Of_Generic_Type_Instanc_Test) {
        TypeSignature ptr_to_ptr_to_array_of_t = TypeSignatureFactory::make_ptr_type(
        TypeSignatureFactory::make_ptr_type(
            TypeSignatureFactory::make_slice_type(TypeSignatureFactory::T)
        )
    );
    ptr_to_ptr_to_array_of_t.instantiate_generics(rules);
    EXPECT_TRUE(is_pointer_to_pointer_to_slice_of_ints(ptr_to_ptr_to_array_of_t));
}