
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

[[nodiscard]] std::string CustomType::function_retrieval_match_string(
    const std::vector<std::string>& template_generics_names
) const {
    std::string transformed_typename = type_name;
    const auto begin = template_generics_names.begin();
    const auto end =  template_generics_names.end();
    if (std::find(begin, end, type_name) != end){
        transformed_typename = "[^ ,\t]+";
    }
    if (instanciated_generics.empty()) return transformed_typename;
    std::string serialized_base_type = transformed_typename + "<";
    for (const TypeSignature& generic : instanciated_generics){
        serialized_base_type += generic.function_retrieval_match_string(template_generics_names);
        serialized_base_type +=  + ",";
    }
    serialized_base_type.back() = '>';
    return serialized_base_type;
}

[[nodiscard]] std::string PointerType::function_retrieval_match_string(
    const std::vector<std::string>& template_generics_names) const {
        return "#" + pointed_type.function_retrieval_match_string(template_generics_names);
}

[[nodiscard]] std::string ArrayType::function_retrieval_match_string(
    const std::vector<std::string>& template_generics_names) const {
        return "Array<" + std::to_string(array_length) + "," +
            stored_type.function_retrieval_match_string(template_generics_names) + ">";
}

[[nodiscard]] std::string SliceType::function_retrieval_match_string(
    const std::vector<std::string>& template_generics_names) const {
        return "Slice<" + stored_type.function_retrieval_match_string(template_generics_names) + ">";
}

[[nodiscard]] std::string PrimitiveType::function_retrieval_match_string(
    const std::vector<std::string>& template_generics_names
) const {
    return type_name;
}