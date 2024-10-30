//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"

[[noreturn]] void assert_unreachable() {
    throw InternalError{
        "an execution path that should have been unreachable has been reached! (assert_unreachable)"
    };
}
void assert_get_operation_is_possible(const char* wanted_type, const std::type_info& type_info) {
    #ifdef DEBUG_BUILD
    if (wanted_type != type_info.name()) {
        throw InternalError {
            "an attempt has been made to cast something that was holding " 
            "the type: " + std::string(type_info.name()) + " to the type: " + wanted_type
        };
    }
    #endif
}