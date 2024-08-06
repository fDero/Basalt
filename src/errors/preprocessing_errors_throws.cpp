
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "toolchain/representation.hpp"
#include <unordered_set>

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
) {
    throw std::runtime_error {
        "No type definition found for type " + type_signature.type_name + "\n"
    };
}

[[noreturn]] void throw_cannot_access_square_brackets_on_type(
    const TypeSignature& left_operand_type,
    const BinaryOperator& expression
) {
    throw InternalError("cannot access square brackets on type");
}

[[noreturn]] void throw_no_such_struct_field(
    const std::string& member_name,
    const StructDefinition& struct_type_definition,
    const BinaryOperator& expression
) {
    throw InternalError("no such struct field");
}
