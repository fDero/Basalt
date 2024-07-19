
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
        
    private:

        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& union_def_generics);
        void verify_that_the_type_exists(const TypeSignature& type_signature);

        TypeDefinitionsRegister& program_representation;
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