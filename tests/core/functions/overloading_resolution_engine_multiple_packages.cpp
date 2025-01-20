
#include <gtest/gtest.h>
#include "core/overloading_resolution_engine.hpp"
#include "core/type_definitions_register.hpp"
#include "frontend/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

ProjectFileStructure simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "simple_project_with_multiple_func_defs",
            .imports = {"genericimpl", "basicimpl"}
        },
        .type_defs = {},
        .func_defs = {
        }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "genericimpl.basalt",
            .packagename = "genericimpl",
            .imports = {}
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add",
                "genericimpl.basalt",
                { "T" },
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::T },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::T }
                },
                TypeSignatureFactory::T
            )
        }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "basicimpl.basalt",
            .packagename = "basicimpl",
            .imports = {}
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add",
                "basicimpl.basalt",
                FunctionDefinitionFactory::no_generics,
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Int },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Int }
                },
                TypeSignatureFactory::Int
            )
        }
    }

});

TEST(Core, Simple_Overloading_Resolution_Of_Generic_Vs_Non_Generic_Function_In_Multiple_Packages_V1) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "add", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        {  }
    };
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    ASSERT_NE(func_def_ref, nullptr);
    EXPECT_EQ(func_def_ref->function_name, "add");
    EXPECT_TRUE(func_def_ref->template_generics_names.empty());
}

TEST(Core, Simple_Overloading_Resolution_Of_Generic_Vs_Non_Generic_Function_In_Multiple_Packages_V2) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "add", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        { 
            TypeSignatureFactory::Int
        }
    };
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    ASSERT_NE(func_def_ref, nullptr);
    EXPECT_EQ(func_def_ref->function_name, "add");
}

TEST(Core, Simple_Overloading_Of_Non_Existing_Function_In_Multiple_Packages) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "min", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        { 
            TypeSignatureFactory::Int
        }
    };
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    EXPECT_EQ(func_def_ref, nullptr);
}

TEST(Core, Simple_Overloading_Resolution_Of_Generic_Vs_Non_Generic_Function_In_Multiple_Packages_V3) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "add", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        {  }
    };
    func_call.package_prefix = "genericimpl";
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    EXPECT_NE(func_def_ref, nullptr); // <-- type inference automatically infers the generic type
    EXPECT_EQ(func_def_ref->function_name, "add<Int>");
}

TEST(Core, Simple_Overloading_Resolution_Of_Generic_Vs_Non_Generic_Function_In_Multiple_Packages_V4) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "add", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        { 
            TypeSignatureFactory::Int
        }
    };
    func_call.package_prefix = "genericimpl";
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    ASSERT_NE(func_def_ref, nullptr);
    EXPECT_EQ(func_def_ref->function_name, "add");
}

TEST(Core, Simple_Overloading_Of_Non_Existing_Function_In_Multiple_Packages_V2) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "min", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        { 
            TypeSignatureFactory::Int
        }
    };
    func_call.package_prefix = "genericimpl";
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    EXPECT_EQ(func_def_ref, nullptr);
}

TEST(Core, Simple_Overloading_Resolution_Of_Generic_Vs_Non_Generic_Function_In_Multiple_Packages_V5) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "add", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        {  }
    };
    func_call.package_prefix = "basicimpl";
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    ASSERT_NE(func_def_ref, nullptr);
    EXPECT_EQ(func_def_ref->function_name, "add");
    EXPECT_TRUE(func_def_ref->template_generics_names.empty());
}

TEST(Core, Simple_Overloading_Resolution_Of_Generic_Vs_Non_Generic_Function_In_Multiple_Packages_V6) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "add", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        { 
            TypeSignatureFactory::Int
        }
    };
    func_call.package_prefix = "basicimpl";
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    EXPECT_EQ(func_def_ref, nullptr);
}

TEST(Core, Simple_Overloading_Of_Non_Existing_Function_In_Multiple_Packages_V3) {
    ProjectFileStructure& proj = simple_project_with_both_generic_and_non_generic_func_overloads_in_multiple_packages;
    TypeDefinitionsRegister type_register(proj);
    FunctionOverloadsRegister func_register(proj);
    OverloadingResolutionEngine overoad_resolver(func_register, type_register, proj);
    FunctionCall func_call {
        Token { "min", "main.basalt", 1, 2, 5, Token::Type::text },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } },
            IntLiteral { Token { "1", "main.basalt", 1, 2, 5, Token::Type::integer_literal } }
        },
        { 
            TypeSignatureFactory::Int
        }
    };
    func_call.package_prefix = "basicimpl";
    std::vector<TypeSignature> arg_types = { TypeSignatureFactory::Int, TypeSignatureFactory::Int };
    FunctionDefinition::Ref func_def_ref = overoad_resolver.retrieve_function_definition(func_call, arg_types);
    EXPECT_EQ(func_def_ref, nullptr);
}