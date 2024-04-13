
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

[[nodiscard]] std::string TypeSignature::to_string() const {
    return ptr->to_string();
}

[[nodiscard]] std::string TypeSignature::function_retrieval_match_string(
    const std::vector<std::string>& generics_names
) const { 
    return ptr->function_retrieval_match_string(template_generics_names); 
}

[[nodiscard]] std::string TypeSignature::struct_retrieval_match_string() const { 
    return ptr->struct_retrieval_match_string(); 
}

[[nodiscard]] bool TypeSignature::is_primitive_type() const { 
    return ptr->is_primitive_type(); 
}

[[nodiscard]] bool TypeSignature::is_generic(const std::vector<std::string>& generic_names) const { 
    return ptr->is_generic(template_generic_names); 
}

BaseType::BaseType(
    const std::string& name, 
    const std::vector<TypeSignature>& generics
) : type_name(name), instanciated_generics(generics) {}

PointerType::PointerType(
    const TypeSignature& pointed
) : pointed_type(pointed) {}

ArrayType::ArrayType(
    int length, 
    const TypeSignature& stored
) : array_length(length), stored_type(stored) {}


SliceType::SliceType(
    const TypeSignature& stored
) : stored_type(stored) {}