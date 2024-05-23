#include "toolchain/representation.hpp"
#include "toolchain/preprocessor.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

AssignmentTypeChecker::AssignmentTypeChecker(ProgramRepresentation& program_representation) 
    : program_representation(program_representation) {}

GenericSubstitutionRuleSet& AssignmentTypeChecker::get_generic_substitution_rules() 
    { return generic_substitution_rules; }

bool AssignmentTypeChecker::validate_assignment(const TypeSignature& source, const TypeSignature& dest){
    if (dest.is<TemplateType>()){
        for (GenericSubstitutionRule& rule : generic_substitution_rules){
            if (rule.to_be_substituded == dest.get<TemplateType>().type_name){
                if (validate_assignment(source, rule.replacement)){
                    return true;
                }
                else if (validate_assignment(rule.replacement, source)){
                    rule.replacement = source;
                    return true;
                }
                else  {
                    return false;
                }
            }
        }
        generic_substitution_rules.push_back({dest.get<TemplateType>().type_name, source});
        return true;
    }
    else if (dest.is<BaseType>()){
        if (source.is<BaseType>()) {
            return validate_assignment_between_base_types(source.get<BaseType>(), dest.get<BaseType>());
        }
        else {
            return validate_assignment_to_union(source, dest.get<BaseType>());
        }
    }
    else if (dest.is<PrimitiveType>()){
        if (dest.get<PrimitiveType>().type_name == "String" || dest.get<PrimitiveType>().type_name == "RawString"){
            return validate_assignment_to_string(source, dest.get<PrimitiveType>());
        }
        else if (source.is<PrimitiveType>()){
            return validate_assignment_between_primitive_types(source.get<PrimitiveType>(), dest.get<PrimitiveType>());
        }
        else {
            return false;
        }
    }
    else if (dest.is<ArrayType>()){
        if (source.is<ArrayType>()){
            return validate_assignment_between_array_types(source.get<ArrayType>(), dest.get<ArrayType>());
        }
        else {
            return false;
        }
    }
    else if (dest.is<PointerType>()){
        if (source.is<PointerType>()){
            return validate_assignment_between_pointer_types(source.get<PointerType>(), dest.get<PointerType>());
        }
        else {
            return false;
        }
    }
    else if (dest.is<SliceType>()){
        if (source.is<ArrayType>()){
            return validate_assignment_array_to_slice(source.get<ArrayType>(), dest.get<SliceType>());
        }
        else if (source.is<SliceType>()){
            return validate_assignment_between_slices(source.get<SliceType>(), dest.get<SliceType>());
        }
    }
    assert_unreachable();
}

bool AssignmentTypeChecker::validate_assignment_to_union(const TypeSignature& source, const BaseType& dest){
    const TypeDefinition& dest_type_definition = program_representation.retrieve_type_definition(dest);
    if (!dest_type_definition.is<UnionDefinition>()){
        return false;
    }
    for (const TypeSignature& alternative : dest_type_definition.get<UnionDefinition>().types){
        if (validate_assignment(source, alternative)){
            return true;
        }
    }
    if (!source.is<BaseType>()){
        return false;
    }
    const BaseType& source_base_type = source.get<BaseType>();
    const TypeDefinition& source_type_definition = program_representation.retrieve_type_definition(source_base_type);
    if (!source_type_definition.is<UnionDefinition>()){
        return false;
    }
    for (const TypeSignature& source_alternative : source_type_definition.get<UnionDefinition>().types){
        bool found = false;
        for (const TypeSignature& dest_alternative : dest_type_definition.get<UnionDefinition>().types){
            if (validate_assignment(source_alternative, dest_alternative)){
                found = true;
                break;
            }
        }
        if (!found){
            return false;
        }
    }
    return true;
}

bool AssignmentTypeChecker::validate_assignment_between_base_types(const BaseType& source, const BaseType& dest){
    if (source.type_name == dest.type_name && source.instantiationd_generics.size() == dest.instantiationd_generics.size()){
        for (int i = 0; i < source.instantiationd_generics.size(); i++){
            if (!validate_assignment(source.instantiationd_generics[i], dest.instantiationd_generics[i])){
                return false;
            }
        }
        return true;
    }
    else {
        return validate_assignment_to_union(source, dest);
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

bool AssignmentTypeChecker::validate_assignment_between_primitive_types(const PrimitiveType& source, const PrimitiveType& dest){
    return source.type_name == dest.type_name;
}

bool AssignmentTypeChecker::validate_assignment_between_array_types(const ArrayType& source, const ArrayType& dest){
    return validate_assignment(source.stored_type, dest.stored_type);
}

bool AssignmentTypeChecker::validate_assignment_between_pointer_types(const PointerType& source, const PointerType& dest){
    return validate_assignment(source.pointed_type, dest.pointed_type);
}

bool AssignmentTypeChecker::validate_assignment_array_to_slice(const ArrayType& source, const SliceType& dest){
    return validate_assignment(source.stored_type, dest.stored_type);
}

bool AssignmentTypeChecker::validate_assignment_between_slices(const SliceType& source, const SliceType& dest){
    return validate_assignment(source.stored_type, dest.stored_type);
}