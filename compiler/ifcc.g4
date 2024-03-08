grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' var_stmt* return_stmt '}'  ;

var_stmt: var_declaration
    | var_affectation
    | var_initialisation ;
return_stmt: RETURN CONST ';' ;

var_declaration: 'int' VARNAME ';' ;
var_affectation: VARNAME '=' (CONST | VARNAME) ';' ;
var_initialisation: 'int' VARNAME '=' (CONST | VARNAME) ';' ;

RETURN : 'return' ;
VARNAME: [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
