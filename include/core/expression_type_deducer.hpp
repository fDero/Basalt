//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/type_definitions_register.hpp"
#include "core/overloading_resolution_engine.hpp"
#include "core/common_feature_adoption_plan_generation_engine.hpp"
#include "core/project_file_structure.hpp"
#include "core/scope_context.hpp"

class ExpressionTypeDeducer {

    public:

        ExpressionTypeDeducer(
            TypeDefinitionsRegister& type_definitions_register,
            OverloadingResolutionEngine& overloading_resolution_engine,
            CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine,
            ProjectFileStructure& project_file_structure,
            ScopeContext& scope_context
        );

        [[nodiscard]] std::optional<TypeSignature> deduce_expression_type(const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_primtive_type(const std::string& type_name, const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_array_literal(const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_identifier(const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_function_call(const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_type_operator(const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_binary_operator(const Expression& expression);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_unary_operator(const Expression& expression);

        [[nodiscard]] std::vector<TypeSignature> deduce_argument_types_from_function_call(const FunctionCall& function_call);

        [[nodiscard]] std::optional<TypeSignature> deduce_address_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_pointer_dereference_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_boolean_not_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_math_prefix_operator_type(const UnaryOperator& unary_op);

        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_eq_binary_operator(const BinaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_ord_binary_operator(const BinaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_logical_binary_operator(const BinaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_math_binary_operator(const BinaryOperator& unary_op);        
        
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_square_brackets_access(const Expression& square_brackets_access);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_dot_member_access(const Expression& dot_member_access);

    private:
        TypeDefinitionsRegister& type_definitions_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
        CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine;
        ProjectFileStructure& project_file_structure;
        ScopeContext& scope_context;
};