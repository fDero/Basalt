
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "misc/forward_declarations.hpp"
#include <unordered_map>
#include <map>
#include <functional>
#include <list>

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

class ScopeContext {

    public:
        void store_local_variable(const VariableDeclaration& var_declaration);
        void store_local_constant(const ConstDeclaration& const_declaration);
        
        [[nodiscard]] bool contains(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_object_type(const std::string& identifier);
        [[nodiscard]] TypeSignature& get_local_mutable_object_type(const std::string& identifier);
        [[nodiscard]] ScopeContext   create_nested_scope();

    private:
        struct ObjectDescriptor {
            std::string identifier;
            TypeSignature type_signature;
            bool is_const = false; 
            bool gets_modified = false;
            bool gets_accessed = false;
        };

        std::vector<ObjectDescriptor> local_objects;
        ScopeContext* parent_scope = nullptr;
};

class FunctionSpecificityDescriptor {

    public:

        enum class ComparisonOutcome {
            MORE_SPECIFIC,
            LESS_SPECIFIC,
            EQUALLY_SPECIFIC
        };

        FunctionSpecificityDescriptor(
            ProgramRepresentation& program_representation,
            const FunctionDefinition& func_definition
        );

        [[nodiscard]] ComparisonOutcome compare(const FunctionSpecificityDescriptor& other) const;
        [[nodiscard]] ComparisonOutcome compare(const std::optional<FunctionSpecificityDescriptor>& other) const;
        
    private:
        size_t number_of_generics = 0;
        size_t number_of_generic_parameters_usage_in_signature = 0;
        size_t argument_types_complexity_indicator = 0;
        size_t amount_of_unions_in_argument_types = 0;
        size_t amount_of_cases_covered_by_argument_types_unions = 0;
        size_t amount_of_slices_in_argument_types = 0;
        size_t amount_of_arrays_in_argument_types = 0;
        size_t amount_of_strings_in_argument_types = 0;
        size_t amount_of_c_strings_in_argument_types = 0;
        
        void compute_number_of_generics(const FunctionDefinition&);
        void compute_number_of_generic_parameters_usage_in_signature(const FunctionDefinition&);
        void compute_argument_types_complexity_indicator(const FunctionDefinition&);
        void compute_amount_of_unions_in_argument_types(ProgramRepresentation&, const FunctionDefinition&);
        void compute_amount_of_cases_covered_by_argument_types_unions(ProgramRepresentation&, const FunctionDefinition&);
        void compute_amount_of_slices_in_argument_types(const FunctionDefinition&);
        void compute_amount_of_arrays_in_argument_types(const FunctionDefinition&);
        void compute_amount_of_strings_in_argument_types(const FunctionDefinition&);
        void compute_amount_of_c_strings_in_argument_types(const FunctionDefinition&);

        [[nodiscard]] size_t count_recursively_on_typesignature(
            const TypeSignature& type_signature,
            const std::function<size_t(const TypeSignature&)>& count_function
        );
};

struct ProgramRepresentation {

    public:

        void store_definitions_from_file(const FileRepresentation& file_representation);
    
        void store_type_definition(const TypeDefinition& type_definition, const std::string& package_name);
        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);

        void store_function_definition(const FunctionDefinition& func_definition, const std::string& package_name);
        [[nodiscard]] FunctionDefinition retrieve_func_definition(const PrecompiledFunctionCall& function_call);

        TypeSignature unalias_type(const TypeSignature& type_signature);

    protected:
        
        std::unordered_map<std::string, std::string> package_name_by_file_name;
        std::unordered_map<std::string, std::vector<FileRepresentation>> files_by_package;
        std::unordered_map<std::string, std::vector<std::string>> imports_by_file;

        std::unordered_map<std::string, TypeDefinition> type_definitions;
         
        [[nodiscard]] std::string get_fully_qualified_customtype_name(const CustomType& type_signature);
        [[nodiscard]] std::string get_fully_qualified_typesignature_name(const TypeSignature& type_signature);
        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(const CustomType&, const std::string&);
        [[nodiscard]] std::string get_type_definition_match_pattern(const std::string&, const TypeDefinition&);
        [[nodiscard]] std::string get_type_signature_match_pattern(const std::string&, const CustomType&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name(const std::string&, const TypeSignature&);
        [[nodiscard]] std::string infer_possible_fully_qualified_customtype_name(const std::string &packageName, const CustomType &custom_type);

        using FunctionDefinitionRef = std::shared_ptr<FunctionDefinition>;
        using FunctionOverloadSet = std::vector<FunctionDefinitionRef>;

        std::unordered_map<std::string, FunctionOverloadSet> function_definitions_overload_sets;
        std::unordered_map<std::string, FunctionDefinitionRef> function_definitions;
        std::unordered_map<FunctionDefinitionRef, FunctionSpecificityDescriptor> function_definitions_specificity;

        [[nodiscard]] std::list<FunctionDefinitionRef> search_compatible_function_definitions_within_given_overload_set(
            const PrecompiledFunctionCall& precompiled_function_call, 
            const std::string& overload_set_id
        );

        [[nodiscard]] std::optional<FunctionDefinitionRef> search_compatible_function_definitions_within_given_package(
            const PrecompiledFunctionCall& precompiled_function_call, 
            const std::string& package_name
        );

        [[nodiscard]] std::optional<FunctionDefinitionRef> check_function_definition_compatibility(
            const FunctionDefinitionRef& function_definition_ref,
            const PrecompiledFunctionCall& precompiled_function_call
        );

        [[nodiscard]] std::string get_function_definition_overload_set_id(const std::string&, const FunctionDefinition&);
        [[nodiscard]] std::string get_generics_unaware_function_definition_overload_set_id(const std::string&, const FunctionDefinition&);
        [[nodiscard]] std::string get_function_call_overload_set_id(const std::string&, const PrecompiledFunctionCall&);

        [[nodiscard]] std::string get_function_fast_retrieve_key(
            const std::string& package_name,
            const FunctionDefinitionRef& function_definition_ref
        );

        [[nodiscard]] std::string get_function_fast_retrieve_key(
            const std::string& package_name,
            const PrecompiledFunctionCall& precompiled_function_call
        );

        void store_function_definition_ref(
            const FunctionDefinitionRef& func_definition_ref, 
            const std::string& package_name
        );


    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};