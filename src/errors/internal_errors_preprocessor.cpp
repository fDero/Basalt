
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

void assert_package_contains_at_least_one_file(
    const std::vector<FileRappresentation>& package_files
){
    #ifdef DEBUG_BUILD
    if (package_files.empty()){
        throw InternalError {
            "somehow the MultiThreadPreProcessor found out that a package has 0 files in it \n"
            "(and this should be impossible since a package is only created when \n"
            "a file that belongs to it is encountered)"
        };
    }
    #endif
}