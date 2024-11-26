
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
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "a.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { "apackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("B", "b.basalt", { }, { }),
        },
        .func_defs = { }
    }
});

TEST(Preprocessor, No_Type_Conflict_In_Same_File) {
    PackageTypeConflictNavigator navigator(simple_multifile_project_with_harmless_typedefs);
    navigator.visit_package("testpackage");
}

TEST(Preprocessor, No_Type_Conflict_In_Two_File_Of_Same_Package) {
    PackageTypeConflictNavigator navigator(simple_multifile_project_with_harmless_typedefs);
    navigator.visit_package("testpackage2");
}

TEST(Preprocessor, No_Type_Conflict_In_Two_File_Of_Different_Packages) {
    PackageTypeConflictNavigator navigator(simple_multifile_project_with_harmless_typedefs);
    navigator.visit_package("apackage");
}