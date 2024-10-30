//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/parsing_errors.hpp"

[[noreturn]] void throw_expression_expected_got_unrecognized(
    const std::vector<Token>::iterator& iterator
) {
    throw ParsingError {
        "unexpected token: an expression was expected, instead " + iterator->sourcetext + " was given",
        *iterator
    };
}

[[noreturn]] void throw_invalid_use_of_token_within_global_scope(
    const std::vector<Token>::iterator& iterator
) {
    throw ParsingError {
        "the given token is not usable at global scope level: ",
        *iterator
    };
}