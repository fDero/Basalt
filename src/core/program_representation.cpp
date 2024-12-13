//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/program_representation.hpp"
#include "core/expression_type_deducer.hpp"
#include "core/assignment_type_checker.hpp"
#include "core/function_call_resolver.hpp"

ProgramRepresentation::ProgramRepresentation(
    const ProjectFileStructure& input_project_file_structure
) 
    : project_file_structure(input_project_file_structure)
    , type_definitions_register(project_file_structure)
    , function_overloads_register(project_file_structure)
    , overloading_resolution_engine(
        function_overloads_register, 
        type_definitions_register, 
        project_file_structure
    )
    , common_feature_adoption_plan_generation_engine(
        overloading_resolution_engine,
        type_definitions_register
    )
{}

std::optional<TypeSignature> ProgramRepresentation::resolve_expression_type(
    const Expression& expression, 
    ScopeContext& scope_context
) {
    return ExpressionTypeDeducer(
        type_definitions_register, 
        overloading_resolution_engine,
        common_feature_adoption_plan_generation_engine, 
        project_file_structure, 
        scope_context
    ).deduce_expression_type(expression);
}

void ProgramRepresentation::foreach_type_definition(
    std::function<void(const TypeDefinition&)> visitor
) {
    type_definitions_register.foreach_type_definition(visitor);
}

void ProgramRepresentation::foreach_function_definition(
    std::function<void(const FunctionDefinition::Ref&)> visitor
) {
    function_overloads_register.foreach_function_definition(visitor);
}

void ProgramRepresentation::verify_that_the_type_exists(
    const TypeSignature& type_signature
) {
    type_definitions_register.verify_that_the_type_exists(type_signature);
}

void ProgramRepresentation::foreach_package(
    const std::function<void(const std::string&)>& func
) {
    project_file_structure.foreach_package(func);
}

TypeDefinition ProgramRepresentation::retrieve_type_definition(
    const CustomType& type_signature
) {
    return type_definitions_register.retrieve_type_definition(type_signature);
}

TypeSignature ProgramRepresentation::unalias_type(
    const TypeSignature& type_signature
) {
    return type_definitions_register.unalias_type(type_signature);
}

std::string ProgramRepresentation::get_fully_qualified_typesignature_name(
    const TypeSignature& typesignature
) {
    return type_definitions_register.get_fully_qualified_typesignature_name(typesignature);
}

std::string ProgramRepresentation::get_fully_qualified_typedefinition_name(
    const TypeDefinition& type_definition
) {
    return type_definitions_register.get_fully_qualified_typedefinition_name(type_definition);
}

std::list<FileRepresentation>& ProgramRepresentation::get_files_by_package(
    const std::string& package_name
) {
    return project_file_structure.get_files_by_package(package_name);
}

bool ProgramRepresentation::validate_assignment(
    const std::optional<TypeSignature>& target,
    const std::optional<TypeSignature>& source
) {
    AssignmentTypeChecker assignment_type_checker(
        type_definitions_register, 
        project_file_structure
    );
    return !target.has_value() || !source.has_value() ||
        assignment_type_checker.validate_assignment(*target, *source);
}

bool ProgramRepresentation::is_void_procedure(
    const FunctionCall& function_call,
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(
        type_definitions_register, 
        overloading_resolution_engine,
        common_feature_adoption_plan_generation_engine, 
        project_file_structure, 
        scope_context
    );
    FunctionCallResolver function_call_resolver(
        overloading_resolution_engine,
        common_feature_adoption_plan_generation_engine
    );
    auto arg_types = expression_type_deducer.deduce_argument_types_from_function_call(function_call);
    auto return_type = function_call_resolver.resolve_function_call_return_type(function_call, arg_types);
    return return_type.is<FunctionCallResolver::Void>();
}