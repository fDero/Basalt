
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"

TEST(TypeSystem, Base_Type_No_Generics_To_String) {
    TypeSignature T = CustomType { "T", {} };
    ASSERT_EQ(T.to_string(), "T");
}

TEST(TypeSystem, Base_Type_Two_Generics_To_String) {
    TypeSignature Pair = CustomType { "Pair", { CustomType{ "T", {} }, CustomType{ "U", {} } } };
    ASSERT_EQ(Pair.to_string(), "Pair<T,U>");
}

TEST(TypeSystem, PointerType_To_String) {
    TypeSignature PtrToX = PointerType { CustomType{ "X", {} } };
    ASSERT_EQ(PtrToX.to_string(), "#X");
}

TEST(TypeSystem, PointerType_To_PointerType_To_String) {
    TypeSignature PtrToX = PointerType { CustomType{ "X", {} } };
    TypeSignature PtrToPtrToX = PointerType { PtrToX };
    ASSERT_EQ(PtrToPtrToX.to_string(), "##X");
}

TEST(TypeSystem, Array_Of_Floats_To_String) {
    TypeSignature Array = ArrayType { 10, CustomType { "Float", {} } };
    ASSERT_EQ(Array.to_string(), "Array<10,Float>");
}

TEST(TypeSystem, Slice_Of_Floats_To_String) {
    TypeSignature Slice = SliceType { CustomType { "Float", {} } };
    ASSERT_EQ(Slice.to_string(), "Slice<Float>");
}

TEST(TypeSystem, Slice_Of_Pointer_To_Array_Of_Floats_To_String) {
    TypeSignature Slice = SliceType { PointerType { ArrayType { 10, CustomType { "Float", {} }}}};
    ASSERT_EQ(Slice.to_string(), "Slice<#Array<10,Float>>");
}