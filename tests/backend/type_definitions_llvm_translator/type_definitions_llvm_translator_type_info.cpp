
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "core/program_representation.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

#include <llvm/Support/raw_ostream.h>

static ProjectFileStructure empty_project({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { },
        .func_defs = { }
    }
});

TEST(Backend, Two_Ints_Same_TypeInfo) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("TwoIntsSameTypeInfo", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::GlobalVariable* llvm_int_type_info_1 = type_definitions_llvm_translator
        .fetch_type_info(TypeSignatureFactory::Int);
    llvm::GlobalVariable* llvm_int_type_info_2 = type_definitions_llvm_translator
        .fetch_type_info(TypeSignatureFactory::Int);
    EXPECT_EQ(llvm_int_type_info_1, llvm_int_type_info_2);
}

TEST(Backend, Correct_IntOrFloat_Compatible_Type_Infos) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("TwoNumbersSameTypeInfo", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    auto compatible_typeinfos = type_definitions_llvm_translator
        .fetch_all_type_infos_for_non_union_compatible_types(TypeSignatureFactory::IntOrFloat);
    EXPECT_EQ(compatible_typeinfos.size(), 2);
    EXPECT_NE(compatible_typeinfos[0], compatible_typeinfos[1]);
    EXPECT_EQ(compatible_typeinfos[0], type_definitions_llvm_translator.fetch_type_info(TypeSignatureFactory::Int));
    EXPECT_EQ(compatible_typeinfos[1], type_definitions_llvm_translator.fetch_type_info(TypeSignatureFactory::Float));
}