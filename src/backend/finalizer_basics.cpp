//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <memory>
#include <optional>
#include <string>

#include <llvm/ADT/Optional.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include "backend/finalizer.hpp"
#include "core/program_representation.hpp"
#include "errors/commandline_errors.hpp"


Finalizer::Finalizer(
    ProgramRepresentation& input_program_representation,
    std::optional<std::string> target_triple
) : Finalizer(
        input_program_representation, 
        target_triple.has_value()
            ? target_triple.value() 
            : llvm::sys::getDefaultTargetTriple()
    )
{}

Finalizer::Finalizer(ProgramRepresentation& program_representation) 
    : Finalizer(
        program_representation, 
        llvm::sys::getDefaultTargetTriple()
    )
{}

Finalizer::Finalizer(
    ProgramRepresentation& input_program_representation,
    const std::string& target_triple
) 
    : program_representation(input_program_representation)
    , target_triple(target_triple)
    , llvm_context(std::make_unique<llvm::LLVMContext>())
    , llvm_module(std::make_unique<llvm::Module>("basalt_program", *llvm_context))
    , type_definitions_llvm_translator(
        program_representation, 
        *llvm_context, 
        *llvm_module
    )
    , callable_codeblocks_llvm_translator(
        program_representation, 
        type_definitions_llvm_translator, 
        *llvm_context, 
        *llvm_module
    )
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();  
    std::string error;
    const llvm::Target* target(llvm::TargetRegistry::lookupTarget(target_triple, error));
    ensure_valid_target_triple(target_triple, error);

    llvm_target_machine = target->createTargetMachine(
        target_triple,
        "generic", 
        "",
        llvm::TargetOptions(), 
        llvm::Optional<llvm::Reloc::Model>()
    );
    llvm_module->setDataLayout(llvm_target_machine->createDataLayout());
}