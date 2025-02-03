
#include "language/typesignatures.hpp"

inline bool is_primitive_type(const TypeSignature& ts, const std::string& type_name) {
    return 
        ts.is<PrimitiveType>() && 
        ts.get<PrimitiveType>().type_name == type_name;
}

inline bool is_list_of_strings(const TypeSignature& ts) {
    return 
        ts.is<CustomType>() && ts.get<CustomType>().type_name == "List" && 
        ts.get<CustomType>().type_parameters[0].is<PrimitiveType>() &&
        ts.get<CustomType>().type_parameters[0].get<PrimitiveType>().type_name == string_type;
}

inline bool is_pointer_to_primitive_type(const TypeSignature& ts, const std::string& type_name) {
    return 
        ts.is<PointerType>() && ts.get<PointerType>().pointed_type.is<PrimitiveType>() && 
        ts.get<PointerType>().pointed_type.get<PrimitiveType>().type_name == type_name;
}

inline bool is_array_of_primitive_type_of_size_n(const TypeSignature& ts, const std::string& type_name, size_t n) {
    return 
        ts.is<ArrayType>() && ts.get<ArrayType>().array_length == n && 
        ts.get<ArrayType>().stored_type.is<PrimitiveType>() &&
        ts.get<ArrayType>().stored_type.get<PrimitiveType>().type_name == type_name;
}

inline bool is_slice_of_primitive_type(const TypeSignature& ts, const std::string& type_name) {
    return 
        ts.is<SliceType>() && ts.get<SliceType>().stored_type.is<PrimitiveType>() && 
        ts.get<SliceType>().stored_type.get<PrimitiveType>().type_name == type_name;
}

inline bool is_pointer_to_slice_of_primitive_type(const TypeSignature& ts, const std::string& type_name) {
    return 
        ts.is<PointerType>() && ts.get<PointerType>().pointed_type.is<SliceType>() && 
        ts.get<PointerType>().pointed_type.get<SliceType>().stored_type.is<PrimitiveType>() && 
        ts.get<PointerType>().pointed_type.get<SliceType>().stored_type.get<PrimitiveType>().type_name == type_name;
}

inline bool is_pointer_to_pointer_to_array_of_primitive_type_of_size_n(const TypeSignature& ts, const std::string& type_name, int n) {
    return 
        ts.is<PointerType>() && ts.get<PointerType>().pointed_type.is<PointerType>() && 
        ts.get<PointerType>().pointed_type.get<PointerType>().pointed_type.is<ArrayType>() && 
        is_array_of_primitive_type_of_size_n(ts.get<PointerType>().pointed_type.get<PointerType>().pointed_type, type_name, n);
}

inline bool is_pointer_to_pointer_to_slice_of_primtive_type(const TypeSignature& ts, const std::string& type_name) {
    return 
        ts.is<PointerType>() && ts.get<PointerType>().pointed_type.is<PointerType>() && 
        ts.get<PointerType>().pointed_type.get<PointerType>().pointed_type.is<SliceType>() && 
        is_slice_of_primitive_type(ts.get<PointerType>().pointed_type.get<PointerType>().pointed_type, int_type);
}

inline bool is_int(const TypeSignature& ts) {
    return is_primitive_type(ts, int_type);
}

inline bool is_float(const TypeSignature& ts) {
    return is_primitive_type(ts, float_type);
}

inline bool is_char(const TypeSignature& ts) {
    return is_primitive_type(ts, char_type);
}

inline bool is_string(const TypeSignature& ts) {
    return is_primitive_type(ts, string_type);
}

inline bool is_pointer_to_float(const TypeSignature& ts) {
    return is_pointer_to_primitive_type(ts, float_type);
}

inline bool is_pointer_to_int(const TypeSignature& ts) {
    return is_pointer_to_primitive_type(ts, int_type);
}

inline bool is_slice_of_floats(const TypeSignature& ts) {
    return is_slice_of_primitive_type(ts, float_type);
}

inline bool is_array_of_ints_of_size_n(const TypeSignature& ts, int n) {
    return is_array_of_primitive_type_of_size_n(ts, int_type, n);
}

inline bool is_pointer_to_slice_of_floats(const TypeSignature& ts) {
    return is_pointer_to_slice_of_primitive_type(ts, float_type);
}

inline bool is_pointer_to_slice_of_ints(const TypeSignature& ts) {
    return is_pointer_to_slice_of_primitive_type(ts, int_type);
}

inline bool is_array_of_strings_of_size_n(const TypeSignature& ts, int n) {
    return is_array_of_primitive_type_of_size_n(ts, string_type, n);
}

inline bool is_pointer_to_pointer_to_array_of_ints_of_size_n(const TypeSignature& ts, int n) {
    return is_pointer_to_pointer_to_array_of_primitive_type_of_size_n(ts, int_type, n);
}

inline bool is_pointer_to_pointer_to_slice_of_ints(const TypeSignature& ts) {
    return is_pointer_to_pointer_to_slice_of_primtive_type(ts, int_type);
}