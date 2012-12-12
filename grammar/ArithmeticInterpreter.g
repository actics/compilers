grammar ArithmeticInterpreter;

options { 
    output = AST;
    language = C;
}

WS  : ( ' '| '\t'| '\r') {$channel=HIDDEN;};

LBR :  '(' ;
RBR :  ')' ;
PLS :  '+' ;
MNS :  '-' ;
MLP :  '*' ;
DIV :  '/' ;
PWR :  '^' ;

ASSIGMENT : '=' ;
PRINT_KEYW : 'print' ;
SCAN_KEYW  : 'scan' ;

SEP : '\n' | ';' ;

INT :  ('0'..'9')+;

FLOAT : INT '.' INT* EXP? | '.' INT EXP? | INT EXP;
fragment EXP : ('e'|'E') (PLS | MNS)? INT;

VARIABLE : SS (SS | '0'..'9')* ;
fragment SS : 'a'..'z' | 'A'..'Z' | '_' ;

//options{greedy=true;}:

axiom : lines EOF! ;
lines : line SEP! lines? ;
line  : (def_var | print_expr | scan_expr)? ;

def_var    : VARIABLE ASSIGMENT^ arith_expr ;
print_expr : PRINT_KEYW^ arith_expr ;
scan_expr  : SCAN_KEYW^ VARIABLE ;

arith_expr : ((PLS|MNS)^)? term ((PLS|MNS)^ term)*;
term       : power ((MLP|DIV)^ power )*;
power      : atom  (options{greedy=true;}: PWR^ power )*;

atom : INT | FLOAT | VARIABLE | LBR arith_expr RBR -> ^(arith_expr);

