/**
 * @file expression_type_deducer.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the ExpressionTypeDeducer class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "model/type_definitions_register.hpp"
#include "model/overloading_resolution_engine.hpp"
#include "model/project_file_structure.hpp"
#include "model/scope_context.hpp"

/**
 * @brief   Used to deduce the type of an expression.
 * 
 * @details The ExpressionTypeDeducer is used to deduce the type of an expression. It is used during the preprocessing phase
 *          togheter with AssignmentTypeChecker to check the correctness of assignments and return values. 
 * 
 * @note    The ExpressionTypeDeducer class needs to be constructed from a TypeDefinitionsRegister
 *          in order to be able to work. The TypeDefinitionsRegister must be already checked for correctness, 
 *          otherwise the ExpressionTypeDeducer could fall in an infinite loop while trying to resolve 
 *          the definitions of the types that contains a direct cyclic dependency
 * 
 * @see     TypeDefinitionsRegister
 * @see     TypeDependencyNavigator
 * @see     AssignmentTypeChecker
 */
class ExpressionTypeDeducer {

    public:

        ExpressionTypeDeducer(
            TypeDefinitionsRegister& program_representation,
            OverloadingResolutionEngine& overloading_resolution_engine,
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

        [[nodiscard]] std::optional<TypeSignature> deduce_address_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_pointer_dereference_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_boolean_not_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_math_prefix_operator_type(const UnaryOperator& unary_op);

        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_comparison_operator(const BinaryOperator& unary_op);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_square_brackets_access(const Expression& square_brackets_access);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_dot_member_access(const Expression& dot_member_access);
        [[nodiscard]] std::optional<TypeSignature> deduce_type_from_math_binary_operator(const BinaryOperator& unary_op);        

    private:
        TypeDefinitionsRegister& type_definitions_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
        ProjectFileStructure& project_file_structure;
        ScopeContext& scope_context;
};