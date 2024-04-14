
#include "errors/internal_errors.hpp"
#include "toolchain/rappresentation.hpp"
#include <unordered_set>

void assert_parsed_files_outcome_empty(
    const std::vector<FileRappresentation>& parsed_files_outcome
){
    #ifdef DEBUG_BUILD
    if (parsed_files_outcome.size() != 0){
        throw InternalError {
            "somehow the MultiThreadPreProcessor found out that multithread parsing \n"
            "has already been done (because parsed_files_outcome is not empty)"
        };
    }
    #endif
}


void assert_no_errors_encountered_during_async_parsing_hence_error_vector_is_empty(
    const std::vector<std::exception_ptr>& errors
){
    #ifdef DEBUG_BUILD
    if (errors.size() != 0){
        throw InternalError {
            "somehow the MultiThreadPreProcessor found out that some errors occurred before \n"
            "and were not properly handled (because errors_occurred_while_preprocessing is not empty)"
        };
    }
    #endif
}