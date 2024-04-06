grammar ifcc;

axiom : personal_function* prog EOF ;

prog : 'int' 'main' '(' ')' block ;

personal_function: declaration_function
    | definition_function
    ;

declaration_function: TYPE_RETURN=('int' | 'void') VAR '(' (parametres)? ')' ';';
definition_function: TYPE_RETURN=('int' | 'void') VAR '(' (parametres)? ')' block ;


parametres : parametre (',' parametre)*;
parametre : TYPE=('int'|'char') VAR;


block: '{' (statement)* '}' ;

statement: var_stmt
    | if_else_stmt
    | while_stmt
    | call_function ';'
    | return_stmt
    | block
    ;

return_stmt: RETURN CONST ';' #returnConst
    | RETURN VAR ';' #returnVar
    | RETURN expr ';' #returnExpr
    ;


var_stmt: affectation_var
    | declaration_var ';'
    | initialisation_var
    ;

if_else_stmt: 'if' '(' expr ')' block (else_stmt)? ;
else_stmt: 'else' block ;

while_stmt: 'while' '(' expr ')' '{' (statement)* '}' ;


affectation_var: VAR '=' VAR ('=' VAR)* ('=' CONST | '=' expr)? ';' #varToVar
    | VAR '=' CONST  ';' #varToConst
    | VAR '=' expr ';' #varToExpr
    ;
declaration_var: 'int' VAR (',' VAR)* ;
initialisation_var: 'int' VAR '=' VAR ( ',' VAR '=' VAR)* ';' #varInitVar
    | 'int' VAR '=' CONST ( ',' VAR '=' CONST)* ';' #varInitConst
    | 'int' VAR '=' expr (',' VAR '=' expr)* ';' #varInitExpr
    | 'int' VAR '=' affectation_var #varInitAffect
    ;


expr: '(' expr ')' #par
    | call_function #function
    | '-' expr #unaire
    | expr OP=('*' | '/' | '%') expr #multdivmod
    | expr OP=('+' | '-') expr #plusmoins
    | expr OP=('<' | '>') expr #lessgreater
    | expr OP=('==' | '!=') expr #equalnotequal
    | expr '&' expr #binaryAND
    | expr '^' expr #binaryXOR
    | expr '|' expr #binaryOR
    | '!' expr #logiNot
    | VAR #var
    | CONST #const
    ;


call_function : VAR  '(' (expr (',' expr)*)? ')' ;


RETURN : 'return' ;
//FUNCTION_NAME :  'putchar'| 'getchar' ;
CONST : [0-9]+ | '\'' (.? | '\\'.) '\'';
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
//KEYWORD : 'auto'|'break'|'case'|'char'|'const'|'continue'|'default'|'do'|'double'|'else'|'enum'|'extern'|'float'|'for'|'goto'|'if'|'int'|'long'|'register'|'return'|'short'|'signed'|'sizeof'|'static'|'struct'|'switch'|'typedef'|'union'|'unsigned'|'void'|'volatile'|'while' ;
VAR : [a-zA-Z_][a-zA-Z0-9_]*;

