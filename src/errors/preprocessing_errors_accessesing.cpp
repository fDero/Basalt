//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/typesignatures.hpp"
#include "syntax/primitive_types.hpp"


void ensure_typesignature_is_int(
    const std::optional<TypeSignature>& index_type,
    const Expression& index
) {
    if (index_type.has_value() && !is_primitive(*index_type, int_type)) {
        throw ValidationError(
            "index must be of type `Int`, but " + describe_expression(index) + " has type " +
            describe_type(index_type), index.as_debug_informations_aware_entity()
        );
    }
}

void ensure_typesignature_is_compatible_square_brackets_access(
    const TypeSignature& storage_type,
    const SquareBracketsAccess& square_brackets_access
) {
    bool is_sequence = storage_type.is<ArrayType>() || storage_type.is<SliceType>();
    bool is_string = is_primitive(storage_type, string_type) || is_primitive(storage_type, raw_string_type);
    if (!is_sequence && !is_string) {
        throw_cannot_access_square_brackets_on_type(storage_type, square_brackets_access);
    }
}

void throw_cannot_access_square_brackets_on_type(
    const TypeSignature& storage_type,
    const SquareBracketsAccess& square_brackets_access
) {
    throw ValidationError(
        "cannot index into " + describe_expression(square_brackets_access.storage) +
        ", which has type " + describe_type(storage_type),
        square_brackets_access.as_debug_informations_aware_entity(),
        "only arrays, slices, `String` and `RawString` can be indexed with `[]`"
    );
}

void ensure_type_definition_is_struct_for_member_access(
    const TypeDefinition& type_definition,
    const DotMemberAccess& dot_member_access
) {
    if (type_definition.is<StructDefinition>()) {
        return;
    }
    std::string kind = type_definition.is<UnionDefinition>() ? "a union" : "a type alias";
    throw ValidationError(
        "cannot access field " + backticked(dot_member_access.member_name) + " on type " +
        backticked(type_definition.get_simple_name()) + ": it is " + kind + ", not a struct",
        dot_member_access.as_debug_informations_aware_entity()
    );
}

void throw_no_such_struct_field(
    const std::string& member_name,
    const StructDefinition& struct_definition,
    const DotMemberAccess& dot_member_access
) {
    std::string available_fields;
    for (const StructDefinition::Field& field : struct_definition.fields) {
        available_fields += (available_fields.empty() ? "" : ", ") + backticked(field.field_name);
    }
    throw ValidationError(
        "struct " + backticked(struct_definition.def_name) +
        " has no field named " + backticked(member_name),
        dot_member_access.as_debug_informations_aware_entity(),
        available_fields.empty()
            ? "struct " + backticked(struct_definition.def_name) + " has no fields"
            : "available fields: " + available_fields
    );
}

void throw_bad_dot_member_access_on_type(
    const TypeSignature& left_operand_type,
    const DotMemberAccess& dot_member_access
) {
    throw ValidationError(
        "cannot access field " + backticked(dot_member_access.member_name) + " on " +
        describe_expression(dot_member_access.struct_value) + ": values of type " +
        describe_type(left_operand_type) + " have no fields",
        dot_member_access.as_debug_informations_aware_entity()
    );
}

void throw_no_such_primitive_field(
    const std::string& member_name,
    const PrimitiveType& primitive_type,
    const DotMemberAccess& dot_member_access
) {
    bool is_string = primitive_type.type_name == string_type;
    throw ValidationError(
        "type " + backticked(primitive_type.type_name) + " has no field named " + backticked(member_name),
        dot_member_access.as_debug_informations_aware_entity(),
        is_string ? "the only field of `String` is `len`" : ""
    );
}

void throw_no_such_slice_field(
    const std::string& member_name,
    const SliceType& slice_type,
    const DotMemberAccess& dot_member_access
) {
    throw ValidationError(
        "slice type " + backticked(TypeSignature(slice_type).to_source_string()) +
        " has no field named " + backticked(member_name),
        dot_member_access.as_debug_informations_aware_entity(),
        "the only field of a slice is `len`"
    );
}
