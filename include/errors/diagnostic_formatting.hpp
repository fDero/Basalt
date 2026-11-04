//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "language/typesignatures.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "syntax/primitive_types.hpp"


inline std::string backticked(const std::string& name) {
    return "`" + name + "`";
}

inline std::string describe_type(const std::optional<TypeSignature>& type) {
    return type.has_value()
        ? backticked(type->to_source_string())
        : "an unknown type";
}

inline std::string describe_argument_types(const std::vector<TypeSignature>& arg_types) {
    if (arg_types.empty()) {
        return "no arguments";
    }
    std::string result = "(";
    for (size_t i = 0; i < arg_types.size(); i++) {
        result += (i == 0 ? "" : ", ") + backticked(arg_types[i].to_source_string());
    }
    return result + ")";
}

inline std::string describe_location(const DebugInformationsAwareEntity& location) {
    return location.filename
        + ":" + std::to_string(location.line_number)
        + ":" + std::to_string(location.char_pos + 1);
}

inline std::string describe_function_name(const FunctionCall& function_call) {
    return function_call.package_prefix.empty()
        ? backticked(function_call.function_name)
        : backticked(function_call.package_prefix + "::" + function_call.function_name);
}

inline std::string describe_expression(const Expression& expression) {
    return expression.is<Identifier>()
        ? backticked(expression.get<Identifier>().name)
        : "this expression";
}

inline bool is_primitive(const TypeSignature& type, const std::string& type_name) {
    return type.is<PrimitiveType>() && type.get<PrimitiveType>().type_name == type_name;
}

inline bool is_numeric(const TypeSignature& type) {
    return is_primitive(type, int_type) || is_primitive(type, float_type);
}
