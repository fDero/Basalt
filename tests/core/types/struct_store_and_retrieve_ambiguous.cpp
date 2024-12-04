
#include <gtest/gtest.h>

#include "core/type_definitions_register.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

TEST(Core, Ambiguous_Instantiated_Generic_Struct_Retrieval) {
    
    FileRepresentation wrapper_file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Wrapper", 
                "test.basalt",
                { "T" }, 
                { 
                    StructDefinition::Field { "wrapped", TypeSignatureFactory::T }
                }
            )
        },
        .func_defs = {}
    };

    FileRepresentation employee_file_of_x_package {
        .file_metadata = { 
            .filename = "xemployee.basalt",
            .packagename = "xpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Employee", "xemployee.basalt", { }, { }
            )
        },
        .func_defs = {}
    };

    
    FileRepresentation employee_file_of_y_package {
        .file_metadata = { 
            .filename = "yemployee.basalt",
            .packagename = "ypackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Employee", "yemployee.basalt", { }, { }
            )
        },
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(wrapper_file);
    project.store_file_representation(employee_file_of_x_package);
    project.store_file_representation(employee_file_of_y_package);

    TypeDefinitionsRegister type_register(project);

    CustomType xemployee = CustomType { Token { "Employee", "test.basalt", 1, 1, 1, Token::Type::type }, { } };
    xemployee.package_prefix = "xpackage";

    CustomType yemployee = CustomType { Token { "Employee", "test.basalt", 1, 1, 1, Token::Type::type }, { } };
    yemployee.package_prefix = "ypackage";

    TypeDefinition wrapper_of_xpackage_employee =  type_register.retrieve_type_definition(
        CustomType { Token { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type }, { xemployee } }
    );

    TypeDefinition wrapper_of_ypackage_employee =  type_register.retrieve_type_definition(
        CustomType { Token { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type }, { yemployee } }
    );
    
    ASSERT_TRUE(wrapper_of_xpackage_employee.is<StructDefinition>());
    ASSERT_TRUE(wrapper_of_ypackage_employee.is<StructDefinition>());

    StructDefinition wrapper_of_xpackage_employee_struct_def = wrapper_of_xpackage_employee.get<StructDefinition>();
    StructDefinition wrapper_of_ypackage_employee_struct_def = wrapper_of_ypackage_employee.get<StructDefinition>();

    EXPECT_EQ(wrapper_of_xpackage_employee_struct_def.template_generics_names.size(), 0);
    EXPECT_EQ(wrapper_of_ypackage_employee_struct_def.template_generics_names.size(), 0);

    ASSERT_EQ(wrapper_of_xpackage_employee_struct_def.fields.size(), 1);
    ASSERT_EQ(wrapper_of_ypackage_employee_struct_def.fields.size(), 1);

    EXPECT_EQ(wrapper_of_xpackage_employee_struct_def.fields[0].field_name, "wrapped");
    EXPECT_EQ(wrapper_of_ypackage_employee_struct_def.fields[0].field_name, "wrapped");

    ASSERT_TRUE(wrapper_of_xpackage_employee_struct_def.fields[0].field_type.is<CustomType>());
    ASSERT_TRUE(wrapper_of_ypackage_employee_struct_def.fields[0].field_type.is<CustomType>());

    CustomType xemployee_field = wrapper_of_xpackage_employee_struct_def.fields[0].field_type.get<CustomType>();
    CustomType yemployee_field = wrapper_of_ypackage_employee_struct_def.fields[0].field_type.get<CustomType>();

    EXPECT_EQ(xemployee_field.type_name, "Employee");
    EXPECT_EQ(yemployee_field.type_name, "Employee");

    EXPECT_EQ(xemployee_field.package_prefix, "xpackage");
    EXPECT_EQ(yemployee_field.package_prefix, "ypackage");
}