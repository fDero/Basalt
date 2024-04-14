
#include "errors/internal_errors.hpp"
#include "toolchain/rappresentation.hpp"
#include "language/generics.hpp"
#include <vector>

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& instanciated_generics
){
    #ifdef DEBUG_BUILD
    if (template_generics.size() != instanciated_generics.size()){
        throw InternalError{ "template generics and instanciated generics have different sizes" };
    }
    #endif
}