//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <unordered_set>

#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
) {
    throw std::runtime_error {
        "No type definition found for type " + type_signature.type_name + "\n"
    };
}

[[noreturn]] void throw_cannot_access_square_brackets_on_type(
    const TypeSignature& left_operand_type,
    const SquareBracketsAccess& expression
) {
    throw InternalError("cannot access square brackets on type");
}

[[noreturn]] void throw_no_such_struct_field(
    const std::string& member_name,
    const StructDefinition& struct_type_definition,
    const DotMemberAccess& expression
) {
    throw InternalError("no such struct field");
}

[[noreturn]] void throw_bad_assignment_target(
    const Expression& expression
) {
    throw InternalError("bad assignment target");
}