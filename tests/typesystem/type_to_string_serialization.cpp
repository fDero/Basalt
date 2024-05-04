
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(TypeSystem, Base_Type_No_Generics_To_String) {
    TypeSignature T = TypeSignatureFactory::T;
    ASSERT_EQ(T.to_string(), "T");
}

TEST(TypeSystem, Base_Type_Two_Generics_To_String) {
    TypeSignature Pair = TypeSignatureFactory::make_base_type(
        "Pair", { 
            TypeSignatureFactory::T, 
            TypeSignatureFactory::U 
        }
    );
    ASSERT_EQ(Pair.to_string(), "Pair<T,U>");
}

TEST(TypeSystem, PointerType_To_String) {
    TypeSignature PtrToX = TypeSignatureFactory::make_ptr_type(
        TypeSignatureFactory::make_base_type("X", {})
    );
    ASSERT_EQ(PtrToX.to_string(), "#X");
}

TEST(TypeSystem, PointerType_To_PointerType_To_String) {
    TypeSignature PtrToPtrToX = TypeSignatureFactory::make_ptr_type(
        TypeSignatureFactory::make_ptr_type(
            TypeSignatureFactory::make_base_type("X", {})
        )
    );
    ASSERT_EQ(PtrToPtrToX.to_string(), "##X");
}

TEST(TypeSystem, Array_Of_Floats_To_String) {
    TypeSignature Array = TypeSignatureFactory::ArrayOfFloats;
    ASSERT_EQ(Array.to_string(), "[10]Float");
}

TEST(TypeSystem, Slice_Of_Floats_To_String) {
    TypeSignature Slice = TypeSignatureFactory::SliceOfFloats;
    ASSERT_EQ(Slice.to_string(), "$Float");
}

TEST(TypeSystem, Slice_Of_Pointer_To_Array_Of_Floats_To_String) {
    TypeSignature Slice = TypeSignatureFactory::make_slice_type(
        TypeSignatureFactory::make_ptr_type(
            TypeSignatureFactory::make_array_type(
                TypeSignatureFactory::Float, 10
            )
        )
    );
    ASSERT_EQ(Slice.to_string(), "$#[10]Float");
}