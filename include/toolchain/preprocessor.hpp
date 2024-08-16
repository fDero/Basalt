
#pragma once
#include "toolchain/tokenizer.hpp"
#include "toolchain/parser.hpp"
#include "language/syntax.hpp"
#include "language/generics.hpp"
#include <vector>
#include <string>
#include <variant>
#include <unordered_set>

class TypeDependencyNavigator {

    public:
        TypeDependencyNavigator(TypeDefinitionsRegister& program_representation);
        void visit_struct_definition(const StructDefinition& struct_definition);
        void visit_union_definition(const UnionDefinition& union_definition);
        void visit_type_definition(const TypeDefinition& type_definition);
        void verify_that_the_type_exists(const TypeSignature& type_signature);

    private:

        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& union_def_generics);

        TypeDefinitionsRegister& type_definitions_register;
        std::unordered_set<std::string> visited_definitions;
};

class PackageTypeConflictNavigator {
    
    public:
        PackageTypeConflictNavigator(ProjectFileStructure& project_file_structure);
        void visit_file(const FileRepresentation& file_representation);
        void visit_package(const std::string& package_name);
        
    private:
        ProjectFileStructure& project_file_structure;
        std::unordered_set<std::string> visited_files;
        std::unordered_set<std::string> type_definition_conflict_detection_patterns;

        [[nodiscard]] std::string get_type_definition_conflict_detection_pattern(
            const TypeDefinition& type_definition
        );
};

class FunctionDefinitionValidator {

    public:
        FunctionDefinitionValidator(
            ProjectFileStructure& project_file_structure,
            TypeDefinitionsRegister& type_definitions_register,
            FunctionOverloadsRegister& function_overloads_register,
            OverloadingResolutionEngine& overloading_resolution_engine
        );

        void validate_all_function_definitions();
        void validate_function_definition(const FunctionDefinition& function_definition);

    protected:
        
        void validate_function_body_statement(
            const Statement& statement, 
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_assignment(
            const Assignment& statement, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_identifier(
            const Identifier& target, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_unary_operator(
            const UnaryOperator& target, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_dereference_operator(
            const UnaryOperator& unary_operator, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_square_brackets_access(
            const SquareBracketsAccess& unary_operator, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_dot_member_access(
            const DotMemberAccess& unary_operator, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_return_statement(
            const Return& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_conditional(
            const Conditional& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_while_loop(
            const WhileLoop& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_until_loop(
            const UntilLoop& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_function_call(
            const FunctionCall& function_call,
            ScopeContext& scope_context
        );

    private:
        ProjectFileStructure& project_file_structure;
        TypeDefinitionsRegister& type_definitions_register;
        FunctionOverloadsRegister& function_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
};

class PreProcessor {

    public:
        PreProcessor(
            ProjectFileStructure& project_file_structure,
            TypeDefinitionsRegister& type_definitions_register,
            FunctionOverloadsRegister& function_overloads_register,
            OverloadingResolutionEngine& overloading_resolution_engine
        );

        void preprocess_packages_typename_conflicts();
        void preprocess_type_definitions();
    
    private:
        ProjectFileStructure& project_file_structure;
        TypeDefinitionsRegister& type_definitions_register;
        FunctionOverloadsRegister& function_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
};