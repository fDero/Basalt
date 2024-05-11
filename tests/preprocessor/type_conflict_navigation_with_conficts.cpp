
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Detected_Type_Conflict_In_Same_File) {
    
    FileRappresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
            StructDefinitionFactory::make_struct_definition("A", { }, { })
        },
        .func_defs = { }
    };
    
    EXPECT_ANY_THROW({
        ProgramRappresentation rappresentation;
        rappresentation.store_definitions_from_file(main_dot_basalt);
        TypeConflictNavigator navigator(rappresentation);
    });
}

TEST(Preprocessor, Detected_Type_Conflict_In_Two_File_Of_Same_Package) {
    
    FileRappresentation a1_dot_basalt = {
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

    FileRappresentation a2_dot_basalt = {
        .file_metadata = {
            .filename = "a2.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition("A", { }, { }),
        },
        .func_defs = { }
    };
    
    EXPECT_ANY_THROW({
        ProgramRappresentation rappresentation;
        rappresentation.store_definitions_from_file(a1_dot_basalt);
        rappresentation.store_definitions_from_file(a2_dot_basalt);
    });
}

TEST(Preprocessor, Detected_Type_Conflict_In_Two_File_Of_Different_Packages) {
    
    FileRappresentation a1_dot_basalt = {
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

    FileRappresentation a2_dot_basalt = {
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
    
    ProgramRappresentation rappresentation;
    rappresentation.store_definitions_from_file(a1_dot_basalt);
    rappresentation.store_definitions_from_file(a2_dot_basalt);
    TypeConflictNavigator navigator(rappresentation);
    EXPECT_ANY_THROW({
        navigator.visit_package("a1package");
    });
}