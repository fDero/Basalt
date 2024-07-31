
#include "toolchain/representation.hpp"
#include "toolchain/parser.hpp"
#include "toolchain/preprocessor.hpp"
#include <vector>
#include <string>

class Compiler {

    public:
        Compiler(
            const std::vector<std::string>& source_files, 
            const std::vector<std::string>& output_files
        );

        void perform_static_analysis();
        void perform_code_generation();
        void output_the_required_files();

    private:
        ProjectFileStructure project_file_structure;
        TypeDefinitionsRegister type_definitions_register;
        FunctionOverloadsRegister function_overloads_register;
        OverloadingResolutionEngine overloading_resolution_engine;
        PreProcessor preprocessor;
        
        const std::vector<std::string>& output_files;
};