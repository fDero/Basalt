//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_definitions_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

TypeDefinitionsLLVMTranslator::TypeDefinitionsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    llvm::LLVMContext& context,
    llvm::Module& llvm_module
)
    : program_representation(program_representation) 
    , context(context)
    , llvm_module(llvm_module)
{ 
    llvm_type_definitions.insert({"Int", llvm::Type::getInt64Ty(context)});
    llvm_type_definitions.insert({"Float", llvm::Type::getDoubleTy(context)});
    llvm_type_definitions.insert({"Bool", llvm::Type::getInt1Ty(context)});
    llvm_type_definitions.insert({"Char", llvm::Type::getInt8Ty(context)});
    llvm_type_definitions.insert({"RawString", llvm::Type::getInt8Ty(context)->getPointerTo()});
    llvm::Type* string_internal_representation[] = {
        llvm_type_definitions["RawString"],
        llvm_type_definitions["Int"],
    };
    llvm_type_definitions.insert({"String", llvm::StructType::create(context, string_internal_representation, "String")});
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_typesignature_to_llvm_type(
    const TypeSignature& type_signature
) {
    const TypeSignature unaliased_type = program_representation.unalias_type(type_signature);
    std::string fully_qualified_typesignature_name = program_representation.get_fully_qualified_typesignature_name(unaliased_type);
    auto search_outcome = llvm_type_definitions.find(fully_qualified_typesignature_name);
    if (search_outcome != llvm_type_definitions.end()) {
        return search_outcome->second;
    }
    switch (type_signature.typesiganture_kind()) {
        case TypeSignatureBody::Kind::pointer_type: {
            const TypeSignature pointed_type = type_signature.get<PointerType>().pointed_type;
            llvm::Type* llvm_pointed_type = translate_typesignature_to_llvm_type(pointed_type);
            return llvm_pointed_type->getPointerTo();
        }
        case TypeSignatureBody::Kind::array_type: {
            const TypeSignature& stored_type = type_signature.get<ArrayType>().stored_type;
            const size_t array_length = type_signature.get<ArrayType>().array_length;
            llvm::Type* llvm_stored_type = translate_typesignature_to_llvm_type(stored_type);
            return llvm::ArrayType::get(llvm_stored_type, array_length);
        }
        case TypeSignatureBody::Kind::slice_type: {
            const TypeSignature& stored_type = type_signature.get<SliceType>().stored_type;
            llvm::Type* llvm_stored_type = translate_typesignature_to_llvm_type(stored_type);
            llvm::StructType* llvm_slice_representation = llvm::StructType::create(context);
            llvm_slice_representation->setBody({ llvm::Type::getInt64Ty(context), llvm_stored_type->getPointerTo() });
            return llvm_slice_representation;
        }
        case TypeSignatureBody::Kind::primitive_type: {
            const PrimitiveType& primitive_type = type_signature.get<PrimitiveType>();
            const std::string& primitive_type_name = primitive_type.type_name;
            return llvm_type_definitions.at(primitive_type_name);
        }
        case TypeSignatureBody::Kind::custom_type: {
            const CustomType& custom_type = type_signature.get<CustomType>();
            return translate_custom_type_to_llvm_type(custom_type);
        }
        case TypeSignatureBody::Kind::template_type: {
            throw std::runtime_error("Template types are not supposed to be translated to LLVM types");
        }
        case TypeSignatureBody::Kind::inline_union: {
            const InlineUnion& inline_union = type_signature.get<InlineUnion>();
            return translate_inline_union_to_llvm_type(inline_union);
        }
    }
    assert_unreachable();
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_inline_union_to_llvm_type(
    const InlineUnion& inline_union
) {
    std::string inline_union_typename =  program_representation.get_fully_qualified_typesignature_name(inline_union);
    llvm::StructType* llvm_type_def = llvm::StructType::create(context, inline_union_typename);
    size_t union_payload_memory_size_in_bytes = 0;
    for (const auto& alternative : inline_union.alternatives) {
        union_payload_memory_size_in_bytes = std::max(
            compute_header_unaware_typesignature_memory_footprint(alternative),
            union_payload_memory_size_in_bytes
        );
    }
    llvm::Type* union_payload = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), union_payload_memory_size_in_bytes);
    llvm::Type* union_header = llvm::Type::getInt8Ty(context)->getPointerTo();
    llvm_type_def->setBody({ union_header, union_payload });
    llvm_type_definitions.insert({inline_union_typename, llvm_type_def});
    return llvm_type_def;
}

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

llvm::Type* TypeDefinitionsLLVMTranslator::translate_named_union_to_llvm_type(
    const UnionDefinition& union_definition
) {
    std::string fully_qualified_name = program_representation.get_fully_qualified_typedefinition_name(union_definition);
    if (llvm_type_definitions.find(fully_qualified_name) != llvm_type_definitions.end()) {
        return llvm_type_definitions.at(fully_qualified_name);
    }
    llvm::StructType* llvm_type_def = llvm::StructType::create(context, fully_qualified_name);
    llvm_type_definitions.insert({fully_qualified_name, llvm_type_def});
    size_t union_payload_memory_size_in_bytes = 0;
    for (const TypeSignature& alternative : union_definition.types) {
        union_payload_memory_size_in_bytes = std::max(
            compute_header_unaware_typesignature_memory_footprint(alternative),
            union_payload_memory_size_in_bytes
        );
    }
    llvm::Type* union_payload = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), union_payload_memory_size_in_bytes);
    llvm::Type* union_header = llvm::Type::getInt8Ty(context)->getPointerTo();
    llvm_type_def->setBody({ union_header, union_payload });
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

llvm::Type* TypeDefinitionsLLVMTranslator::translate_return_type_to_llvm_type(
    const std::optional<TypeSignature>& ret
) {
    return (ret.has_value())
        ? translate_typesignature_to_llvm_type(ret.value())
        : llvm::Type::getVoidTy(context);
}

std::vector<llvm::Type*> TypeDefinitionsLLVMTranslator::translate_all_types_to_llvm_types(
    const std::vector<TypeSignature>& types
) {
    std::vector<llvm::Type*> llvm_types;
    llvm_types.reserve(types.size());
    for (const TypeSignature& type : types) {
        llvm::Type* llvm_type = translate_typesignature_to_llvm_type(type);
        llvm_types.push_back(llvm_type);
    }
    return llvm_types;
}