//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_definitions_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

size_t TypeDefinitionsLLVMTranslator::compute_header_unaware_typesignature_memory_footprint(
    const TypeSignature& typesignature
) {
    TypeSignature unaliased_typesignature = program_representation.unalias_type(typesignature);
    switch (unaliased_typesignature.typesiganture_kind()) {
        case TypeSignatureBody::Kind::custom_type: {
            const CustomType& custom_type = unaliased_typesignature.get<CustomType>();
            return compute_header_unaware_customtype_memory_footprint(custom_type);
        }
        case TypeSignatureBody::Kind::inline_union: {
            const InlineUnion& inline_union = unaliased_typesignature.get<InlineUnion>();
            return compute_header_unaware_inline_union_memory_footprint(inline_union);
        }
        default: {
            llvm::Type* llvm_type = translate_typesignature_to_llvm_type(unaliased_typesignature);
            return llvm_module.getDataLayout().getTypeAllocSize(llvm_type);
        }
    }    
}

size_t TypeDefinitionsLLVMTranslator::compute_header_unaware_inline_union_memory_footprint(
    const InlineUnion& inline_union
) {
    size_t union_payload_memory_size_in_bytes = 0;
    for (const TypeSignature& alternative : inline_union.alternatives) {
        union_payload_memory_size_in_bytes += compute_header_unaware_typesignature_memory_footprint(alternative);
    }
    return union_payload_memory_size_in_bytes;
}

size_t TypeDefinitionsLLVMTranslator::compute_header_unaware_named_union_memory_footprint(
    const UnionDefinition& union_definition
) {
    size_t union_payload_memory_size_in_bytes = 0;
    for (const TypeSignature& alternative : union_definition.types) {
        union_payload_memory_size_in_bytes += compute_header_unaware_typesignature_memory_footprint(alternative);
    }
    return union_payload_memory_size_in_bytes;
}

size_t TypeDefinitionsLLVMTranslator::compute_header_unaware_customtype_memory_footprint(
    const CustomType& custom_type
){
    TypeDefinition type_definition = program_representation.retrieve_type_definition(custom_type);
    if (!type_definition.is<UnionDefinition>()) {
        llvm::Type* llvm_type = translate_custom_type_to_llvm_type(custom_type);
        return llvm_module.getDataLayout().getTypeAllocSize(llvm_type);
    }
    return compute_header_unaware_named_union_memory_footprint(type_definition.get<UnionDefinition>());
}