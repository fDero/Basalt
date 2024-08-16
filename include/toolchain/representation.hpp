
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "misc/forward_declarations.hpp"
#include <unordered_map>
#include <map>
#include <functional>
#include <list>
#include "language/generics.hpp"

struct FileRepresentation {

    struct Metadata {
        std::string filename;
        std::string packagename;
        std::vector<std::string> imports;
    };

    Metadata file_metadata;
    std::vector<TypeDefinition> type_defs;
    std::vector<FunctionDefinition> func_defs;
};


class ProjectFileStructure {

    public:

        ProjectFileStructure() = default;
        ProjectFileStructure(const std::vector<FileRepresentation>& file_representations);

        void store_file_representation(const FileRepresentation& file_representation);
        
        std::string& get_package_name_by_file_name(const std::string& file_name);
        std::vector<FileRepresentation>& get_files_by_package(const std::string& package_name);
        std::vector<std::string>& get_imports_by_file(const std::string& file_name);
        
        const std::string& get_package_name_by_file_name(const std::string& file_name) const;
        const std::vector<FileRepresentation>& get_files_by_package(const std::string& package_name) const;
        const std::vector<std::string>& get_imports_by_file(const std::string& file_name) const;
        const std::unordered_map<std::string, std::vector<FileRepresentation>>& get_all_files_grouped_by_package() const;

    private:

        std::unordered_map<std::string, std::string> package_name_by_file_name;
        std::unordered_map<std::string, std::vector<FileRepresentation>> files_by_package;
        std::unordered_map<std::string, std::vector<std::string>> imports_by_file;
};


class TypeDefinitionsRegister {

    public:
    
        TypeDefinitionsRegister(ProjectFileStructure& project_file_structure);

        void store_type_definition(const TypeDefinition& type_definition, const std::string& package_name);
        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);
        [[nodiscard]] std::string get_fully_qualified_typesignature_name(const TypeSignature& type_signature);
        [[nodiscard]] TypeSignature unalias_type(const TypeSignature& type_signature);
        [[nodiscard]] std::unordered_map<std::string, TypeDefinition>& get_all_type_definitions();

    protected:
         
        [[nodiscard]] std::string get_fully_qualified_customtype_name(const CustomType& type_signature);
        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(const CustomType&, const std::string&);
        [[nodiscard]] std::string get_type_definition_match_pattern(const std::string&, const TypeDefinition&);
        [[nodiscard]] std::string get_type_signature_match_pattern(const std::string&, const CustomType&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name(const std::string&, const TypeSignature&);
        [[nodiscard]] std::string infer_possible_fully_qualified_customtype_name(const std::string &packageName, const CustomType &custom_type);

    private:

        std::unordered_map<std::string, TypeDefinition> type_definitions;
        ProjectFileStructure& project_file_structure;

    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};

class FunctionOverloadsRegister {

    public:

        FunctionOverloadsRegister(ProjectFileStructure& project_file_structure);
        
        void store_function_definition(const FunctionDefinition& function_definition, const std::string& package_name);
        
        [[nodiscard]] std::vector<std::string> retrieve_overload_sets_ids(const FunctionCall& function_call);
        [[nodiscard]] std::vector<FunctionDefinition::Ref>& retrieve_specific_overload_set(const std::string& overload_set_id);
        [[nodiscard]] std::string get_function_default_search_key(const FunctionCall& function_call);
        [[nodiscard]] const std::unordered_map<std::string, FunctionDefinition::OverloadSet>& get_all_function_overload_sets() const;

    protected:

        [[nodiscard]] std::string get_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition
        );

        [[nodiscard]] std::string get_generics_unaware_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition
        );

        [[nodiscard]] std::string get_function_call_overload_set_id(
            const std::string& package_name, 
            const FunctionCall& original_function_call
        );

    private:

        ProjectFileStructure& project_file_structure;
        std::unordered_map<std::string, FunctionDefinition::OverloadSet> function_definitions_overload_sets;
};


class ScopeContext {

    public:

        enum class ScopeKind {
            function_scope,
            loop_scope,
            conditional_scope
        };

        ScopeContext(const ScopeKind& scope_kind);
        ScopeContext(const std::vector<FunctionDefinition::Argument>& arguments);

        void store_local_variable(const VariableDeclaration& var_declaration);
        void store_local_constant(const ConstDeclaration& const_declaration);
        
        [[nodiscard]] bool contains(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_object_type(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_mutable_object_type(const std::string& identifier);
        [[nodiscard]] ScopeContext   create_nested_scope(const ScopeKind& scope_kind);
        
        [[nodiscard]] ScopeKind get_scope_kind() const;

    private:
    
        struct ObjectDescriptor {
            std::string identifier;
            TypeSignature type_signature;
            bool is_const = false; 
            bool is_arg = false;
            bool gets_modified = false;
            bool gets_accessed = false;
        };

        std::vector<ObjectDescriptor> local_objects;
        ScopeContext* parent_scope = nullptr;
        ScopeKind scope_kind;
};

class FunctionSpecificityDescriptor {

    public:

        FunctionSpecificityDescriptor(
            const FunctionDefinition& function_definition,
            TypeDefinitionsRegister& type_definitions_register
        );

        FunctionSpecificityDescriptor(
            bool is_generic, 
            size_t number_of_uses_of_its_generic_parameters,
            size_t arguments_types_complexity_score,
            size_t number_of_unions_in_arguments_types,
            size_t number_of_cases_covered_by_unions_in_arguments_types,
            size_t number_of_possible_type_conversions
        );

        enum class ComparisonResult {
            less_specific,
            equally_specific,
            more_specific
        };

        ComparisonResult compare_with(const FunctionSpecificityDescriptor& other) const;

        static FunctionSpecificityDescriptor worst_possible_specificity();

    protected:

        void update_indices_based_on_argument_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_primitive_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_pointer_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_array_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );
        
        void update_indicies_based_on_slice_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_inline_union_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_indicies_based_on_custom_type(
            const TypeSignature& arg_type, 
            TypeDefinitionsRegister& type_definitions_register,
            bool current_type_is_fixed_and_cannot_be_an_upcasting_target
        );

        void update_union_covered_cases(
            const std::vector<TypeSignature>& alternatives, 
            TypeDefinitionsRegister& type_definitions_register
        );

    private:

        bool   is_generic = false;
        size_t number_of_uses_of_its_generic_parameters = 0;
        size_t arguments_types_complexity_score = 0;
        size_t number_of_unions_in_arguments_types = 0;
        size_t number_of_cases_covered_by_unions_in_arguments_types = 0;
        size_t number_of_possible_type_conversions = 0;
};

class OverloadingResolutionEngine {

    public:

        OverloadingResolutionEngine(
            FunctionOverloadsRegister& function_overloads_register, 
            TypeDefinitionsRegister& type_definitions_register,
            ProjectFileStructure& project_file_structure
        );

        FunctionDefinition::Ref retrieve_function_definition(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

        GenericSubstitutionRuleSet::Ref check_function_compatibility(
            const FunctionDefinition::Ref func_def_ref,
            const FunctionCall& func_call,
            const std::vector<TypeSignature>& arg_types
        );

    protected:
        
        FunctionDefinition::Ref cache_unaware_function_definition_retrieval(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    private:
        FunctionOverloadsRegister& function_overloads_register;
        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        std::unordered_map<std::string, FunctionDefinition::Ref> fast_retrieve_cache;
};