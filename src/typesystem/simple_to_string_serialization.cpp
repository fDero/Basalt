
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

[[nodiscard]] std::string CustomType::to_string() const {
    std::string fully_qualified_name = type_name;
    if (instanciated_generics.empty()) return fully_qualified_name;
    std::string serialized_base_type = fully_qualified_name + "<";
    for (const TypeSignature& generic : instanciated_generics){
        serialized_base_type += generic.to_string() + ",";
    }
    serialized_base_type.back() = '>';
    return serialized_base_type;
}

[[nodiscard]] std::string PointerType::to_string() const {
    return "#" + pointed_type.to_string();
}

[[nodiscard]] std::string ArrayType::to_string() const {
    return "Array<" + std::to_string(array_length) + 
        "," + stored_type.to_string() + ">";
}

[[nodiscard]] std::string SliceType::to_string() const {
    return "Slice<" + stored_type.to_string() + ">";
}

[[nodiscard]] std::string PrimitiveType::to_string() const {
    return type_name;
}