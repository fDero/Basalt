//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <llvm/Support/Host.h>

#include "frontend/file_descriptor.hpp"
#include "frontend/user_command.hpp"
#include "errors/commandline_errors.hpp"

using Files = std::vector<FileDescriptor>;
using SubCommand = UserCommand::SubCommand;


static const std::regex cmdrgx(
    R"(^(?:version|help|build|exec|check))"
    R"((?:\s+(?:-t|--target|-i|--include|-s|--source|-o|--output|-l|--links|[^-\s]\S*))*$)"
);

UserCommand::UserCommand(int argc, char** argv) {
    std::vector<std::string>  cmd(argv, argv + argc);
    std::vector<std::string>  fake_dummy_vector;
    std::vector<std::string>* active_vector = &fake_dummy_vector;

    std::ostringstream oss;
    for (size_t i = 1; i < cmd.size(); ++i) {
        switch (i > 1) {
            case true:  oss << ' ';
            case false: oss << cmd[i];
        };
    }

    if (!std::regex_match(oss.str(), cmdrgx)) throw_unspecified_commandline_subcommand();
    else if (cmd[1] == "version") subcommand = UserCommand::SubCommand::version;
    else if (cmd[1] == "help")    subcommand = UserCommand::SubCommand::help;
    else if (cmd[1] == "build")   subcommand = UserCommand::SubCommand::build;
    else if (cmd[1] == "check")   subcommand = UserCommand::SubCommand::check;
    else if (cmd[1] == "exec")    subcommand = UserCommand::SubCommand::exec;

    for (auto it = cmd.begin() + 2; it != cmd.end(); ++it) {
        if (*it == "-t" || *it == "--target")  active_vector = &this->targets;
        if (*it == "-i" || *it == "--include") active_vector = &this->includes;
        if (*it == "-s" || *it == "--source")  active_vector = &this->inputs;
        if (*it == "-o" || *it == "--output")  active_vector = &this->outputs;
        if (*it == "-l" || *it == "--links")   active_vector = &this->links;
        if (!it->empty() && (*it)[0] != '-')   active_vector->emplace_back(*it);
    }
}

static Files as_file_descriptors(const std::vector<std::string>& paths) {
    return Files(paths.begin(), paths.end());
}

Files UserCommand::parse_inputs() const {
    if (subcommand == SubCommand::version || subcommand == SubCommand::help) {
        ensure_lack_of_input_files(inputs);
        return {};
    }
    avoid_lack_of_input_files(inputs);
    avoid_duplicate_input_files(inputs);
    ensure_input_files_exist(inputs);
    Files files = as_file_descriptors(inputs);
    ensure_files_are_basalt_sources(files);
    return files;
}

Files UserCommand::parse_outputs() const {
    if (subcommand != SubCommand::build) {
        ensure_lack_of_output_files(outputs);
        return {};
    }
    avoid_lack_of_output_files(outputs);
    Files files = as_file_descriptors(outputs);
    ensure_files_are_compilation_outputs(files);
    avoid_duplicate_output_file_extensions(files);
    return files;
}

Files UserCommand::parse_links() const {
    if (subcommand != SubCommand::exec) {
        ensure_lack_of_link_files(links);
        return {};
    }
    Files files = as_file_descriptors(links);
    ensure_files_are_libraries(files);
    return files;
}

std::string UserCommand::parse_target() const {
    if (subcommand != SubCommand::build) {
        ensure_lack_of_target_triple(targets);
    }
    avoid_multiple_target_triples(targets);
    return targets.empty() ? llvm::sys::getDefaultTargetTriple() : targets.front();
}
