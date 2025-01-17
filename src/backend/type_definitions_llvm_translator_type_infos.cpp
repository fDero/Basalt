//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_definitions_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

llvm::GlobalVariable* TypeDefinitionsLLVMTranslator::fetch_type_info(
    const TypeSignature& type_signature
) {
    std::string type_signature_name = program_representation.get_fully_qualified_typesignature_name(type_signature);
    if (llvm_exact_type_infos.find(type_signature_name) != llvm_exact_type_infos.end()) {
        return llvm_exact_type_infos.at(type_signature_name);
    }
    llvm::Constant* llvm_type_signature_name_constant = llvm::ConstantDataArray::getString(context, type_signature_name);
    llvm::ArrayType* llvm_type_signature_name_char_array_type = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), type_signature_name.size() + 1);
    llvm::GlobalVariable* global_type_info_var = new llvm::GlobalVariable(
        llvm_module, 
        llvm_type_signature_name_char_array_type, 
        true, 
        llvm::GlobalValue::LinkageTypes::ExternalLinkage, 
        llvm_type_signature_name_constant, 
        type_signature_name 
    );
    llvm_exact_type_infos.insert({type_signature_name, global_type_info_var});
    return global_type_info_var;
}

std::vector<llvm::GlobalVariable*> 
TypeDefinitionsLLVMTranslator::fetch_all_type_infos_for_non_union_compatible_types(
    const TypeSignature& type_signature
) {
    std::string type_signature_name = program_representation.get_fully_qualified_typesignature_name(type_signature);
    if (llvm_compatible_type_infos.find(type_signature_name) != llvm_compatible_type_infos.end()) {
        return llvm_compatible_type_infos.at(type_signature_name);
    }
    std::vector<TypeSignature> compatible_types = program_representation
        .fetch_non_union_compatible_types_for_union_assignment(type_signature);
    std::vector<llvm::GlobalVariable*> compatible_type_infos;
    for (const TypeSignature& compatible_type : compatible_types) {
        llvm::GlobalVariable* compatible_type_info = fetch_type_info(compatible_type);
        compatible_type_infos.push_back(compatible_type_info);
    }
    llvm_compatible_type_infos.insert({type_signature_name, compatible_type_infos});
    return compatible_type_infos;
}