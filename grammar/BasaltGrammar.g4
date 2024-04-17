parser grammar BasaltGrammar;
options { tokenVocab=BasaltVocabulary; }





// PROGRAM

program
    : definition* EOF
    ;

definition
    : struct_definition
    | union_definition
    | function_definition
    | variable_declaration
    | const_declaration
    ;




// DECLARATIONS

variable_declaration
    : VAR ID ':' typesignature '=' expression ';'
    | VAR ID ':' typesignature ';'
    ;
    
const_declaration
    : CONST ID ':' typesignature '=' expression ';'
    ; 





// STRUCTS

struct_definition
    : STRUCT TYPENAME formal_type_parameters_section? 
      '{' struct_field* '}'
    ;

struct_field
    : ID ':' typesignature ';' 
    ;





// UNIONS

union_definition
    : UNION TYPENAME formal_type_parameters_section? 
      '{' typesignature* '}'
    ;





// FUNCTIONS 

function_definition
    : FUNC ID formal_type_parameters_section? 
      function_definition_arguments_section
      function_return_type_section? 
      multiline_scoped_instruction_block
    ;
    
function_definition_arguments_section
    : '(' function_definition_arguments_list? ')'
    ;
    
function_definition_arguments_list
    : ID ':' typesignature (',' ID ':' typesignature)* 
    ;
    
function_return_type_section
    : ':' typesignature
    ;
    
function_call
    : ID actual_type_parameters_section? 
      actual_function_call_arguments_section
    ;
    
function_call_statement
    : function_call ';'
    ;

actual_function_call_arguments_section
    : '(' expression (',' expression)* ')' 
    | '(' ')'
    ;





// TYPES

typesignature
    : BASETYPE
    | TYPENAME actual_type_parameters_section?
    | pointer_type
    | slice_type
    | array_type
    ;

pointer_type
    : '#' typesignature ;

slice_type
    : '$' typesignature ;

array_type
    : '[' INT_LITERAL ']' typesignature ;

actual_type_parameters_section
    : '<' typesignature (',' typesignature)* '>'
    ;

formal_type_parameters_section
    : '<' TYPENAME (',' TYPENAME)* '>'
    ;






// EXPRESSIONS
            
expression
    : terminal_expression
    | infix_operator
    | dot_member_access
    ;

terminal_expression
    : array_square_brackets_access 
    | array_literal
    | function_call
    | parenthesys_delimited_expression
    | prefix_operator
    | INT_LITERAL
    | FLOAT_LITERAL
    | CHAR_LITERAL
    | STRING_LITERAL
    | BOOL_LITERAL
    | ID
    ;

infix_operator
    : terminal_expression (PLUS | MINUS | MUL | DIV | POW | MOD) expression
    | terminal_expression (AND | OR | EQUAL | NOTEQUAL) expression
    | terminal_expression (GT | LT | LE | GE | EQUAL | NOTEQUAL) expression
    ;

dot_member_access
    : terminal_expression ('.' ID)+
    ;

prefix_operator
    : ('#' | NOT | PLUS | MINUS | INC | DEC) expression
    ;

array_square_brackets_access
    : (ID | parenthesys_delimited_expression | array_literal | function_call) ('[' expression ']')+
    ;

array_literal
    : '[' typesignature ']' '{' expression (',' expression)* '}' ;

parenthesys_delimited_expression
    : '(' expression ')'
    ;






// STATEMENTS

multiline_scoped_instruction_block
    : '{' statement* '}'
    ;
    
assignment
    : terminal_expression '=' expression ';'
    | terminal_expression (ADDEQ | SUBEQ | MULEQ | DIVEQ) expression ';'
    | terminal_expression (OREQ | POWEQ | MODEQ | ANDEQ) expression ';'
    ;

statement
    : function_call_statement
    | assignment
    | variable_declaration
    | const_declaration
    | return_statement
    | break_statement
    | continue_statement
    | if_statement
    | while_loop
    | until_loop
    ;
    
return_statement
    : RETURN expression? ';'
    ;
    
break_statement
    : BREAK ';'
    ;

continue_statement
    : CONTINUE ';'
    ;
    
instruction_block
    : multiline_scoped_instruction_block
    | statement
    ;

if_statement
    : IF '(' expression ')' instruction_block 'else' instruction_block
    | IF '(' expression ')' instruction_block
    ;
    
while_loop
    : WHILE '(' expression ')' instruction_block
    ;

until_loop
    : UNTIL '(' expression ')' instruction_block
    ;

switch_statement
    : SWITCH '(' expression ')' '{' switch_case* '}'
    ;

switch_case
    : DEFAULT instruction_block
    | ID ':' typesignature instruction_block
    ;
