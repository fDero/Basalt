//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_dot_member_access_to_llvm(
    llvm::BasicBlock* block,
    const DotMemberAccess& dot_member_access
) {    
    DotMemberAccess normalized = program_representation
        .normalize_dot_member_access(dot_member_access, scope_context.raw_scope_context);
    std::optional<TypeSignature> accessed_type_opt = program_representation
        .resolve_expression_type(normalized.struct_value, scope_context.raw_scope_context);
    assert_type_deduction_success_in_backend_layer(accessed_type_opt.has_value());
    TypeSignature accessed_type = accessed_type_opt.value();
    switch (accessed_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::custom_type: return translate_dot_member_access_on_custom_type_to_llvm(
            block, 
            normalized
        );
        case TypeSignatureBody::Kind::primitive_type: return translate_dot_member_access_on_primitive_type_to_llvm(
            block, 
            normalized.struct_value,
            accessed_type.get<PrimitiveType>(),
            normalized.member_name
        );
        case TypeSignatureBody::Kind::slice_type: return translate_dot_member_access_on_slice_type_to_llvm(
            block, 
            normalized.struct_value,
            accessed_type.get<SliceType>(),
            normalized.member_name
        );
        default: assert_unreachable();
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_dot_member_access_on_custom_type_to_llvm(
    llvm::BasicBlock* block,
    const DotMemberAccess& dot_member_access_on_struct
) { 
    size_t field_index = program_representation
        .resolve_field_index(dot_member_access_on_struct, scope_context.raw_scope_context);
    TranslatedExpression target = translate_expression_to_llvm(block, dot_member_access_on_struct.struct_value);
    llvm::IRBuilder<> builder(block);
    llvm::Value* target_address = get_llvm_address(builder, target);
    llvm::Value* field_address = builder.CreateStructGEP(target_address, field_index);
    llvm::Value* field_value = builder.CreateLoad(field_address);
    return (target.address == nullptr)
        ? TranslatedExpression(field_value)
        : TranslatedExpression(field_value, field_address);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_dot_member_access_on_primitive_type_to_llvm(
    llvm::BasicBlock* block,
    const Expression& accessed_expression,
    const PrimitiveType primitive_type,
    const std::string& member_name
) { 
    std::string pattern = primitive_type.type_name + "." + member_name;
    if (pattern == "String.len") return translate_string_or_slice_length_to_llvm(block, accessed_expression);
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_dot_member_access_on_slice_type_to_llvm(
    llvm::BasicBlock* block,
    const Expression& accessed_expression,
    const SliceType& primitive_type,
    const std::string& member_name
) { 
    if (member_name == "len") return translate_string_or_slice_length_to_llvm(block, accessed_expression);
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_string_or_slice_length_to_llvm(
    llvm::BasicBlock* block,
    const Expression& expr
) {
    TranslatedExpression target = translate_expression_to_llvm(block, expr);
    llvm::IRBuilder<> builder(block);
    llvm::Value* target_address = get_llvm_address(builder, target);
    llvm::Value* length_address = builder.CreateStructGEP(target_address, 0);
    llvm::Value* length_value = builder.CreateLoad(length_address);
    return (target.address == nullptr)
        ? TranslatedExpression(length_value)
        : TranslatedExpression(length_value, length_address);
}