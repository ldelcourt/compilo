grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' var_declaration* return_stmt '}'  ;

var_declaration: 'int' VARNAME '=' (CONST | VARNAME) ';' ;
return_stmt: RETURN CONST ';' ;

RETURN : 'return' ;
VARNAME: [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
