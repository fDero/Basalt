//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/definitions.hpp"

size_t TypeDefinition::get_number_of_generics() const {
    auto visiting_lambda = [](const auto& type_def) -> size_t {
        return type_def.template_generics_names.size();
    };
    return std::visit(visiting_lambda, *this);
}

std::string TypeDefinition::get_simple_name() const {
    auto visiting_lambda = [](const auto& type_def) -> std::string {
        return type_def.def_name;
    };
    return std::visit(visiting_lambda, *this);
}

void TypeDefinition::set_name(const std::string& name) {
    auto visiting_lambda = [&name](auto& type_def) {
        type_def.def_name = name;
    };
    std::visit(visiting_lambda, *this);
}

const std::vector<std::string>& TypeDefinition::get_template_generics() const {
    auto visiting_lambda = [](const auto& type_def) -> const std::vector<std::string>& {
        return type_def.template_generics_names;
    };
    return std::visit(visiting_lambda, *this);
}

const std::string& TypeDefinition::get_filename() const {
    auto visiting_lambda = [](const auto& type_def) -> const std::string& {
        return type_def.filename;
    };
    return std::visit(visiting_lambda, *this);
}

std::string TypeAlias::generate_alias_id() const {
    return filename + "/" + std::to_string(line_number) + "/" +
        std::to_string(char_pos) + "/" + std::to_string(tok_number) + "/" + def_name;
}

std::string UnionDefinition::generate_union_id() const {
    return filename + "/" + std::to_string(line_number) + "/" +
        std::to_string(char_pos) + "/" + std::to_string(tok_number) + "/" + def_name;
}

std::string StructDefinition::generate_struct_id() const {
    return filename + "/" + std::to_string(line_number) + "/" +
        std::to_string(char_pos) + "/" + std::to_string(tok_number) + "/" + def_name;
}