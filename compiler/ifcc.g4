grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' var_stmt* return_stmt '}'  ;

var_stmt: var_declaration
    | var_affectation
    | var_initialisation ;
return_stmt: RETURN expression ';' ;

var_declaration: 'int' VARNAME (',' VARNAME)* ';' ;
var_affectation: VARNAME '=' expression ';' ;
var_initialisation: 'int' VARNAME '=' expression ';' ;

expression: '(' expression ')' #par
        | expression OP=('*'|'/') expression #multdiv
        | expression OP=('+'|'-') expression #addsub
        | CONST #const
        | VARNAME #var
        ;

RETURN : 'return' ;
VARNAME: [a-zA-Z_][a-zA-Z_0-9]* ;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
