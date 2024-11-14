//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <functional>

#include "model/type_definitions_register.hpp"
#include "language/definitions.hpp"

class TypeDefinitionsLLVMTranslator {
    public:
        TypeDefinitionsLLVMTranslator(TypeDefinitionsRegister& type_definitions_register, llvm::LLVMContext& context)
            : type_definitions_register(type_definitions_register) 
            , context(context)
        { 
            llvm_type_definitions.insert({"Int", llvm::Type::getInt64Ty(context)});
            llvm_type_definitions.insert({"Float", llvm::Type::getDoubleTy(context)});
            llvm_type_definitions.insert({"Bool", llvm::Type::getInt1Ty(context)});
            llvm_type_definitions.insert({"Char", llvm::Type::getInt8Ty(context)});
            //String
            llvm_type_definitions.insert({"RawString", llvm::Type::getInt8Ty(context)->getPointerTo()});
        }

        llvm::Type* translate_typesignature_to_llvm_type(const TypeSignature& type_signature) {
            switch (type_signature.typesiganture_kind()) {
                case TypeSignatureBody::Kind::pointer_type: {
                    llvm::Type* pointed_type = translate_typesignature_to_llvm_type(type_signature.get<PointerType>().pointed_type);
                    return pointed_type->getPointerTo();
                }
                case TypeSignatureBody::Kind::array_type: {
                    llvm::Type* stored_type = translate_typesignature_to_llvm_type(type_signature.get<ArrayType>().stored_type);
                    return llvm::ArrayType::get(stored_type, type_signature.get<ArrayType>().array_length);
                }
                case TypeSignatureBody::Kind::slice_type: {
                    llvm::Type* stored_type = translate_typesignature_to_llvm_type(type_signature.get<SliceType>().stored_type);
                    return stored_type->getPointerTo();
                }
                case TypeSignatureBody::Kind::primitive_type: {
                    std::string primitive_type_name = type_signature.get<PrimitiveType>().type_name;
                    return llvm_type_definitions.at(primitive_type_name);
                }
                case TypeSignatureBody::Kind::custom_type: {
                    return translate_custom_type_to_llvm_type(type_signature.get<CustomType>());
                }
                case TypeSignatureBody::Kind::template_type: {
                    throw std::runtime_error("Template types are not supposed to be translated to LLVM types");
                }
                case TypeSignatureBody::Kind::inline_union: {
                    return translate_inline_union_to_llvm_type(type_signature.get<InlineUnion>());
                }
            }
        }

        llvm::Type* translate_inline_union_to_llvm_type(const InlineUnion& inline_union) {
            std::string inline_union_typename =  type_definitions_register.get_fully_qualified_typesignature_name(inline_union);
            llvm::StructType* llvm_type_def = llvm::StructType::create(context, inline_union_typename);
            size_t union_payload_memory_size_in_bytes = 0;
            for (const auto& alternative : inline_union.alternatives) {
                union_payload_memory_size_in_bytes += type_definitions_register.compute_header_unaware_typesignature_memory_footprint(alternative);
            }
            llvm::Type* union_payload = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), union_payload_memory_size_in_bytes);
            llvm::Type* union_header = llvm::Type::getInt8Ty(context)->getPointerTo();
            llvm_type_def->setBody({ union_header, union_payload });
            return llvm_type_def;
        }

        llvm::Type* translate_custom_type_to_llvm_type(const CustomType& custom_type) {
            TypeSignature unaliased_type = type_definitions_register.unalias_type(custom_type);
            if (unaliased_type.typesiganture_kind() != TypeSignatureBody::Kind::custom_type) {
                return translate_typesignature_to_llvm_type(unaliased_type);
            }
            CustomType unaliased_custom_type = unaliased_type.get<CustomType>();
            TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(unaliased_custom_type);
            std::string fully_qualified_name = type_definitions_register.get_fully_qualified_typedefinition_name(type_definition);
            if (llvm_type_definitions.find(fully_qualified_name) != llvm_type_definitions.end()) {
                return llvm_type_definitions.at(fully_qualified_name);
            }
            llvm::StructType* llvm_type_def = llvm::StructType::create(context, fully_qualified_name);
            llvm_type_definitions.insert({fully_qualified_name, llvm_type_def});
            std::vector<llvm::Type*> field_types;
            if (type_definition.is<StructDefinition>()) {
                StructDefinition struct_definition = type_definition.get<StructDefinition>();
                field_types.reserve(struct_definition.fields.size());
                for (const auto& field : struct_definition.fields) {
                    llvm::Type* llvm_field_type = translate_typesignature_to_llvm_type(field.field_type);
                    field_types.push_back(llvm_field_type);
                }
            }
            else {
                //assert typedef is union
                UnionDefinition union_definition = type_definition.get<UnionDefinition>();
                size_t union_payload_memory_size_in_bytes = type_definitions_register.compute_header_unaware_type_definition_memory_footprint(type_definition);
                llvm::Type* union_payload = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), union_payload_memory_size_in_bytes);
                llvm::Type* union_header = llvm::Type::getInt8Ty(context)->getPointerTo();
                field_types = { union_header, union_payload };
            }
            llvm_type_def->setBody(field_types);
            return llvm_type_definitions.at(fully_qualified_name);
        }
        
    private:
        TypeDefinitionsRegister& type_definitions_register;
        llvm::LLVMContext& context;
        std::unordered_map<std::string, llvm::Type*> llvm_type_definitions;
};

