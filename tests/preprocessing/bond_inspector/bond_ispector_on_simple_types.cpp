
#include <gtest/gtest.h>
#include "preprocessing/bond_inspector.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessing, Bond_Inspector_On_Non_String_Primitive_Types) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    BondInspector bond_inspector(empty_scope_context, empty_program);
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::Int));
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::Float));
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::Char));
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::Bool));
}

TEST(Preprocessing, Bond_Inspector_On_Strings) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    BondInspector bond_inspector(empty_scope_context, empty_program);
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::String));
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::RawString));
}

TEST(Preprocessing, Bond_Inspector_On_Pointers_And_Slices) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    BondInspector bond_inspector(empty_scope_context, empty_program);
    EXPECT_TRUE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::PointerToInt));
    EXPECT_TRUE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::SliceOfInts));
}

TEST(Preprocessing, Bond_Inspector_On_Arrays) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    BondInspector bond_inspector(empty_scope_context, empty_program);
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bound(TypeSignatureFactory::ArrayOfInts));
}