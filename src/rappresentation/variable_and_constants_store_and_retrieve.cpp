
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"

const TypeSignature& ScopedData::get_object_type_by_name(const std::string& name) const {
    for (const VariableDeclaration& var : variables) {
        if (var.identifier_name == name) {
            return var.typesignature;
        }
    }
    for (const ConstDeclaration& con : constants) {
        if (con.identifier_name == name) {
            return con.typesignature;
        }
    }
    throw std::runtime_error("NO OBJECT FOUND: " + name);
}