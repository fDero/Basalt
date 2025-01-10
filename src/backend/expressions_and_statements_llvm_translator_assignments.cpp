//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"

void ExpressionsAndStatementsLLVMTranslator::translate_variable_declaration_into_llvm(const VariableDeclaration& variable_declaration) {
    llvm::Type* llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(variable_declaration.typesignature);
    llvm::AllocaInst* alloca_inst = builder.CreateAlloca(llvm_type, nullptr);
    scope_context.store_local_variable(variable_declaration, alloca_inst);
    if (variable_declaration.initial_value.has_value()) {
        Identifier identifier(variable_declaration, variable_declaration.identifier_name);
        translate_assignment_into_llvm(identifier, variable_declaration.initial_value.value());
    }
}

void ExpressionsAndStatementsLLVMTranslator::translate_constant_declaration_into_llvm(const ConstDeclaration& const_declaration) {
    llvm::Type* llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(const_declaration.typesignature);
    llvm::AllocaInst* alloca_inst = builder.CreateAlloca(llvm_type, nullptr);
    scope_context.store_local_constant(const_declaration, alloca_inst);
    Identifier identifier(const_declaration, const_declaration.identifier_name);
    translate_assignment_into_llvm(identifier, const_declaration.value);
}

void ExpressionsAndStatementsLLVMTranslator::translate_assignment_into_llvm(const Assignment& assignment) {
    translate_assignment_into_llvm(assignment.assignment_target, assignment.assigned_value);
}

void ExpressionsAndStatementsLLVMTranslator::translate_assignment_into_llvm(const Expression& target, const Expression& source) {
    throw std::runtime_error("not implemented yet");
    /* std::optional<TypeSignature> target_type_opt = program_representation.resolve_expression_type(target, scope_context.raw_scope_context);
    std::optional<TypeSignature> source_type_opt = program_representation.resolve_expression_type(source, scope_context.raw_scope_context);
    assert_type_correctly_deduced_during_backend_llvm_translation(target_type_opt, target);
    assert_type_correctly_deduced_during_backend_llvm_translation(source_type_opt, source);
    TypeSignature target_type = program_representation.unalias_type(target_type_opt.value());
    TypeSignature source_type = program_representation.unalias_type(source_type_opt.value());
    if (target_type.is<InlineUnion>()) {
        translate_assignment_to_union_into_llvm(target, target_type, source, source_type);
        return;
    }
    if (target_type.is<CustomType>()) {
        TypeDefinition type_def = program_representation.retrieve_type_definition(target_type.get<CustomType>());
        if (type_def.is<UnionDefinition>()) {
            translate_assignment_to_union_into_llvm(target, target_type,source, source_type);
            return;
        }
    }
    llvm::Value* source_value = translate_expression_into_llvm(source).value;
    TranslatedExpression target_expr = translate_expression_into_llvm(target);
    builder.CreateStore(source_value, target_expr.address); */
}