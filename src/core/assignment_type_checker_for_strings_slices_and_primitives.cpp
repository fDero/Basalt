//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/assignment_type_checker.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

bool AssignmentTypeChecker::validate_assignment_to_primitive_type(const TypeSignature& source, const PrimitiveType& dest) {
    return (dest.type_name == "String" || dest.type_name == "RawString")
        ? validate_assignment_to_string(source, dest)
        : source.is<PrimitiveType>() && source.get<PrimitiveType>().type_name == dest.type_name;
}

bool AssignmentTypeChecker::validate_assignment_to_slice_type(const TypeSignature& source, const SliceType& dest) {
        switch (source.typesiganture_kind()) {
        case TypeSignatureBody::Kind::slice_type: return validate_assignment_very_strictly(
            source.get<SliceType>().stored_type, dest.stored_type
        );
        case TypeSignatureBody::Kind::pointer_type: return validate_assignment_to_slice_from_pointer_type(
            source.get<PointerType>(), dest
        );
        default: return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest) {
    switch (source.typesiganture_kind()) {
        case TypeSignatureBody::Kind::primitive_type: return validate_assignment_to_string_from_primitive_type(source.get<PrimitiveType>(), dest);
        case TypeSignatureBody::Kind::slice_type:     return validate_assignment_to_string_from_slice_type(source.get<SliceType>(), dest);
        case TypeSignatureBody::Kind::pointer_type:   return validate_assignment_to_string_from_pointer_type(source.get<PointerType>(), dest);
        default: return false;
    }
}

bool AssignmentTypeChecker::validate_assignment_to_string_from_primitive_type(const PrimitiveType& source, const PrimitiveType& dest) {
    return source.type_name == "String" || dest.type_name == "RawString";
}

bool AssignmentTypeChecker::validate_assignment_to_string_from_slice_type(const SliceType& source, const PrimitiveType& dest) {
    return source.stored_type.is<PrimitiveType>() && 
        source.stored_type.get<PrimitiveType>().type_name == "Char";
}

bool AssignmentTypeChecker::validate_assignment_to_string_from_pointer_type(const PointerType& pointer_type, const PrimitiveType& dest) {
    if (!pointer_type.pointed_type.is<ArrayType>()) {
        return false;
    }
    const ArrayType& array_type = pointer_type.pointed_type.get<ArrayType>();
    return array_type.stored_type.is<PrimitiveType>() && 
        array_type.stored_type.get<PrimitiveType>().type_name == "Char";
}

bool AssignmentTypeChecker::validate_assignment_to_slice_from_pointer_type(const PointerType& pointer_type, const SliceType& dest) {
    ensure_typesignature_is<ArrayType>(pointer_type.pointed_type);
    const ArrayType& array_type = pointer_type.pointed_type.get<ArrayType>();
    return validate_assignment_very_strictly(array_type.stored_type, dest.stored_type);
}