//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/display_utilities.hpp"
#include "cli/commandline.hpp"

#include <exception>
#include <cstdlib>

int main(int argc, char** argv) {
    try {
        CommandLineController controller(argc, argv);
        controller.dispatch_based_on_user_input();
        exit(EXIT_SUCCESS);
    }
    catch(...) {
        std::exception_ptr error = std::current_exception();
        display_pretty_error_message(error);
        exit(EXIT_FAILURE);
    }
}
