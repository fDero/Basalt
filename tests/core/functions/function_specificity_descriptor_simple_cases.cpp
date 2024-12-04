
#include <gtest/gtest.h>
#include "core/function_specificity_descriptor.hpp"
#include "frontend/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

FunctionDefinition add_int_int = FunctionDefinitionFactory::make_function_definition(
    "add",
    "test.basalt", 
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "a", TypeSignatureFactory::Int },
        FunctionDefinition::Argument { "b", TypeSignatureFactory::Int }
    },
    TypeSignatureFactory::Int
);

FunctionDefinition add_T_T = FunctionDefinitionFactory::make_function_definition(
    "add", 
    "test.basalt",
    { "T" }, 
    {
        FunctionDefinition::Argument { "a", TypeSignatureFactory::T },
        FunctionDefinition::Argument { "b", TypeSignatureFactory::T }
    },
    TypeSignatureFactory::T
);


FunctionDefinition add_T_U = FunctionDefinitionFactory::make_function_definition(
    "add", 
    "test.basalt",
    { "T", "U" }, 
    {
        FunctionDefinition::Argument { "a", TypeSignatureFactory::T },
        FunctionDefinition::Argument { "b", TypeSignatureFactory::U }
    },
    TypeSignatureFactory::TorU
);

FunctionDefinition add_IntOrFloat_IntOrFloat = FunctionDefinitionFactory::make_function_definition(
    "add", 
    "test.basalt",
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "a", TypeSignatureFactory::IntOrFloat},
        FunctionDefinition::Argument { "b", TypeSignatureFactory::IntOrFloat }
    },
    TypeSignatureFactory::IntOrFloat
);

FunctionDefinition add_int_IntOrFloat = FunctionDefinitionFactory::make_function_definition(
    "add", 
    "test.basalt",
    FunctionDefinitionFactory::no_generics, 
    {
        FunctionDefinition::Argument { "a", TypeSignatureFactory::Int},
        FunctionDefinition::Argument { "b", TypeSignatureFactory::IntOrFloat }
    },
    TypeSignatureFactory::IntOrFloat
);

TEST(Core, Function_Specificiy_Descritor_Non_Generic_Function_Is_More_Specific_Then_A_Generic_One) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    FunctionSpecificityDescriptor non_generic_func_desc(add_int_int, type_register);
    FunctionSpecificityDescriptor generic_func_desc(add_T_T, type_register);
    EXPECT_EQ(
        non_generic_func_desc.compare_with(generic_func_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::more_specific
    );
    EXPECT_EQ(
        generic_func_desc.compare_with(non_generic_func_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::less_specific
    );
}

TEST(Core, Function_Specificiy_Descritor_Same_Number_Of_Generics_Usages_Are_Equally_Specific_Even_With_Different_Number_Of_Generics) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    FunctionSpecificityDescriptor add_T_U_desc(add_T_U, type_register);
    FunctionSpecificityDescriptor add_T_T_desc(add_T_T, type_register);
    EXPECT_EQ(
        add_T_T_desc.compare_with(add_T_U_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::equally_specific
    );
    EXPECT_EQ(
        add_T_U_desc.compare_with(add_T_T_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::equally_specific
    );
}

TEST(Core, Function_Specificiy_Descritor_Only_One_Union_More_Specific_Then_Two) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    FunctionSpecificityDescriptor add_IntOrFloat_IntOrFloat_desc(add_IntOrFloat_IntOrFloat, type_register);
    FunctionSpecificityDescriptor add_int_IntOrFloat_desc(add_int_IntOrFloat, type_register);
    EXPECT_EQ(
        add_IntOrFloat_IntOrFloat_desc.compare_with(add_int_IntOrFloat_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::less_specific
    );
    EXPECT_EQ(
        add_int_IntOrFloat_desc.compare_with(add_IntOrFloat_IntOrFloat_desc), 
        FunctionSpecificityDescriptor::ComparisonResult::more_specific
    );
}