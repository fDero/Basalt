#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "language/generics.hpp"

FunctionSpecificityDescriptor::FunctionSpecificityDescriptor(
    ProgramRepresentation& program_representation,
    const FunctionDefinition& func_definition
) 
    : program_representation(program_representation)
    , func_definition(func_definition)
{
    compute_number_of_generics();
    compute_number_of_generic_parameters_usage_in_signature();
    compute_argument_types_complexity_indicator();
    compute_amount_of_unions_in_argument_types();
    compute_amount_of_cases_covered_by_argument_types_unions();
    compute_amount_of_slices_in_argument_types();
    compute_amount_of_arrays_in_argument_types();
    compute_amount_of_strings_in_argument_types();
    compute_amount_of_c_strings_in_argument_types();
}

[[nodiscard]] FunctionSpecificityDescriptor::ComparisonOutcome 
FunctionSpecificityDescriptor::compare(const FunctionSpecificityDescriptor& other) const {
    const size_t LOWER_IS_BETTER = 1;
    const size_t HIGHER_IS_BETTER = -1;
    size_t difference_array[] = {
        LOWER_IS_BETTER  * (number_of_generics - other.number_of_generics),
        LOWER_IS_BETTER  * (number_of_generic_parameters_usage_in_signature - other.number_of_generic_parameters_usage_in_signature),
        HIGHER_IS_BETTER * (argument_types_complexity_indicator - other.argument_types_complexity_indicator),
        LOWER_IS_BETTER  * (amount_of_unions_in_argument_types - other.amount_of_unions_in_argument_types),
        LOWER_IS_BETTER  * (amount_of_cases_covered_by_argument_types_unions - other.amount_of_cases_covered_by_argument_types_unions),
        LOWER_IS_BETTER  * (amount_of_slices_in_argument_types - other.amount_of_slices_in_argument_types),
        LOWER_IS_BETTER  * (amount_of_arrays_in_argument_types - other.amount_of_arrays_in_argument_types),
        LOWER_IS_BETTER  * (amount_of_strings_in_argument_types - other.amount_of_strings_in_argument_types),
        LOWER_IS_BETTER  * (amount_of_c_strings_in_argument_types - other.amount_of_c_strings_in_argument_types)
    };
    for (const size_t delta : difference_array) {
        if (delta != 0) {
            return (delta > 0) 
                ? ComparisonOutcome::MORE_SPECIFIC 
                : ComparisonOutcome::LESS_SPECIFIC;
        }
    }
    return ComparisonOutcome::EQUALLY_SPECIFIC;
}

void FunctionSpecificityDescriptor::compute_number_of_generics(){
    number_of_generics = func_definition.template_generics_names.size();
}

void FunctionSpecificityDescriptor::compute_number_of_generic_parameters_usage_in_signature(){
    std::function<size_t(const TypeSignature&)> count_function = [](const TypeSignature& type_signature){
        return (size_t) type_signature.is<TemplateType>();
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        number_of_generic_parameters_usage_in_signature += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_argument_types_complexity_indicator(){
    std::function<size_t(const TypeSignature&)> count_function = [](const TypeSignature& type_signature){
        return (size_t) 1;
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        argument_types_complexity_indicator += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_amount_of_unions_in_argument_types(){
    std::function<size_t(const TypeSignature&)> count_function = [this](const TypeSignature& type_signature){
        if (!type_signature.is<CustomType>()) {
            return (size_t) 0;
        }
        TypeDefinition def = this->program_representation.retrieve_type_definition(type_signature.get<CustomType>());
        return (size_t) def.is<UnionDefinition>();
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        amount_of_unions_in_argument_types += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_amount_of_cases_covered_by_argument_types_unions(){
    std::function<size_t(const TypeSignature&)> count_function = [this, &count_function](const TypeSignature& type_signature){
        if (!type_signature.is<CustomType>()) {
            return (size_t) 0;
        }
        TypeDefinition def = this->program_representation.retrieve_type_definition(type_signature.get<CustomType>());
        size_t count = 1;
        if (def.is<UnionDefinition>()){
            for (const TypeSignature& alternative : def.get<UnionDefinition>().types){
                count += count_recursivly_on_typesignature(alternative, count_function);
            }
        }
        return count;
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        amount_of_unions_in_argument_types += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_amount_of_slices_in_argument_types(){
    std::function<size_t(const TypeSignature&)> count_function = [](const TypeSignature& type_signature){
        return (size_t) type_signature.is<SliceType>();
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        amount_of_slices_in_argument_types += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_amount_of_arrays_in_argument_types(){
    std::function<size_t(const TypeSignature&)> count_function = [](const TypeSignature& type_signature){
        return (size_t) type_signature.is<ArrayType>();
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        amount_of_arrays_in_argument_types += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_amount_of_strings_in_argument_types(){
    std::function<size_t(const TypeSignature&)> count_function = [](const TypeSignature& type_signature){
        return (size_t) (type_signature.is<PrimitiveType>() && type_signature.get<PrimitiveType>().type_name == "String");
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        amount_of_strings_in_argument_types += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}

void FunctionSpecificityDescriptor::compute_amount_of_c_strings_in_argument_types(){
    std::function<size_t(const TypeSignature&)> count_function = [](const TypeSignature& type_signature){
        return (size_t) (type_signature.is<PrimitiveType>() && type_signature.get<PrimitiveType>().type_name == "RawString");
    };
    for (const FunctionDefinition::Argument& arg : func_definition.arguments){
        amount_of_c_strings_in_argument_types += 
            count_recursivly_on_typesignature(arg.arg_type, count_function);
    }
}



[[nodiscard]] size_t FunctionSpecificityDescriptor::count_recursivly_on_typesignature(
    const TypeSignature& type_signature,
    std::function<size_t(const TypeSignature&)> count_function
){
    size_t current_count = count_function(type_signature);
    if (type_signature.is<PrimitiveType>()){
        return current_count;
    }
    if (type_signature.is<ArrayType>()){
        return current_count + count_recursivly_on_typesignature(
            type_signature.get<ArrayType>().stored_type,
            count_function
        );
    }
    if (type_signature.is<SliceType>()){
        return current_count + count_recursivly_on_typesignature(
            type_signature.get<SliceType>().stored_type,
            count_function
        );
    }
    if (type_signature.is<PointerType>()){
        return current_count + count_recursivly_on_typesignature(
            type_signature.get<PointerType>().pointed_type,
            count_function
        );
    }
    if (type_signature.is<CustomType>()){
        const CustomType& custom_type = type_signature.get<CustomType>();
        for (const TypeSignature& generic : custom_type.instantiation_generics){
            current_count += count_recursivly_on_typesignature(generic, count_function);
        }
        return current_count;
    }
    assert_unreachable();
}