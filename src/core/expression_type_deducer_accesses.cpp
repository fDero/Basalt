//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/expression_type_deducer.hpp"
#include "core/assignment_type_checker.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_square_brackets_access(const Expression& square_brackets_access_expr) {
    assert_expression_is<SquareBracketsAccess>(square_brackets_access_expr);
    const SquareBracketsAccess& square_brackets_access = square_brackets_access_expr.get<SquareBracketsAccess>();
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(square_brackets_access.storage);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(square_brackets_access.index);
    ensure_typesignature_is_int(right_operand_type);
    if (!left_operand_type.has_value()) {
        return std::nullopt;
    }
    switch(left_operand_type.value().typesiganture_kind()) {
        case TypeSignatureBody::Kind::array_type: return left_operand_type.value().get<ArrayType>().stored_type;
        case TypeSignatureBody::Kind::slice_type: return left_operand_type.value().get<SliceType>().stored_type;
        case TypeSignatureBody::Kind::primitive_type: {
            std::string primitive_type_name = left_operand_type.value().get<PrimitiveType>().type_name;
            if (primitive_type_name == string_type || primitive_type_name == raw_string_type) {
                return deduce_primtive_type(char_type, square_brackets_access);
            }
        }
        default: throw_cannot_access_square_brackets_on_type(left_operand_type.value(), square_brackets_access);
    }
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access(const Expression& dot_member_access_expr) {
    assert_expression_is<DotMemberAccess>(dot_member_access_expr);
    const DotMemberAccess& dot_member_access = dot_member_access_expr.get<DotMemberAccess>();
    std::optional<TypeSignature> left_operand_type_opt = deduce_expression_type(dot_member_access.struct_value);
    if (!left_operand_type_opt.has_value()) {
        return std::nullopt;
    }
    TypeSignature left_operand_type = type_definitions_register.unalias_type(left_operand_type_opt.value());
    return deduce_type_from_dot_member_access_on_type(dot_member_access, left_operand_type);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access_on_type(
    const DotMemberAccess& dot_member_access,
    const TypeSignature& left_operand_type
) {
    switch (left_operand_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::custom_type:      return deduce_type_from_dot_member_access_on_custom_type(dot_member_access, left_operand_type.get<CustomType>());
        case TypeSignatureBody::Kind::pointer_type:     return deduce_type_from_dot_member_access_on_pointer_type(dot_member_access, left_operand_type.get<PointerType>());
        case TypeSignatureBody::Kind::primitive_type:   return deduce_type_from_dot_member_access_on_primitive_type(dot_member_access, left_operand_type.get<PrimitiveType>());
        case TypeSignatureBody::Kind::slice_type:       return deduce_type_from_dot_member_access_on_slice_type(dot_member_access, left_operand_type.get<SliceType>());
        default: throw_bad_dot_member_access_on_type(left_operand_type, dot_member_access); 
    }
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access_on_pointer_type(
    const DotMemberAccess& dot_member_access,
    const PointerType& left_operand_type
) {
    TypeSignature member_type = left_operand_type;
    while (member_type.is<PointerType>()) {
        member_type = member_type.get<PointerType>().pointed_type;
        member_type = type_definitions_register.unalias_type(member_type);
    }
    return deduce_type_from_dot_member_access_on_type(dot_member_access, member_type);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access_on_custom_type(
    const DotMemberAccess& dot_member_access,
    const CustomType& custom_type
) {
    TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(custom_type);
    ensure_type_definition_is<StructDefinition>(type_definition);
    const StructDefinition& struct_type_definition = type_definition.get<StructDefinition>();
    const std::string& member_name = dot_member_access.member_name;
    for (const auto& member : struct_type_definition.fields) {
        if (member.field_name == member_name) {
            return member.field_type;
        }
    }
    throw_no_such_struct_field(member_name, struct_type_definition, dot_member_access);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access_on_primitive_type(
    const DotMemberAccess& dot_member_access,
    const PrimitiveType& primitive_type
) {
    if (dot_member_access.member_name == "len" && primitive_type.type_name == string_type) {
        return deduce_primtive_type(int_type, dot_member_access);
    }
    throw_no_such_primitive_field(dot_member_access.member_name, primitive_type, dot_member_access);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_dot_member_access_on_slice_type(
    const DotMemberAccess& dot_member_access,
    const SliceType& slice_type
) {
    if (dot_member_access.member_name == "len") {
        return deduce_primtive_type(int_type, dot_member_access);
    }
    throw_no_such_slice_field(dot_member_access.member_name, slice_type, dot_member_access);
}