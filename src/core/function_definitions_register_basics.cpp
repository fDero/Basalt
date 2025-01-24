//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/function_definitions_register.hpp"
#include "core/function_definitions_register.hpp"

FunctionDefinitionsRegister::FunctionDefinitionsRegister(
    TypeDefinitionsRegister& type_definitions_register,
    ProjectFileStructure& project_file_structure
)
    : type_definitions_register(type_definitions_register) 
    , project_file_structure(project_file_structure)
{
    project_file_structure.foreach_file([&](const FileRepresentation& file_representation) {
        for (const auto& function_definition : file_representation.func_defs) {
            store_function_definition(function_definition);
        }
    });
}

void FunctionDefinitionsRegister::maybe_register_as_main_function(
    const FunctionDefinition::Ref& function_definition_ref,
    const std::string& package_name
) {
    if (function_definition_ref->function_name == "main") {
        main_function_definitions_and_their_packages.push_back({
            function_definition_ref,
            package_name
        });
    }
}

void FunctionDefinitionsRegister::foreach_function_definition(
    std::function<void(FunctionDefinition::Ref)> functor
) {
    for (const auto& func_def : function_definitions) {
        functor(func_def);
    }
}

void FunctionDefinitionsRegister::foreach_main_function_definition(
    std::function<void(FunctionDefinition::Ref, std::string)> functor
) {
    for (const auto& def_and_package : main_function_definitions_and_their_packages) {
        functor(def_and_package.first, def_and_package.second);
    }
}