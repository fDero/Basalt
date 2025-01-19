//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "core/program_representation.hpp"
#include "core/expression_type_deducer.hpp"
#include "core/assignment_type_checker.hpp"
#include "core/function_call_resolver.hpp"
#include "core/dot_member_access_normalizer.hpp"
#include "errors/internal_errors.hpp"

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
    const std::optional<TypeSignature>& source,
    const std::optional<TypeSignature>& dest
) {
    AssignmentTypeChecker assignment_type_checker(
        type_definitions_register, 
        project_file_structure
    );
    return !source.has_value() || !dest.has_value() ||
        assignment_type_checker.validate_assignment(*source, *dest);
}

bool ProgramRepresentation::is_union(
    const TypeSignature& maybe_union_type
) {
    return type_definitions_register.is_union(maybe_union_type);
}

std::vector<TypeSignature> ProgramRepresentation::fetch_union_alternatives(
    const TypeSignature& maybe_union_type
) {
    return type_definitions_register.fetch_union_alternatives(maybe_union_type);
}

std::vector<TypeSignature> ProgramRepresentation::fetch_non_union_compatible_types_for_union_assignment(
    const TypeSignature& maybe_union_type
) {
    return type_definitions_register.fetch_non_union_compatible_types_for_union_assignment(maybe_union_type);
}


std::vector<TypeSignature> ProgramRepresentation::fetch_non_union_compatible_types_for_union_assignment(
    const UnionDefinition& union_definition
) {
    return type_definitions_register.fetch_non_union_compatible_types_for_union_assignment(union_definition);
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
        type_definitions_register,
        overloading_resolution_engine,
        common_feature_adoption_plan_generation_engine
    );
    auto arg_types = expression_type_deducer.deduce_argument_types_from_function_call(function_call);
    auto return_type = function_call_resolver.resolve_function_call_return_type(function_call, arg_types);
    return return_type.is<FunctionCallResolver::Void>();
}

CallableCodeBlock ProgramRepresentation::resolve_function_call(
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
        type_definitions_register,
        overloading_resolution_engine,
        common_feature_adoption_plan_generation_engine
    );
    auto arg_types = expression_type_deducer.deduce_argument_types_from_function_call(function_call);
    return function_call_resolver.resolve_function_call(function_call, arg_types);
}

DotMemberAccess ProgramRepresentation::normalize_dot_member_access(
    const DotMemberAccess& dot_member_access, 
    ScopeContext& scope_context
) {
    ExpressionTypeDeducer expression_type_deducer(
        type_definitions_register,
        overloading_resolution_engine,
        common_feature_adoption_plan_generation_engine,
        project_file_structure,
        scope_context
    );
    DotMemberAccessNormalizer dot_member_access_normalizer(
        expression_type_deducer,
        dot_member_access
    );
    return dot_member_access_normalizer
        .normalize_dot_member_access();
}

size_t ProgramRepresentation::resolve_field_index(
    const DotMemberAccess& dot_member_access, 
    ScopeContext& scope_context
) {
    std::optional<TypeSignature> target_type_opt = 
        resolve_expression_type(dot_member_access.struct_value, scope_context);
    assert_type_deduction_success_in_backend_layer(target_type_opt.has_value());
    TypeSignature target_type = target_type_opt.value();
    TypeSignature unaliased_target_type = unalias_type(target_type);
    assert_typesignature_is<CustomType>(unaliased_target_type);
    CustomType target_custom_type = unaliased_target_type.get<CustomType>();
    TypeDefinition target_type_definition = retrieve_type_definition(target_custom_type);
    assert_typedefinition_is<StructDefinition>(target_type_definition);
    const StructDefinition& target_struct_definition = target_type_definition.get<StructDefinition>();
    for (size_t field_index = 0; field_index < target_struct_definition.fields.size(); field_index++) {
        const StructDefinition::Field& field = target_struct_definition.fields[field_index];
        if (field.field_name == dot_member_access.member_name) {
            return field_index;
        }
    }
    assert_unreachable();
}

[[nodiscard]] bool ProgramRepresentation::is_binary_operator_over_float_operands(
    const BinaryOperator& binary_op, 
    ScopeContext& scope_context
) {
    auto lx_type_opt = resolve_expression_type(binary_op.left_operand, scope_context);
    auto rx_type_opt = resolve_expression_type(binary_op.right_operand, scope_context);
    return lx_type_opt.has_value() && rx_type_opt.has_value() 
        && lx_type_opt.value().is<PrimitiveType>() 
        && rx_type_opt.value().is<PrimitiveType>()
        && lx_type_opt.value().get<PrimitiveType>().type_name == float_type
        && rx_type_opt.value().get<PrimitiveType>().type_name == float_type;
}

[[nodiscard]] bool ProgramRepresentation::is_unary_operator_over_float_operand(
    const UnaryOperator& binary_op, 
    ScopeContext& scope_context
) {
    auto type_opt = resolve_expression_type(binary_op.operand, scope_context);
    return type_opt.has_value()
        && type_opt.value().is<PrimitiveType>() 
        && type_opt.value().get<PrimitiveType>().type_name == float_type;
}