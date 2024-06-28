
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, No_Type_Conflict_In_Same_File) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
            StructDefinitionFactory::make_struct_definition("B", { }, { })
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(main_dot_basalt);
    PackageTypeConflictNavigator navigator(representation);
    navigator.visit_package("testpackage");
}

TEST(Preprocessor, No_Type_Conflict_In_Two_File_Of_Same_Package) {
    
    FileRepresentation a_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    FileRepresentation b_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("B", { }, { }),
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(a_dot_basalt);
    representation.store_definitions_from_file(b_dot_basalt);
    PackageTypeConflictNavigator navigator(representation);
    navigator.visit_package("testpackage");
}

TEST(Preprocessor, No_Type_Conflict_In_Two_File_Of_Different_Packages) {
    
    FileRepresentation a_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    FileRepresentation b_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { "apackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("B", { }, { }),
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(a_dot_basalt);
    representation.store_definitions_from_file(b_dot_basalt);
    PackageTypeConflictNavigator navigator(representation);
    navigator.visit_package("apackage");
}