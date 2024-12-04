
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/expression_type_deducer.hpp"

#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

static ProjectFileStructure empty_project;
static TypeDefinitionsRegister empty_type_register(empty_project);
static FunctionOverloadsRegister empty_overloads_register(empty_project);
static OverloadingResolutionEngine empty_overloading_resolution_engine(empty_overloads_register, empty_type_register, empty_project);
static CommonFeatureAdoptionPlanGenerationEngine empty_common_feature_adoption_plan_generation_engine(empty_overloading_resolution_engine, empty_type_register);
static ScopeContext empty_scope_context;

static ExpressionTypeDeducer type_deducer(
    empty_type_register,
    empty_overloading_resolution_engine,
    empty_common_feature_adoption_plan_generation_engine,
    empty_project,
    empty_scope_context
);

static Expression sum_of_two_integer_literals = BinaryOperator {
    Token { "+", "main.basalt", 1, 1, 1, Token::Type::symbol },
    IntLiteral { Token { "1", "main.basalt", 1, 1, 1, Token::Type::integer_literal } },
    IntLiteral { Token { "2", "main.basalt", 1, 1, 1, Token::Type::integer_literal } }
};

static Expression square_brackets_access_on_int_array = SquareBracketsAccess {
    Token { "[", "main.basalt", 1, 1, 1, Token::Type::symbol },
    ArrayLiteral {
        2,
        PrimitiveType { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::text } },
        {
            IntLiteral { Token { "1", "main.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "2", "main.basalt", 1, 1, 1, Token::Type::integer_literal } }
        },
        Token { "[", "main.basalt", 1, 1, 1, Token::Type::symbol },
    },
    IntLiteral { Token { "0", "main.basalt", 1, 1, 1, Token::Type::integer_literal } }
};

static Expression illegal_square_brackets_access_on_int_literal = SquareBracketsAccess {
    Token { "[", "main.basalt", 1, 1, 1, Token::Type::symbol },
    IntLiteral { Token { "0", "main.basalt", 1, 1, 1, Token::Type::integer_literal } },
    IntLiteral { Token { "0", "main.basalt", 1, 1, 1, Token::Type::integer_literal } }
};

TEST(Core, Type_Deduction_Of_Sum_Of_Two_Integer_Literals) {
    std::optional<TypeSignature> sum_of_two_integer_literals_type_opt = type_deducer.deduce_expression_type(sum_of_two_integer_literals);
    ASSERT_TRUE(sum_of_two_integer_literals_type_opt.has_value());
    const TypeSignature& sum_of_two_integer_literals_type = sum_of_two_integer_literals_type_opt.value();
    ASSERT_TRUE(sum_of_two_integer_literals_type.is<PrimitiveType>());
    const PrimitiveType& sum_of_two_integer_literals_primitive_type = sum_of_two_integer_literals_type.get<PrimitiveType>();
    EXPECT_EQ(sum_of_two_integer_literals_primitive_type.type_name, "Int");
}

TEST(Core, Type_Deduction_Of_Square_Brackets_Access_On_Array_Literal_Of_Integers) {
    std::optional<TypeSignature> square_brackets_access_on_int_array_type_opt = type_deducer.deduce_expression_type(square_brackets_access_on_int_array);
    ASSERT_TRUE(square_brackets_access_on_int_array_type_opt.has_value());
    const TypeSignature& square_brackets_access_on_int_array_type = square_brackets_access_on_int_array_type_opt.value();
    ASSERT_TRUE(square_brackets_access_on_int_array_type.is<PrimitiveType>());
    const PrimitiveType& square_brackets_access_on_int_array_primitive_type = square_brackets_access_on_int_array_type.get<PrimitiveType>();
    EXPECT_EQ(square_brackets_access_on_int_array_primitive_type.type_name, "Int");
}

TEST(Core, Type_Deduction_Of_Illegal_Square_Brackets_Access_On_Int_Literal_Throws_Exception) {
    EXPECT_ANY_THROW({
        std::optional<TypeSignature> illegal_square_brackets_access_on_int_literal_type_opt = 
            type_deducer.deduce_expression_type(illegal_square_brackets_access_on_int_literal);
    });
}