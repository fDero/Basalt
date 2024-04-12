lexer grammar BasaltVocabulary;

STRUCT   : 'struct';
UNION    : 'union';
SWITCH   : 'switch';
DEFAULT  : 'default';
FUNC     : 'func';
RETURN   : 'return';
BREAK    : 'break';
CONTINUE : 'continue';
IF       : 'if';
ELSE     : 'else';
WHILE    : 'while';
UNTIL    : 'until';
VAR      : 'var';
CONST    : 'const';

INT_LITERAL    : [0-9]+ ;
FLOAT_LITERAL  : [0-9]+'.'[0-9]+;
BOOL_LITERAL   : 'true'|'false';
CHAR_LITERAL   : '\''[a-zA-Z_0-9]*'\'';
STRING_LITERAL : '"'[a-zA-Z_0-9]*'"';

BASETYPE : 'Int | Float | Bool | Char | String';
ID       : [a-z][a-zA-Z_0-9]* ;
TYPENAME : [A-Z][a-zA-Z_0-9]* ;
FUNCNAME : [a-z][a-zA-Z_0-9]* ;
WS       : [ \t\n\r\f]+ -> skip;

LPAREN    : '(';
RPAREN    : ')';
LBRACE    : '{';
RBRACE    : '}';
LBRACK    : '[';
RBRACK    : ']';
SEMICOLON : ';';
COMMA     : ',';
COLON     : ':';
DOT       : '.';

EQ        : '=';
GT        : '>';
LT        : '<';
NOT       : '!';
TILDE     : '~';
QUESTION  : '?';
EQUAL     : '==';
LE        : '<=';
GE        : '>=';
NOTEQUAL  : '!=';
AND       : '&&';
OR        : '||';
INC       : '++';
DEC       : '--';
PLUS      : '+';
MINUS     : '-';
MUL       : '*';
DIV       : '/';
BITAND    : '&';
BITOR     : '|';
POW       : '^';
MOD       : '%';

ADDEQ     : '+=';
SUBEQ     : '-=';
MULEQ     : '*=';
DIVEQ     : '/=';
ANDEQ     : '&=';
OREQ      : '|=';
POWEQ     : '^=';
MODEQ     : '%=';
ARROW     : '->';
COLONSQ   : '::';

HASHTAG   : '#';
DOLLAR    : '$';
AT        : '@';
ELLIPSIS  : '...';
