//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/assignment_type_checker.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

bool AssignmentTypeChecker::validate_assignment_to_primitive_type(
    const TypeSignature& source, 
    const PrimitiveType& dest,
    bool strict_mode
) {
    return (dest.type_name == string_type || dest.type_name == raw_string_type)
        ? validate_assignment_to_string(source, dest, strict_mode)
        : source.is<PrimitiveType>() && source.get<PrimitiveType>().type_name == dest.type_name;
}

bool AssignmentTypeChecker::validate_assignment_to_slice_type(
    const TypeSignature& source, 
    const SliceType& dest,
    bool strict_mode
) {
    switch (source.typesiganture_kind()) {
        case TypeSignatureBody::Kind::slice_type: return validate_assignment(
            source.get<SliceType>().stored_type, dest.stored_type, true
        );
        case TypeSignatureBody::Kind::pointer_type: return validate_assignment_to_slice_from_pointer_type(
            source.get<PointerType>(), dest, strict_mode
        );
        default: return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_string(
    const TypeSignature& source, 
    const PrimitiveType& dest,
    bool strict_mode
) {
    switch (source.typesiganture_kind()) {
        case TypeSignatureBody::Kind::primitive_type: 
            return validate_assignment_to_string_from_primitive_type(source.get<PrimitiveType>(), dest, strict_mode);
        case TypeSignatureBody::Kind::slice_type:     
            return validate_assignment_to_string_from_slice_type(source.get<SliceType>(), dest, strict_mode);
        case TypeSignatureBody::Kind::pointer_type:   
            return validate_assignment_to_string_from_pointer_type(source.get<PointerType>(), dest, strict_mode);
        default: return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_string_from_primitive_type(
    const PrimitiveType& source, 
    const PrimitiveType& dest,
    bool strict_mode
) {
    return source.type_name == string_type || dest.type_name == raw_string_type;
}

bool AssignmentTypeChecker::validate_assignment_to_string_from_slice_type(
    const SliceType& source, 
    const PrimitiveType& dest,
    bool strict_mode
) {
    return source.stored_type.is<PrimitiveType>() && 
        source.stored_type.get<PrimitiveType>().type_name == char_type;
}

bool AssignmentTypeChecker::validate_assignment_to_string_from_pointer_type(
    const PointerType& pointer_type, 
    const PrimitiveType& dest,
    bool strict_mode
) {
    if (!pointer_type.pointed_type.is<ArrayType>()) {
        return false;
    }
    const ArrayType& array_type = pointer_type.pointed_type.get<ArrayType>();
    return array_type.stored_type.is<PrimitiveType>() && 
        array_type.stored_type.get<PrimitiveType>().type_name == char_type;
}

bool AssignmentTypeChecker::validate_assignment_to_slice_from_pointer_type(
    const PointerType& pointer_type, 
    const SliceType& dest,
    bool strict_mode
) {
    if (!pointer_type.pointed_type.is<ArrayType>()) {
        return false;
    }
    const ArrayType& array_type = pointer_type.pointed_type.get<ArrayType>();
    return validate_assignment(array_type.stored_type, dest.stored_type, true);
}

bool AssignmentTypeChecker::validate_assignment_to_array_type(
    const TypeSignature& source, 
    const ArrayType& dest,
    bool strict_mode
) {
    bool assignment_makes_sense = (source.is<ArrayType>());
    assignment_makes_sense &= validate_assignment(source.get<ArrayType>().stored_type, dest.stored_type);
    assignment_makes_sense &= source.get<ArrayType>().array_length == dest.array_length;
    return assignment_makes_sense;
}

bool AssignmentTypeChecker::validate_assignment_to_pointer_type(
    const TypeSignature& source, 
    const PointerType& dest,
    bool strict_mode
) {
    bool assignment_makes_sense = (source.is<PointerType>());
    return assignment_makes_sense && validate_assignment(source.get<PointerType>().pointed_type, dest.pointed_type, true);
}