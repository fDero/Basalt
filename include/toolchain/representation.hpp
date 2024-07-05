
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

class ProgramRepresentation {

    public:

        void store_definitions_from_file(const FileRepresentation& file_representation);
    
        void store_type_definition(const TypeDefinition& type_definition, const std::string& package_name);
        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);

        TypeSignature unalias_type(const TypeSignature& type_signature);

    protected:
         
        // types
        [[nodiscard]] std::string get_fully_qualified_customtype_name(const CustomType& type_signature);
        [[nodiscard]] std::string get_fully_qualified_typesignature_name(const TypeSignature& type_signature);
        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(const CustomType&, const std::string&);
        [[nodiscard]] std::string get_type_definition_match_pattern(const std::string&, const TypeDefinition&);
        [[nodiscard]] std::string get_type_signature_match_pattern(const std::string&, const CustomType&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name(const std::string&, const TypeSignature&);
        [[nodiscard]] std::string infer_possible_fully_qualified_customtype_name(const std::string &packageName, const CustomType &custom_type);

        // functions
        // TODO

    private:

        //project file structure        
        std::unordered_map<std::string, std::string> package_name_by_file_name;
        std::unordered_map<std::string, std::vector<FileRepresentation>> files_by_package;
        std::unordered_map<std::string, std::vector<std::string>> imports_by_file;

        // types
        std::unordered_map<std::string, TypeDefinition> type_definitions;

        // functions
        // TODO

    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};