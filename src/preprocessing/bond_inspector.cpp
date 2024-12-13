//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/bond_inspector.hpp"
#include "errors/internal_errors.hpp"

BondInspector::BondInspector(
    ScopeContext& scope_context, 
    ProgramRepresentation& program_representation
)
    : scope_context(scope_context)
    , program_representation(program_representation) 
{}

bool BondInspector::does_the_type_of_this_expr_imply_a_bond(const TypeSignature& expression_type) {
    switch (expression_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::inline_union: return does_this_inline_union_imply_a_bond(expression_type.get<InlineUnion>());
        case TypeSignatureBody::Kind::custom_type: return does_this_custom_type_imply_a_bond(expression_type.get<CustomType>());
        case TypeSignatureBody::Kind::array_type: return false;
        case TypeSignatureBody::Kind::primitive_type: return false;
        case TypeSignatureBody::Kind::pointer_type: return true;
        case TypeSignatureBody::Kind::slice_type: return true;
        case TypeSignatureBody::Kind::template_type: return true;
    }
    assert_unreachable();
}

bool BondInspector::does_this_inline_union_imply_a_bond(const InlineUnion& inline_union) {
    for (const TypeSignature& alternative : inline_union.alternatives) {
        if (does_the_type_of_this_expr_imply_a_bond(alternative)) {
            return true;
        }
    }
    return false;
}

bool BondInspector::does_this_custom_type_imply_a_bond(const CustomType& custom_type) {
    TypeDefinition type_definition = program_representation.retrieve_type_definition(custom_type);
    if (type_definition.is<StructDefinition>()) {
        return does_this_struct_imply_a_bond(type_definition.get<StructDefinition>());
    }
    else if (type_definition.is<UnionDefinition>()) {
        return does_this_union_imply_a_bond(type_definition.get<UnionDefinition>());
    }
    else if (type_definition.is<TypeAlias>()) {
        return does_this_type_alias_imply_a_bond(type_definition.get<TypeAlias>());
    } 
    assert_unreachable();
}

bool BondInspector::does_this_struct_imply_a_bond(const StructDefinition& struct_def) {
    for (const StructDefinition::Field& field : struct_def.fields) {
        if (does_the_type_of_this_expr_imply_a_bond(field.field_type)) {
            return true;
        }
    }
    return false;
}

bool BondInspector::does_this_union_imply_a_bond(const UnionDefinition& union_def) {
    for (const TypeSignature& alternative : union_def.types) {
        if (does_the_type_of_this_expr_imply_a_bond(alternative)) {
            return true;
        }
    }
    return false;
}

bool BondInspector::does_this_type_alias_imply_a_bond(const TypeAlias& type_alias) {
    return does_the_type_of_this_expr_imply_a_bond(type_alias.aliased_type);
}