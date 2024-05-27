
#include "toolchain/representation.hpp"
#include "toolchain/preprocessor.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

AssignmentTypeChecker::AssignmentTypeChecker(ProgramRepresentation& program_representation) 
    : program_representation(program_representation) {}

GenericSubstitutionRuleSet& AssignmentTypeChecker::get_generic_substitution_rules() { 
    return generic_substitution_rules; 
}

bool AssignmentTypeChecker::validate_assignment(const TypeSignature& source, const TypeSignature& dest){
    if (validate_type_alias_unaware_assignment(source, dest)){
        return true;
    }
    if (source.is<BaseType>()){
        TypeDefinition source_type_definition = program_representation.retrieve_type_definition(source.get<BaseType>());
        if (source_type_definition.is<TypeAlias>()){
            if (validate_assignment(source_type_definition.get<TypeAlias>().aliased_type, dest)){
                return true;
            }
        }
    }
    if (dest.is<BaseType>()){
        TypeDefinition dest_type_definition = program_representation.retrieve_type_definition(dest.get<BaseType>());
        if (dest_type_definition.is<TypeAlias>()){
            if (validate_assignment(source, dest_type_definition.get<TypeAlias>().aliased_type)){
                return true;
            }
        }
    }
    return false;
}

bool AssignmentTypeChecker::validate_type_alias_unaware_assignment(const TypeSignature& source, const TypeSignature& dest){
    if (dest.is<TemplateType>()){
        return validate_assignment_to_template_generic(source, dest.get<TemplateType>());
    }
    else if (dest.is<BaseType>()){
        return validate_assignment_to_base_type(source, dest.get<BaseType>());
    }
    else if (dest.is<PrimitiveType>()){
        return validate_assignment_to_primitive_type(source, dest.get<PrimitiveType>());
    }
    else if (dest.is<ArrayType>()){
        return validate_assignment_to_array_type(source, dest.get<ArrayType>());
    }
    else if (dest.is<PointerType>()){
        return validate_assignment_to_pointer_type(source, dest.get<PointerType>());
    }
    else if (dest.is<SliceType>()){
        return validate_assignment_to_slice_type(source, dest.get<SliceType>());
    }
    assert_unreachable();
}

bool AssignmentTypeChecker::validate_assignment_to_slice_type(const TypeSignature& source, const SliceType& dest){
    if (source.is<ArrayType>()){
        return validate_assignment(source.get<ArrayType>().stored_type, dest.stored_type);
    }
    else if (source.is<SliceType>()){
        return validate_assignment(source.get<SliceType>().stored_type, dest.stored_type);
    }
    return false;
}

bool AssignmentTypeChecker::validate_assignment_to_pointer_type(const TypeSignature& source, const PointerType& dest){
    if (source.is<PointerType>()){
        return validate_assignment(source.get<PointerType>().pointed_type, dest.pointed_type);
    }
    else {
        return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_array_type(const TypeSignature& source, const ArrayType& dest){
    if (source.is<ArrayType>()){
        return validate_assignment(source.get<ArrayType>().stored_type, dest.stored_type);
    }
    else {
        return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_template_generic(const TypeSignature& source, const TemplateType& dest){
    for (GenericSubstitutionRule& rule : generic_substitution_rules){
        if (rule.to_be_substituded == dest.type_name){
            if (validate_assignment(rule.replacement, source)){
                rule.replacement = source;
                return true;
            }
            else {
                return validate_assignment(source, rule.replacement);
            }
        }
    }
    generic_substitution_rules.push_back({dest.type_name, source});
    return true;
}

bool AssignmentTypeChecker::validate_assignment_to_base_type(const TypeSignature& source, const BaseType& dest){
    return (source.is<BaseType>())
        ? validate_assignment_between_base_types(source.get<BaseType>(), dest)
        : validate_complex_assignment(source, dest);
}

bool AssignmentTypeChecker::validate_assignment_to_primitive_type(const TypeSignature& source, const PrimitiveType& dest){
    if (dest.type_name == "String" || dest.type_name == "RawString"){
        return validate_assignment_to_string(source, dest);
    }
    else if (source.is<PrimitiveType>()){
        return source.get<PrimitiveType>().type_name == dest.type_name;
    }
    else {
        return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_union(const TypeSignature& source, const UnionDefinition& union_def){
    for (const TypeSignature& alternative : union_def.types){
        if (validate_assignment(source, alternative)){
            return true;
        }
    }
    return false;
}

bool AssignmentTypeChecker::validate_complex_assignment(const TypeSignature& source, const BaseType& dest){
    const TypeDefinition& dest_type_definition = program_representation.retrieve_type_definition(dest);
    if (dest_type_definition.is<UnionDefinition>()){    
        if (validate_assignment_to_union(source, dest_type_definition.get<UnionDefinition>())){
            return true;
        }
    }
    if (!source.is<BaseType>()){
        return false;
    }
    const BaseType& source_base_type = source.get<BaseType>();
    const TypeDefinition& source_type_definition = program_representation.retrieve_type_definition(source_base_type);
    if (!source_type_definition.is<UnionDefinition>() || !dest_type_definition.is<UnionDefinition>()){
        return false;
    }
    const UnionDefinition& source_union_definition = source_type_definition.get<UnionDefinition>();
    const UnionDefinition& dest_union_definition = dest_type_definition.get<UnionDefinition>();
    for (const TypeSignature& source_alternative : source_union_definition.types){
        if (!validate_assignment_to_union(source_alternative, dest_union_definition)){
            return false;
        }
    }
    return true;
}

bool AssignmentTypeChecker::validate_assignment_between_base_types(const BaseType& source, const BaseType& dest){
    if (source.type_name == dest.type_name && source.instantiation_generics.size() == dest.instantiation_generics.size()){
        for (int i = 0; i < source.instantiation_generics.size(); i++){
            if (dest.instantiation_generics[i].is<TemplateType>()){
                if (!validate_assignment(source.instantiation_generics[i], dest.instantiation_generics[i])){
                    return false;
                }
            }
            else if (dest.instantiation_generics[i].to_string() != source.instantiation_generics[i].to_string()){
                return false;
            }
        }
        return true;
    }
    else {
        return validate_complex_assignment(source, dest);
    }
}

bool AssignmentTypeChecker::validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest){
    if (source.is<PrimitiveType>()){
        return source.get<PrimitiveType>().type_name == "String" || 
            source.get<PrimitiveType>().type_name == "RawString";
    }
    else if (source.is<SliceType>()){
        return source.get<SliceType>().stored_type.is<PrimitiveType>() && 
            source.get<SliceType>().stored_type.get<PrimitiveType>().type_name == "Char";
    }
    else if (source.is<ArrayType>()){
        return source.get<ArrayType>().stored_type.is<PrimitiveType>() &&
            source.get<ArrayType>().stored_type.get<PrimitiveType>().type_name == "Char";
    }
    else {
        return false;
    }
}