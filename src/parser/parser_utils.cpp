
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"

Parser::Parser(const std::vector<Token>& tokens) {
    source_tokens = tokens; 
    iterator = source_tokens.begin(); 
}

[[nodiscard]] FileRappresentation Parser::parse_everything(){
    FileRappresentation output;
    StructDefinitionsRegister& struct_defs_register = output.struct_defs_register;
    FunctionDefinitionsRegister& func_defs_register = output.func_defs_register;
    while (iterator != source_tokens.end()){
        switch (iterator->type){
            break; case Token::Type::func_keyword:    func_defs_register.store(parse_function_definition());
            break; case Token::Type::struct_keyword:  struct_defs_register.store(parse_struct_definition());
            break; default: throw_invalid_use_of_token_within_global_scope(iterator);
        }
    }
    return output;
}

[[nodiscard]] bool Parser::is_binary_operator(){
    if (iterator == source_tokens.end()) return false;
    auto binary_operator_search_outcome =  infix_operators_priority.find(iterator->sourcetext);
    auto not_found = infix_operators_priority.end();
    return (binary_operator_search_outcome != not_found);
}

[[nodiscard]] bool Parser::is_square_bracket(){
    if (iterator == source_tokens.end()) return false;
    return (iterator->sourcetext == "[");
}

[[nodiscard]] bool Parser::expression_ended(){
    if (iterator == source_tokens.end()) return true;
    if (iterator->sourcetext == ")") return true;
    if (iterator->sourcetext == "]") return true;
    if (iterator->sourcetext == "}") return true;
    if (iterator->sourcetext == ";") return true;
    if (iterator->sourcetext == ",") return true;
    if (iterator->sourcetext == "=") return true;
    return false;
}