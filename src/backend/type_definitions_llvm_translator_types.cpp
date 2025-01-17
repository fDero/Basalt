//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_definitions_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

llvm::Type* TypeDefinitionsLLVMTranslator::translate_custom_type_to_llvm_type(
    const CustomType& custom_type
) {
    TypeSignature unaliased_type = program_representation.unalias_type(custom_type);
    if (unaliased_type.typesiganture_kind() != TypeSignatureBody::Kind::custom_type) {
        return translate_typesignature_to_llvm_type(unaliased_type);
    }
    const CustomType& unaliased_customtype = unaliased_type.get<CustomType>();
    TypeDefinition type_definition = program_representation.retrieve_type_definition(unaliased_customtype);
    if (type_definition.is<StructDefinition>()) {
        return translate_struct_to_llvm_type(type_definition.get<StructDefinition>());
    }
    assert_typedefinition_is<UnionDefinition>(type_definition);
    return translate_named_union_to_llvm_type(type_definition.get<UnionDefinition>());
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_pointer_type_to_llvm_type(
    const PointerType& pointer_type
) {
    const TypeSignature& pointed_type = pointer_type.pointed_type;
    llvm::Type* llvm_pointed_type = translate_typesignature_to_llvm_type(pointed_type);
    return llvm_pointed_type->getPointerTo();
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_primitive_type_to_llvm(
    const PrimitiveType& primitive_type
) {
    const std::string primitive_type_name = primitive_type.type_name;
    llvm::Type* primitive_llvm_type = llvm_type_definitions.at(primitive_type_name);
    return primitive_llvm_type;
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_array_type_to_llvm_type(
    const ArrayType& array_type
) {
    const TypeSignature& stored_type = array_type.stored_type;
    const size_t array_length = array_type.array_length;
    llvm::Type* llvm_stored_type = translate_typesignature_to_llvm_type(stored_type);
    return llvm::ArrayType::get(llvm_stored_type, array_length);
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_slice_type_to_llvm_type(
    const SliceType& slice_type
) {
    const std::string fully_qualified_name = program_representation.get_fully_qualified_typesignature_name(slice_type);
    auto search_outcome = llvm_type_definitions.find(fully_qualified_name);
    if (search_outcome != llvm_type_definitions.end()) {
        return search_outcome->second;
    }
    const TypeSignature& stored_type = slice_type.stored_type;
    llvm::Type* llvm_stored_type = translate_typesignature_to_llvm_type(stored_type);
    llvm::StructType* llvm_slice_representation = llvm::StructType::create(context, fully_qualified_name);
    llvm_slice_representation->setBody({ llvm::Type::getInt64Ty(context), llvm_stored_type->getPointerTo() });
    llvm_type_definitions.insert({fully_qualified_name, llvm_slice_representation});
    return llvm_slice_representation;
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_inline_union_to_llvm_type(
    const InlineUnion& inline_union
) {
    const std::string fully_qualified_name = program_representation.get_fully_qualified_typesignature_name(inline_union);
    return translate_union_to_llvm_type(inline_union.alternatives, fully_qualified_name);
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_named_union_to_llvm_type(
    const UnionDefinition& union_definition
) {
    const std::string fully_qualified_name = program_representation.get_fully_qualified_typedefinition_name(union_definition);
    return translate_union_to_llvm_type(union_definition.types, fully_qualified_name);
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_union_to_llvm_type(
    const std::vector<TypeSignature>& alternatives,
    const std::string& fully_qualified_name
) {
    auto search_outcome = llvm_type_definitions.find(fully_qualified_name);
    if (search_outcome != llvm_type_definitions.end()) {
        return search_outcome->second;
    }
    llvm::StructType* llvm_type_def = llvm::StructType::create(context, fully_qualified_name);
    llvm_type_definitions.insert({fully_qualified_name, llvm_type_def});
    size_t union_payload_memory_size_in_bytes = 0;
    for (const TypeSignature& alternative : alternatives) {
        union_payload_memory_size_in_bytes = std::max(
            compute_header_unaware_typesignature_memory_footprint(alternative),
            union_payload_memory_size_in_bytes
        );
    }
    llvm::Type* union_payload = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), union_payload_memory_size_in_bytes);
    llvm::Type* union_header = llvm::Type::getInt8Ty(context)->getPointerTo();
    llvm_type_def->setBody({ union_header, union_payload });
    llvm_type_definitions.insert({fully_qualified_name, llvm_type_def});
    return llvm_type_definitions.at(fully_qualified_name);
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_struct_to_llvm_type(
    const StructDefinition& struct_definition
) {
    std::string fully_qualified_name = program_representation.get_fully_qualified_typedefinition_name(struct_definition);
    if (llvm_type_definitions.find(fully_qualified_name) != llvm_type_definitions.end()) {
        return llvm_type_definitions.at(fully_qualified_name);
    }
    llvm::StructType* llvm_type_def = llvm::StructType::create(context, fully_qualified_name);
    llvm_type_definitions.insert({fully_qualified_name, llvm_type_def});
    std::vector<llvm::Type*> fields_types;
    fields_types.reserve(struct_definition.fields.size());
    for (const StructDefinition::Field& field : struct_definition.fields) {
        llvm::Type* llvm_field_type = translate_typesignature_to_llvm_type(field.field_type);
        fields_types.push_back(llvm_field_type);
    }
    if (fields_types.empty()) {
        fields_types.push_back(llvm::Type::getInt1Ty(context));
    }
    llvm_type_def->setBody(fields_types);
    return llvm_type_definitions.at(fully_qualified_name);
}