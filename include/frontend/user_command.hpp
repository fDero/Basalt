//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>

#include "frontend/file_descriptor.hpp"

struct UserCommand {
    enum class SubCommand {
        version,
        help,
        build,
        exec,
        check
    };

    UserCommand(int argc, char** argv);

    [[nodiscard]] std::vector<FileDescriptor> parse_inputs() const;
    [[nodiscard]] std::vector<FileDescriptor> parse_outputs() const;
    [[nodiscard]] std::vector<FileDescriptor> parse_links() const;
    [[nodiscard]] std::string parse_target() const;

    SubCommand subcommand;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<std::string> targets;
    std::vector<std::string> includes;
    std::vector<std::string> links;
};
