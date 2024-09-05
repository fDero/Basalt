
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "typesystem/expression_type_deducer.hpp"

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
            FunctionDefinitionFactory::make_void_function_definition(
                "add", "main.basalt", FunctionDefinitionFactory::no_generics, {
                    FunctionDefinition::Argument {
                        "a", TypeSignatureFactory::Int
                    },
                    FunctionDefinition::Argument {
                        "b", TypeSignatureFactory::Int
                    },
                }
            ),
            FunctionDefinitionFactory::make_void_function_definition(
                "add", "main.basalt", FunctionDefinitionFactory::no_generics, {
                    FunctionDefinition::Argument {
                        "a", TypeSignatureFactory::Float
                    },
                    FunctionDefinition::Argument {
                        "b", TypeSignatureFactory::Float
                    },
                }
            )
        }
    }
});

static TypeDefinitionsRegister simple_type_register(simple_project);
static FunctionOverloadsRegister simple_overloads_register(simple_project);
static OverloadingResolutionEngine simple_overloading_resolution_engine(simple_overloads_register, simple_type_register, simple_project);
static ScopeContext simple_scope_context(ScopeContext::ScopeKind::function_scope);
static ExpressionTypeDeducer type_deducer(
    simple_type_register,
    simple_overloading_resolution_engine,
    simple_project,
    simple_scope_context    
);

TEST(TypeChecking, Type_Deduction_Cannot_Figure_Out_Wich_Overload_To_Use) {
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
    std::optional<TypeSignature> function_call_type_opt = type_deducer.deduce_expression_type(function_call);
    EXPECT_FALSE(function_call_type_opt.has_value());
}

TEST(TypeChecking, Type_Deduction_Can_Still_Spot_Errors_With_Function_Calls_Even_When_Type_Is_Already_Known_To_Be_Undecidible) {
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

TEST(TypeChecking, Type_Deduction_Cannot_Figure_What_Operator_To_Use) {
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

