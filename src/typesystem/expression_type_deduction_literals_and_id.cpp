
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include "errors/internal_errors.hpp"

[[nodiscard]] TypeSignature ArrayLiteral::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& data
) const {
    return ArrayType{ array_length, stored_type };
}

[[nodiscard]] TypeSignature StringLiteral::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& data
) const {
    return PrimitiveType{ "String" };
}

[[nodiscard]] TypeSignature IntLiteral::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& data
) const {
    return PrimitiveType{ "Int" };
}

[[nodiscard]] TypeSignature FloatLiteral::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& data
) const {
    return PrimitiveType{ "Float" };
}

[[nodiscard]] TypeSignature BoolLiteral::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& data
) const {
    return PrimitiveType{ "Bool" };
}

[[nodiscard]] TypeSignature CharLiteral::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& data
) const {
    return PrimitiveType{ "Char" };
}

[[nodiscard]] TypeSignature Identifier::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& scoped_data
) const {
    const TypeSignature& type_of_scoped_object = scoped_data.get_object_type_by_name(name);
    return type_of_scoped_object;
}