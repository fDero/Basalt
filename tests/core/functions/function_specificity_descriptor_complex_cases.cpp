
#include <gtest/gtest.h>
#include "core/function_specificity_descriptor.hpp"
#include "frontend/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

FunctionDefinition print_IntOrFloat = FunctionDefinitionFactory::make_void_function_definition(
    "print",
    "test.basalt", 
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "x", TypeSignatureFactory::IntOrFloat }
    }
);

FunctionDefinition print_PrimitiveTypesUnion = FunctionDefinitionFactory::make_void_function_definition(
    "print", 
    "test.basalt",
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "x", TypeSignatureFactory::PrimitiveTypesUnion }
    }
);

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

TypeSignature WrapperOfTs = CustomType { Token { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
TypeSignature WrapperOfWrapperOfTs = CustomType { Token { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type }, { WrapperOfTs } };

FunctionDefinition f_wrapper_of_wrapper_of_Ts = FunctionDefinitionFactory::make_void_function_definition(
    "f", 
    "test.basalt",
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "list", WrapperOfWrapperOfTs },
        FunctionDefinition::Argument { "item", TypeSignatureFactory::IntOrFloat }
    }
);

FunctionDefinition f_wrapper_of_Ts = FunctionDefinitionFactory::make_void_function_definition(
    "f", 
    "test.basalt",
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "list", WrapperOfTs },
        FunctionDefinition::Argument { "item", TypeSignatureFactory::Int }
    }
);

TEST(Core, Function_Specificiy_Descritor_Less_Covered_Cases_Is_More_Spcecific) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    FunctionSpecificityDescriptor print_PrimitiveTypesUnion_desc(print_PrimitiveTypesUnion, type_register);
    FunctionSpecificityDescriptor print_IntOrFloat_desc(print_IntOrFloat, type_register);
    EXPECT_EQ(
        print_IntOrFloat_desc.compare_with(print_PrimitiveTypesUnion_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::more_specific
    );
    EXPECT_EQ(
        print_PrimitiveTypesUnion_desc.compare_with(print_IntOrFloat_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::less_specific
    );
}

TEST(Core, Function_Specificiy_Descritor_More_Complex_Is_More_Specific) {
    ProjectFileStructure simple_project({wrapper_file});
    TypeDefinitionsRegister type_register(simple_project);
    FunctionSpecificityDescriptor f_wrapper_of_wrapper_of_Ts_desc(f_wrapper_of_wrapper_of_Ts, type_register);
    FunctionSpecificityDescriptor f_wrapper_of_Ts_desc(f_wrapper_of_Ts, type_register);
    EXPECT_EQ(
        f_wrapper_of_wrapper_of_Ts_desc.compare_with(f_wrapper_of_Ts_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::more_specific
    );
    EXPECT_EQ(
        f_wrapper_of_Ts_desc.compare_with(f_wrapper_of_wrapper_of_Ts_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::less_specific
    );
}