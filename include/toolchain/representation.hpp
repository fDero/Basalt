
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "misc/aliases.hpp"
#include <unordered_map>
#include <map>

struct Filerepresentation {

    struct Metadata {
        FileName filename;
        PackageName packagename;
        std::vector<PackageName> imports;
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


struct ProgramRepresentation {

    public:

        void store_definitions_from_file(const Filerepresentation& file_representation);
    
        void store_type_definition(const TypeDefinition& type_definition, const PackageName& package_name);
        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);

        void store_function_definition(const FunctionDefinition& func_definition, const PackageName& package_name);
        [[nodiscard]] FunctionDefinition retrieve_func_definition(const PrecompiledFunctionCall& function_call);

    protected:
        
        std::unordered_map<FileName, PackageName> package_name_by_file_name;
        std::unordered_map<PackageName, std::vector<Filerepresentation>> files_by_package;
        std::unordered_map<FileName, std::vector<PackageName>> imports_by_file;

        std::unordered_map<std::string, TypeDefinition> type_definitions;
         
        [[nodiscard]] std::string get_fully_quilified_customtype_name(const CustomType& type_signature);
        [[nodiscard]] std::string get_fully_quilified_typesignature_name(const TypeSignature& type_signature);
        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(const CustomType&, const PackageName&);
        [[nodiscard]] std::string get_type_definition_match_pattern(const PackageName&, const TypeDefinition&);
        [[nodiscard]] std::string get_type_signature_match_pattern(const PackageName&, const CustomType&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name(const PackageName&, const TypeSignature&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name_for_custom_type(const PackageName&, const CustomType&);

        using FunctionOverloadSet = std::vector<FunctionDefinition>;
        
        struct FunctionDefinitionFastRetrieveKey {
            std::string overload_set_id;
            size_t index_within_overload_set;
        };

        std::unordered_map<std::string, FunctionOverloadSet> function_definitions_overload_sets;
        std::unordered_map<std::string, FunctionDefinitionFastRetrieveKey> function_definitions;

        [[nodiscard]] std::vector<FunctionDefinition> search_compatible_function_definitions_within_given_overload_set(
            const PrecompiledFunctionCall& precompiled_function_call, 
            const std::string& overload_set_id
        );

        [[nodiscard]] std::optional<FunctionDefinition> search_compatible_function_definitions_within_given_package(
            const PrecompiledFunctionCall& precompiled_function_call, 
            const PackageName& package_name
        );

        [[nodiscard]] std::string get_function_definition_overload_set_id(const PackageName&, const FunctionDefinition&);
        [[nodiscard]] std::string get_generics_unaware_function_definition_overload_set_id(const PackageName&, const FunctionDefinition&);
        [[nodiscard]] std::string get_function_call_overload_set_id(const PackageName&, const PrecompiledFunctionCall&);

        [[nodiscard]] std::string get_function_fast_retrieve_key(
            const PackageName& package_name,
            const std::string& func_name, 
            const std::vector<TypeSignature>& arguments_types
        );         


    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};
