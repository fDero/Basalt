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
        llvm_type_definitions["Int"],
        llvm_type_definitions["RawString"],
    };
    llvm::Type* string_type = llvm::StructType::create(context, string_internal_representation, "String");
    llvm_type_definitions.insert({"String", string_type});
}

llvm::Type* TypeDefinitionsLLVMTranslator::translate_typesignature_to_llvm_type(
    const TypeSignature& type_signature
) {
    switch (type_signature.typesiganture_kind()) {
        case TypeSignatureBody::Kind::pointer_type: return translate_pointer_type_to_llvm_type(type_signature.get<PointerType>());
        case TypeSignatureBody::Kind::array_type: return translate_array_type_to_llvm_type(type_signature.get<ArrayType>());
        case TypeSignatureBody::Kind::slice_type: return translate_slice_type_to_llvm_type(type_signature.get<SliceType>());
        case TypeSignatureBody::Kind::custom_type: return translate_custom_type_to_llvm_type(type_signature.get<CustomType>());
        case TypeSignatureBody::Kind::inline_union: return translate_inline_union_to_llvm_type(type_signature.get<InlineUnion>());
        case TypeSignatureBody::Kind::primitive_type: return translate_primitive_type_to_llvm(type_signature.get<PrimitiveType>());
        case TypeSignatureBody::Kind::template_type: throw_generics_not_allowed_in_backend_layer(type_signature.get<TemplateType>());
    }
    assert_unreachable();
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

llvm::Type* TypeDefinitionsLLVMTranslator::translate_return_type_to_llvm_type(
    const std::optional<TypeSignature>& ret
) {
    return (ret.has_value())
        ? translate_typesignature_to_llvm_type(ret.value())
        : llvm::Type::getVoidTy(context);
}

llvm::Type* TypeDefinitionsLLVMTranslator::get_int_llvm_type() {
    return llvm_type_definitions["Int"];
}

llvm::Type* TypeDefinitionsLLVMTranslator::get_float_llvm_type() {
    return llvm_type_definitions["Float"];
}

llvm::Type* TypeDefinitionsLLVMTranslator::get_char_llvm_type() {
    return llvm_type_definitions["Char"];
}

llvm::Type* TypeDefinitionsLLVMTranslator::get_bool_llvm_type() {
    return llvm_type_definitions["Bool"];
}

llvm::Type* TypeDefinitionsLLVMTranslator::get_string_llvm_type() {
    return llvm_type_definitions["String"];
}

llvm::Type* TypeDefinitionsLLVMTranslator::get_raw_string_llvm_type() {
    return llvm_type_definitions["RawString"];
}