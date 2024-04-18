
#include "errors/internal_errors.hpp"

[[noreturn]] void assert_unreachable(){
    throw InternalError{
        "an execution path that should have been unreachable has been reached! (assert_unreachable)"
    };
}

[[noreturn]] void throw_unknown_token_type(){
    throw InternalError{
        "during tokenization, a token was found such that its type is unknown"
    };
}

[[noreturn]] void throw_attempt_to_retrieve_struct_definition_from_primitive_type(const TypeSignature& type){
    throw InternalError {
        "the type was expected to be a primitive type but it wasn't"
    };
}

void assert_basetype_has_no_generics(const TypeSignature& typesignature){
    #ifdef DEBUG_BUILD
    if (!typesignature.is<CustomType>()){
        throw InternalError {
            "typesignature was expected to be a base type (non generic), but this wasn't the case"
        };
    }
    if (!(typesignature.get<CustomType>().instanciated_generics.empty())){
        throw InternalError {
            "base-type was expected to be non generic, but this wasn't the case"
        };
    }
    #endif
}

void assert_type_is_non_primitive(const TypeSignature& type){
    #ifdef DEBUG_BUILD
    if (type.is<CustomType>()) {
        std::string type_name = type.get<CustomType>().type_name;
        if (primitive_types.find(type_name) != primitive_types.end()) {
            throw_attempt_to_retrieve_struct_definition_from_primitive_type(type);
        }
    }
    #endif
}

void assert_token_sourcetext_non_empty(const std::string& sourcetext){
    #ifdef DEBUG_BUILD
    if (sourcetext.empty()){
        throw InternalError {
            "during tokenization, a token was found such that its sourcetext it's empty"
        };
    }
    #endif
}

void assert_function_actually_retrieved(
    const std::unordered_map<std::string, FunctionDefinition>::const_iterator iterator,
    const std::unordered_map<std::string, FunctionDefinition>& function_definitions_register
){
    #ifdef DEBUG_BUILD
    if (iterator == function_definitions_register.end()){
        throw InternalError {
            "such function doesn't exists in the given scope",
        };
    }
    #endif
}

void assert_get_operation_is_possible(const std::string& wanted_type, const std::type_info& type_info){
    #ifdef DEBUG_BUILD
    if (wanted_type != type_info.name()){
        throw InternalError {
            "an attempt has been made to cast something that was holding " 
            "the type: " + std::string(type_info.name()) + " to the type: " + wanted_type
        };
    }
    #endif
}