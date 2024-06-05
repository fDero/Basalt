
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
        TypeDependencyNavigator(ProgramRepresentation& program_representation);
        void visit_struct_definition(const StructDefinition& struct_definition);
        void visit_union_definition(const UnionDefinition& union_definition);
        
    private:

        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& union_def_generics);
        void verify_that_the_type_exists(const TypeSignature& type_signature);

        ProgramRepresentation& program_representation;
        std::unordered_set<std::string> visited_definitions;
};

class PackageTypeConflictNavigator {
    
    public:
        PackageTypeConflictNavigator(ProgramRepresentation& program_representation);
        void visit_file(const Filerepresentation& file_representation);
        void visit_package(const std::string& package_name);
        
    private:
        ProgramRepresentation& program_representation;
        std::unordered_set<std::string> visited_files;
        std::unordered_set<std::string> type_definition_conflict_detection_patterns;

        [[nodiscard]] std::string get_type_definition_conflict_detection_pattern(
            const TypeDefinition& type_definition
        );
};

class AssignmentTypeChecker {

    public:
        AssignmentTypeChecker(ProgramRepresentation& program_representation);
        bool validate_assignment(const TypeSignature& source, const TypeSignature& dest);
        GenericSubstitutionRuleSet& get_generic_substitution_rules(); 

    private:
        bool validate_type_alias_unaware_assignment(const TypeSignature& source, const TypeSignature& dest);
        bool validate_assignment_to_slice_type(const TypeSignature& source, const SliceType& dest);
        bool validate_assignment_to_pointer_type(const TypeSignature& source, const PointerType& dest);
        bool validate_assignment_to_array_type(const TypeSignature& source, const ArrayType& dest);
        bool validate_assignment_to_primitive_type(const TypeSignature& source, const PrimitiveType& dest);
        bool validate_assignment_to_custom_type(const TypeSignature& source, const CustomType& dest);
        bool validate_assignment_to_template_generic(const TypeSignature& source, const TemplateType& dest);
        bool validate_assignment_between_custom_types(const CustomType& source, const CustomType& dest);
        bool validate_complex_assignment(const TypeSignature& source, const CustomType& dest);
        bool validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest);
        bool validate_assignment_to_union(const TypeSignature& source, const UnionDefinition& union_def);

        bool validate_assignment_between_custom_types_generic_type_parameters(const TypeSignature& source, const TypeSignature& dest);
        bool validate_type_alias_unaware_assignment_between_custom_types_generic_type_parameters(const TypeSignature& source, const TypeSignature& dest);


        bool typesignatures_are_equal(const TypeSignature& t1, const TypeSignature& t2);
        bool type_alias_unaware_typesignatures_are_equal(const TypeSignature& t1, const TypeSignature& t2);
        
        ProgramRepresentation& program_representation;
        GenericSubstitutionRuleSet generic_substitution_rules;
};