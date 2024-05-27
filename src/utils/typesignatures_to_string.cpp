
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

[[nodiscard]] std::string TypeSignature::to_string() const {
    return ptr->to_string();
}

[[nodiscard]] std::string CustomType::to_string() const {
    std::string fully_qualified_name = type_name;
    if (instantiation_generics.empty()) return fully_qualified_name;
    std::string serialized_custom_type = fully_qualified_name + "<";
    for (const TypeSignature& generic : instantiation_generics){
        serialized_custom_type += generic.to_string() + ",";
    }
    serialized_custom_type.back() = '>';
    return serialized_custom_type;
}

[[nodiscard]] std::string PointerType::to_string() const {
    return pointer_type_symbol + pointed_type.to_string();
}

[[nodiscard]] std::string ArrayType::to_string() const {
    return "[" + std::to_string(array_length) + "]" + stored_type.to_string();
}

[[nodiscard]] std::string SliceType::to_string() const {
    return slice_type_symbol + stored_type.to_string();
}

[[nodiscard]] std::string TemplateType::to_string() const {
    return type_name;
}

[[nodiscard]] std::string PrimitiveType::to_string() const {
    return type_name;
}