//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <optional>
#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Target/TargetMachine.h>

#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "misc/forward_declarations.hpp"


class Finalizer {
    public:
        Finalizer(ProgramRepresentation& program_representation, std::optional<std::string> target_triple_opt);
        Finalizer(ProgramRepresentation& program_representation, const std::string& target_triple);
        Finalizer(ProgramRepresentation& program_representation);

        void emit_object_file(const std::string& output_file);
        void emit_assembly_file(const std::string& output_file_name);
        void emit_llvm_ir(const std::string& output_file);
        void generate_llvm_ir();

    private:
        ProgramRepresentation& program_representation;
        std::string target_triple;
        llvm::LLVMContext llvm_context;
        llvm::Module llvm_module;
        llvm::TargetMachine* llvm_target_machine;
        TypeDefinitionsLLVMTranslator type_definitions_llvm_translator;
        CallableCodeBlocksLLVMTranslator callable_codeblocks_llvm_translator;
};
