grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' instr_stmt* return_stmt '}' ;


instr_stmt: var_stmt
    | call_function
    ;


return_stmt: RETURN CONST ';' #returnConst
    | RETURN VAR ';' #returnVar
    | RETURN expr ';' #returnExpr
    ;


var_stmt: affectation_var
    | declaration_var
    | initialisation_var
    ;

affectation_var: VAR '=' VAR ('=' VAR)* ('=' CONST | '=' expr)? ';' #varToVar
    | VAR '=' CONST  ';' #varToConst
    | VAR '=' expr ';' #varToExpr
    ;
declaration_var: 'int' VAR (',' VAR)* ';' ;
initialisation_var: 'int' VAR '=' VAR ( ',' VAR '=' VAR)* ';' #varInitVar
    | 'int' VAR '=' CONST ( ',' VAR '=' CONST)* ';' #varInitConst
    | 'int' VAR '=' expr (',' VAR '=' expr)* ';' #varInitExpr
    ;


expr: '(' expr ')' #par
    | call_function #function
    | '-' expr #unaire
    | expr OP=('*' | '/' | '%') expr #multdivmod
    | expr OP=('+' | '-') expr #plusmoins
    | VAR #var
    | CONST #const
    ;


call_function : FUNCTION_NAME '(' expr? (',' expr)* ')' (';')? ;


RETURN : 'return' ;
FUNCTION_NAME :  'putchar'| 'getchar' ;
CONST : [0-9]+ | '\'' (.? | '\\'.) '\'';
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
KEYWORD : 'auto'|'break'|'case'|'char'|'const'|'continue'|'default'|'do'|'double'|'else'|'enum'|'extern'|'float'|'for'|'goto'|'if'|'int'|'long'|'register'|'return'|'short'|'signed'|'sizeof'|'static'|'struct'|'switch'|'typedef'|'union'|'unsigned'|'void'|'volatile'|'while' ;
VAR : [a-zA-Z_][a-zA-Z0-9_]*;

