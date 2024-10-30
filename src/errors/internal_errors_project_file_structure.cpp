//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"

void assert_packagename_is_found(
    const std::unordered_map<std::string, std::string>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::string>::const_iterator& end
) {
    #ifdef DEBUG_BUILD
    if (search_outcome == end) {
        throw InternalError {
            "package name was not found in the map of package names by file names"
        };
    }
    #endif
}

void assert_files_are_found(
    const std::unordered_map<std::string, std::list<FileRepresentation>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::list<FileRepresentation>>::const_iterator& end
) {
    #ifdef DEBUG_BUILD
    if (search_outcome == end) {
        throw InternalError {
            "files vector was not found in the map of files by package names"
        };
    }
    #endif
}

void assert_imports_vector_is_found(
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator& end
) {
    #ifdef DEBUG_BUILD
    if (search_outcome == end) {
        throw InternalError {
            "imports vector was not found in the map of imports by file names"
        };
    }
    #endif
}

void assert_overload_set_exists(
    const std::unordered_map<std::string, std::vector<FunctionDefinition::Ref>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<FunctionDefinition::Ref>>::const_iterator& end
) {
    #ifdef DEBUG_BUILD
    if (search_outcome == end) {
        throw InternalError {
            "overload set was not found in the map of overload sets by overload ids"
        };
    }
    #endif
}