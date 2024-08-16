

#pragma once
#include "toolchain/representation.hpp"
#include "language/generics.hpp"

class AssignmentTypeChecker {

    public:
        AssignmentTypeChecker(TypeDefinitionsRegister& program_representation, ProjectFileStructure& project_file_structure);
        bool validate_assignment(const TypeSignature& source, const TypeSignature& dest);
        GenericSubstitutionRuleSet::Ref get_generic_substitution_rules(); 

    private:
        bool validate_type_alias_unaware_assignment(const TypeSignature& source, const TypeSignature& dest);
        bool validate_assignment_to_slice_type(const TypeSignature& source, const SliceType& dest);
        bool validate_assignment_to_pointer_type(const TypeSignature& source, const PointerType& dest);
        bool validate_assignment_to_array_type(const TypeSignature& source, const ArrayType& dest);
        bool validate_assignment_to_primitive_type(const TypeSignature& source, const PrimitiveType& dest);
        bool validate_assignment_to_custom_type(const TypeSignature& source, const CustomType& dest);
        bool validate_assignment_to_template_generic(const TypeSignature& source, const TemplateType& dest);
        bool name_equivalence_assignment_validation(const CustomType& source, const CustomType& dest);
        bool structural_equivalence_assignment_validation(const TypeSignature& source, const TypeSignature& dest);
        bool validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest);
        bool validate_assignment_to_inline_union(const TypeSignature& source, const InlineUnion& inline_union);
        bool validate_assignment_to_generic_type_parameter(const TypeSignature& source, const TemplateType& dest);
        bool validate_assignment_very_strictly(const TypeSignature& source_generic, const TypeSignature& dest_generic);
        bool validate_assignment_to_string_from_primitive_type(const PrimitiveType& source, const PrimitiveType& dest);
        bool validate_assignment_to_string_from_slice_type(const SliceType& source, const PrimitiveType& dest);
        bool validate_assignment_to_string_from_array_type(const ArrayType& source, const PrimitiveType& dest);

        std::vector<TypeSignature> try_to_get_union_alternatives(const TypeSignature& maybe_union_type_signature);
        bool validate_assignment_to_union_alternatives(const TypeSignature& source, const std::vector<TypeSignature>& alternatives);

        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        GenericSubstitutionRuleSet::Ref generic_substitution_rules = std::make_shared<GenericSubstitutionRuleSet>();

        bool type_parameters_assignment_validation(const CustomType &source, const CustomType &dest);
};

class ExpressionTypeDeducer {

    public:
        ExpressionTypeDeducer(
            TypeDefinitionsRegister& program_representation,
            OverloadingResolutionEngine& overloading_resolution_engine,
            ProjectFileStructure& project_file_structure,
            ScopeContext& scope_context
        );

        [[nodiscard]] TypeSignature deduce_expression_type(const Expression& expression);
        [[nodiscard]] TypeSignature deduce_primtive_type(const std::string& type_name, const Expression& expression);
        [[nodiscard]] TypeSignature deduce_type_from_identifier(const Expression& expression);
        [[nodiscard]] TypeSignature deduce_type_from_function_call(const Expression& expression);
        [[nodiscard]] TypeSignature deduce_type_from_type_operator(const Expression& expression);
        [[nodiscard]] TypeSignature deduce_type_from_binary_operator(const Expression& expression);
        [[nodiscard]] TypeSignature deduce_type_from_unary_operator(const Expression& expression);

        [[nodiscard]] TypeSignature deduce_address_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] TypeSignature deduce_pointer_dereference_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] TypeSignature deduce_boolean_not_operator_type(const UnaryOperator& unary_op);
        [[nodiscard]] TypeSignature deduce_math_prefix_operator_type(const UnaryOperator& unary_op);

        [[nodiscard]] TypeSignature deduce_type_from_comparison_operator(const BinaryOperator& unary_op);
        [[nodiscard]] TypeSignature deduce_type_from_square_brackets_access(const Expression& square_brackets_access);
        [[nodiscard]] TypeSignature deduce_type_from_dot_member_access(const Expression& dot_member_access);
        [[nodiscard]] TypeSignature deduce_type_from_math_binary_operator(const BinaryOperator& unary_op);        

    private:
        TypeDefinitionsRegister& type_definitions_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
        ProjectFileStructure& project_file_structure;
        ScopeContext& scope_context;
};