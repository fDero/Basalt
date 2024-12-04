
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/package_type_conflict_navigator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure project_with_type_conflicts_in_two_files_in_different_packages_that_mutually_imports_each_other ({
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "a1package",
            .imports = { "a2package" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "a.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "a2package",
            .imports = { "a1package" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "b.basalt", { }, { }),
        },
        .func_defs = { }
    }
});

ProjectFileStructure project_with_type_conflicts_in_two_files_in_different_packages_imported_by_a_third_one ({
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "a1package",
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
            .packagename = "a2package",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "b.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "mainpackage",
            .imports = { "a1package", "a2package" }
        },
        .type_defs = { },
        .func_defs = { }
    }
});

ProjectFileStructure project_with_type_conflicts_in_tree_files_in_different_packages_in_an_import_chain ({
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "a1package",
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
            .packagename = "a2package",
            .imports = { "a1package" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", "b.basalt", { }, { }),
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "mainpackage",
            .imports = { "a2package" }
        },
        .type_defs = { },
        .func_defs = { }
    }
});


TEST(Preprocessing, Detected_Type_Conflict_In_Two_File_Of_Different_Packages) {
    ProgramRepresentation program_representation(project_with_type_conflicts_in_two_files_in_different_packages_that_mutually_imports_each_other);
    PackageTypeConflictNavigator navigator(program_representation);
    EXPECT_ANY_THROW({
        navigator.visit_package("a1package");
    });
}

TEST(Preprocessing, Detected_Type_Conflict_In_Three_Files_Of_Different_Packages) {
    ProgramRepresentation program_representation(project_with_type_conflicts_in_two_files_in_different_packages_that_mutually_imports_each_other);
    PackageTypeConflictNavigator navigator(program_representation);
    EXPECT_ANY_THROW({
        navigator.visit_package("mainpackage");
    });
}

TEST(Preprocessing, Detected_Type_Conflict_In_Three_Files_During_Chain_Import) {
    ProgramRepresentation program_representation(project_with_type_conflicts_in_two_files_in_different_packages_that_mutually_imports_each_other);
    PackageTypeConflictNavigator navigator(program_representation);
    EXPECT_ANY_THROW({
        navigator.visit_package("mainpackage");
    });
}