
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
    if (source.is<CustomType>()){
        TypeDefinition source_type_definition = program_representation.retrieve_type_definition(source.get<CustomType>());
        if (source_type_definition.is<TypeAlias>()){
            if (validate_assignment(source_type_definition.get<TypeAlias>().aliased_type, dest)){
                return true;
            }
        }
    }
    if (dest.is<CustomType>()){
        TypeDefinition dest_type_definition = program_representation.retrieve_type_definition(dest.get<CustomType>());
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
    else if (dest.is<CustomType>()){
        return validate_assignment_to_custom_type(source, dest.get<CustomType>());
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

bool AssignmentTypeChecker::validate_assignment_to_custom_type(const TypeSignature& source, const CustomType& dest){
    return (source.is<CustomType>())
        ? validate_assignment_between_custom_types(source.get<CustomType>(), dest)
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

bool AssignmentTypeChecker::validate_complex_assignment(const TypeSignature& source, const CustomType& dest){
    const TypeDefinition& dest_type_definition = program_representation.retrieve_type_definition(dest);
    if (dest_type_definition.is<UnionDefinition>()){    
        if (validate_assignment_to_union(source, dest_type_definition.get<UnionDefinition>())){
            return true;
        }
    }
    if (!source.is<CustomType>()){
        return false;
    }
    const CustomType& source_custom_type = source.get<CustomType>();
    const TypeDefinition& source_type_definition = program_representation.retrieve_type_definition(source_custom_type);
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

bool AssignmentTypeChecker::validate_assignment_between_custom_types(const CustomType& source, const CustomType& dest){
    if (source.type_name != dest.type_name || source.instantiation_generics.size() != dest.instantiation_generics.size()){
        return validate_complex_assignment(source, dest);
    }
    for (int i = 0; i < source.instantiation_generics.size(); i++){
        if (dest.instantiation_generics[i].is<TemplateType>()){
            if (!validate_assignment(source.instantiation_generics[i], dest.instantiation_generics[i])){
                return false;
            }
        }
        else if (!typesignatures_are_equal(source.instantiation_generics[i], dest.instantiation_generics[i])){
            return false;
        }
    }
    return true;
}

bool AssignmentTypeChecker::validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest){
    if (source.is<PrimitiveType>()){
        if (source.get<PrimitiveType>().type_name == "RawString")
            return dest.type_name == "RawString";
        else
            return (source.get<PrimitiveType>().type_name == "String");
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

bool AssignmentTypeChecker::typesignatures_are_equal(const TypeSignature& t1, const TypeSignature& t2){
    if (type_alias_unaware_typesignatures_are_equal(t1, t2)){
        return true;
    }
    if (t1.is<CustomType>()){
        TypeDefinition t1_type_definition = program_representation.retrieve_type_definition(t1.get<CustomType>());
        if (t1_type_definition.is<TypeAlias>()){
            return typesignatures_are_equal(t1_type_definition.get<TypeAlias>().aliased_type, t2);
        }
    }
    if (t2.is<CustomType>()){
        TypeDefinition t2_type_definition = program_representation.retrieve_type_definition(t2.get<CustomType>());
        if (t2_type_definition.is<TypeAlias>()){
            return typesignatures_are_equal(t1, t2_type_definition.get<TypeAlias>().aliased_type);
        }
    }
    return false;
}

bool AssignmentTypeChecker::type_alias_unaware_typesignatures_are_equal(const TypeSignature& t1, const TypeSignature& t2){
    if (t1.is<PrimitiveType>() && t2.is<PrimitiveType>()){
        return t1.get<PrimitiveType>().type_name == t2.get<PrimitiveType>().type_name;
    }
    if (t1.is<PointerType>() && t2.is<PointerType>()){
        return typesignatures_are_equal(t1.get<PointerType>().pointed_type, t2.get<PointerType>().pointed_type);
    }
    if (t1.is<ArrayType>() && t2.is<ArrayType>()){
        return t1.get<ArrayType>().array_length == t2.get<ArrayType>().array_length && 
            typesignatures_are_equal(t1.get<ArrayType>().stored_type, t2.get<ArrayType>().stored_type);
    }
    if (t1.is<SliceType>() && t2.is<SliceType>()){
        return typesignatures_are_equal(t1.get<SliceType>().stored_type, t2.get<SliceType>().stored_type);
    }
    if (t1.is<CustomType>() && t2.is<CustomType>()){
        const std::string t1_fully_qualified_name = program_representation.get_fully_quilified_typesignature_name(t1.get<CustomType>());
        const std::string t2_fully_qualified_name = program_representation.get_fully_quilified_typesignature_name(t2.get<CustomType>());
        return t1_fully_qualified_name == t2_fully_qualified_name;
    }
    return false;
}