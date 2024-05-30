
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

    protected:

        [[nodiscard]] std::string get_fully_quilified_typesignature_name(
            const CustomType& type_signature
        );

        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(
            const CustomType& type_signature, const PackageName& package_name
        );

        [[nodiscard]] std::string get_type_definition_match_pattern(
            const PackageName& packageName, const TypeDefinition& type_definition
        );

        [[nodiscard]] std::string get_type_signature_match_pattern(
            const PackageName& packageName, const CustomType& type_signature
        );

        [[nodiscard]] std::string infer_possible_fully_qualified_name(
            const PackageName& packageName, 
            const TypeSignature& type_signature
        );

        std::unordered_map<FileName, PackageName> package_name_by_file_name;
        std::unordered_map<PackageName, std::vector<Filerepresentation>> files_by_package;
        std::unordered_map<FileName, std::vector<PackageName>> imports_by_file;

        std::unordered_map<std::string, TypeDefinition> type_definitions;

        using FunctionOverloadSet = std::vector<FunctionDefinition>;
        using PackageFunctionDefinitions = std::map<std::string, FunctionOverloadSet>;
        std::unordered_map<PackageName, PackageFunctionDefinitions> functions_by_package;

    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};
