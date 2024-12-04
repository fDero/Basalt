
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/package_type_conflict_navigator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure simple_multifile_project_with_harmless_typedefs({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "main.basalt", { }, { }),
            StructDefinitionFactory::make_struct_definition("B", "main.basalt", { }, { })
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "testpackage2",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "a.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "testpackage2",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("B", "b.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "x.basalt",
            .packagename = "xpackage",
            .imports = { "ypackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "x.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "y.basalt",
            .packagename = "ypackage",
            .imports = { "xpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("B", "y.basalt", { }, { }),
        },
        .func_defs = { }
    }
});

TEST(Preprocessing, No_Type_Conflict_In_Same_File) {
    ProgramRepresentation program_representation(simple_multifile_project_with_harmless_typedefs);
    PackageTypeConflictNavigator navigator(program_representation);
    navigator.visit_package("testpackage");
}

TEST(Preprocessing, No_Type_Conflict_In_Two_File_Of_Same_Package) {
    ProgramRepresentation program_representation(simple_multifile_project_with_harmless_typedefs);
    PackageTypeConflictNavigator navigator(program_representation);
    navigator.visit_package("testpackage2");
}

TEST(Preprocessing, No_Type_Conflict_In_Two_File_Of_Different_Packages) {
    ProgramRepresentation program_representation(simple_multifile_project_with_harmless_typedefs);
    PackageTypeConflictNavigator navigator(program_representation);
    navigator.visit_package("xpackage");
}

TEST(Preprocessing, No_Type_Conflict_In_All_Packages) {
    ProgramRepresentation program_representation(simple_multifile_project_with_harmless_typedefs);
    PackageTypeConflictNavigator navigator(program_representation);
    navigator.visit_all_packages();
}