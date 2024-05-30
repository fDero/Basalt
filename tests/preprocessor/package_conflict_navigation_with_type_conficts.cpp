
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Detected_Type_Conflict_In_Two_File_Of_Different_Packages) {
    
    Filerepresentation a1_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "a1package",
            .imports = { "a2package" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    Filerepresentation a2_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "a2package",
            .imports = { "a1package" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(a1_dot_basalt);
    representation.store_definitions_from_file(a2_dot_basalt);
    PackageTypeConflictNavigator navigator(representation);
    EXPECT_ANY_THROW({
        navigator.visit_package("a1package");
    });
}

TEST(Preprocessor, Detected_Type_Conflict_In_Three_Files_Of_Different_Packages) {
    
    Filerepresentation a1_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "a1package",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    Filerepresentation a2_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "a2package",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    Filerepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "mainpackage",
            .imports = { "a1package", "a2package" }
        },
        .type_defs = { },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(a1_dot_basalt);
    representation.store_definitions_from_file(a2_dot_basalt);
    representation.store_definitions_from_file(main_dot_basalt);
    PackageTypeConflictNavigator navigator(representation);
    EXPECT_ANY_THROW({
        navigator.visit_package("mainpackage");
    });
}

TEST(Preprocessor, Detected_Type_Conflict_In_Three_Files_During_Chain_Import) {
    
    Filerepresentation a1_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "a1package",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    Filerepresentation a2_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "a2package",
            .imports = { "a1package" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };

    Filerepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "mainpackage",
            .imports = { "a2package" }
        },
        .type_defs = { },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(a1_dot_basalt);
    representation.store_definitions_from_file(a2_dot_basalt);
    representation.store_definitions_from_file(main_dot_basalt);
    PackageTypeConflictNavigator navigator(representation);
    EXPECT_ANY_THROW({
        navigator.visit_package("mainpackage");
    });
}