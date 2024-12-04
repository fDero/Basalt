
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/expression_type_deducer.hpp"

#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

static ProjectFileStructure simple_project ({
    {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "main",
            .imports = {}
        },
        .type_defs = {
        },
        .func_defs = {
            FunctionDefinitionFactory::make_void_function_definition(
                "main", "main.basalt", FunctionDefinitionFactory::no_generics, FunctionDefinitionFactory::no_args
            ),
            FunctionDefinitionFactory::make_function_definition(
                "add", "main.basalt", FunctionDefinitionFactory::no_generics, {
                    FunctionDefinition::Argument {
                        "a", TypeSignatureFactory::Int
                    },
                    FunctionDefinition::Argument {
                        "b", TypeSignatureFactory::Int
                    },
                },
                TypeSignatureFactory::Int
            ),
            FunctionDefinitionFactory::make_function_definition(
                "add", "main.basalt", FunctionDefinitionFactory::no_generics, {
                    FunctionDefinition::Argument {
                        "a", TypeSignatureFactory::Float
                    },
                    FunctionDefinition::Argument {
                        "b", TypeSignatureFactory::Float
                    },
                },
                TypeSignatureFactory::Float
            )
        }
    }
});

static TypeDefinitionsRegister simple_type_register(simple_project);
static FunctionOverloadsRegister simple_overloads_register(simple_project);
static OverloadingResolutionEngine simple_overloading_resolution_engine(simple_overloads_register, simple_type_register, simple_project);
static CommonFeatureAdoptionPlanGenerationEngine simple_common_feature_adoption_plan_generation_engine(simple_overloading_resolution_engine, simple_type_register);
static ScopeContext simple_scope_context;
static ExpressionTypeDeducer type_deducer(
    simple_type_register,
    simple_overloading_resolution_engine,
    simple_common_feature_adoption_plan_generation_engine,
    simple_project,
    simple_scope_context    
);

TEST(Core, Type_Deduction_Fails_Because_Args_Are_Generic) {
    simple_scope_context.store_local_variable(VariableDeclaration {
        "t", TypeSignatureFactory::T, std::nullopt, 
        Token { "var", "main.basalt", 1, 1, 1, Token::Type::var_keyword }
    });
    Expression function_call = FunctionCall {
        Token { "add", "main.basalt", 1, 1, 1, Token::Type::text },
        {
            Identifier { Token { "t", "main.basalt", 1, 1, 1, Token::Type::text } },
            Identifier { Token { "t", "main.basalt", 1, 1, 1, Token::Type::text } }
        },
        {}
    };
    EXPECT_ANY_THROW({
        std::optional<TypeSignature> function_call_type_opt = type_deducer.deduce_expression_type(function_call);
    });
}

TEST(Core, Type_Deduction_Can_Still_Spot_Errors_With_Function_Calls_Even_When_Type_Is_Already_Known_To_Be_Undecidible) {
    simple_scope_context.store_local_variable(VariableDeclaration {
        "u", TypeSignatureFactory::U, std::nullopt, 
        Token { "var", "main.basalt", 1, 1, 1, Token::Type::var_keyword }
    });
    Expression function_call = FunctionCall {
        Token { "add", "main.basalt", 1, 1, 1, Token::Type::text },
        {
            Identifier { Token { "u", "main.basalt", 1, 1, 1, Token::Type::text } },
            Identifier { Token { "undefined_symbol_that_will_cause_an_error", "main.basalt", 1, 1, 1, Token::Type::text } }
        },
        {}
    };
    EXPECT_ANY_THROW({
        std::optional<TypeSignature> function_call_type_opt = 
            type_deducer.deduce_expression_type(function_call);
    });
}

TEST(Core, Type_Deduction_Cannot_Figure_What_Operator_To_Use) {
    simple_scope_context.store_local_variable(VariableDeclaration {
        "s", TypeSignatureFactory::S, std::nullopt, 
        Token { "var", "main.basalt", 1, 1, 1, Token::Type::var_keyword }
    });
    Expression binary_operator = BinaryOperator {
        Token { "+", "main.basalt", 1, 1, 1, Token::Type::symbol },
        Identifier { Token { "s", "main.basalt", 1, 1, 1, Token::Type::text } },
        Identifier { Token { "s", "main.basalt", 1, 1, 1, Token::Type::text } }
    };
    std::optional<TypeSignature> binary_operator_type_opt = type_deducer.deduce_expression_type(binary_operator);
    ASSERT_TRUE(binary_operator_type_opt.has_value());
    const TypeSignature& binary_operator_type = binary_operator_type_opt.value();
    ASSERT_TRUE(binary_operator_type.is<TemplateType>());
    const TemplateType& binary_operator_template_type = binary_operator_type.get<TemplateType>();
    EXPECT_EQ(binary_operator_template_type.type_name, "S");
}

TEST(Core, Type_Deduction_Works_For_Address_Of_Generic_Typed_Variable) {
    simple_scope_context.store_local_variable(VariableDeclaration {
        "v", TypeSignatureFactory::V, std::nullopt, 
        Token { "var", "main.basalt", 1, 1, 1, Token::Type::var_keyword }
    });
    Expression address_of = UnaryOperator {
        Token { "&", "main.basalt", 1, 1, 1, Token::Type::symbol },
        Identifier { Token { "v", "main.basalt", 1, 1, 1, Token::Type::text } }
    };
    std::optional<TypeSignature> address_of_type_opt = type_deducer.deduce_expression_type(address_of);
    ASSERT_TRUE(address_of_type_opt.has_value());
    const TypeSignature& address_of_type = address_of_type_opt.value();
    ASSERT_TRUE(address_of_type.is<PointerType>());
    const PointerType& address_of_pointer_type = address_of_type.get<PointerType>();
    EXPECT_TRUE(address_of_pointer_type.pointed_type.is<TemplateType>());
    const TemplateType& address_of_pointer_template_type = address_of_pointer_type.pointed_type.get<TemplateType>();
    EXPECT_EQ(address_of_pointer_template_type.type_name, "V");
}

TEST(Core, Type_Deduction_Works_For_Pointer_Dereference_Of_Generic_Typed_Pointer) {
    simple_scope_context.store_local_variable(VariableDeclaration {
        "k", TypeSignatureFactory::PtrToK, std::nullopt, 
        Token { "var", "main.basalt", 1, 1, 1, Token::Type::var_keyword }
    });

    ASSERT_TRUE(simple_scope_context.get_local_object_type("k").is<PointerType>());

    Expression ptr_deref = UnaryOperator {
        Token { "#", "main.basalt", 1, 1, 1, Token::Type::symbol },
        Identifier { Token { "k", "main.basalt", 1, 1, 1, Token::Type::text } }
    };
   
    std::optional<TypeSignature> ptr_deref_type_opt = type_deducer.deduce_expression_type(ptr_deref);
    ASSERT_TRUE(ptr_deref_type_opt.has_value());
    const TypeSignature& ptr_deref_type = ptr_deref_type_opt.value();
    ASSERT_TRUE(ptr_deref_type.is<TemplateType>());
    const TemplateType& ptr_deref_template_type = ptr_deref_type.get<TemplateType>();
    EXPECT_EQ(ptr_deref_template_type.type_name, "K");
}