grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{'(var_stmt)* return_stmt '}' ;

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
    | '-' expr #unaire
    | expr OP=('*' | '/' | '%') expr #multdivmod
    | expr OP=('+' | '-') expr #plusmoins
    | expr OP=('<' | '>') expr #lessgreater
    | expr OP=('==' | '!=') expr #equalnotequal
    | VAR #var
    | CONST #const
    ;


RETURN : 'return' ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
VAR : [a-zA-Z_][a-zA-Z0-9_]* ;
