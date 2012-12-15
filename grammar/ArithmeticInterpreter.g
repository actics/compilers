grammar ArithmeticInterpreter;

options { 
    output = AST;
    language = C;
}
//options{greedy=true;}:

axiom : lines EOF! ;
lines : line (SEP! line)* ;
line  : (def_var | print_expr | scan_expr | while_expr )? ;

def_var    : VARIABLE ASSIGMENT^ arith_expr ;
print_expr : PRINT_KEYW^ arith_expr ;
scan_expr  : SCAN_KEYW^ VARIABLE ;

arith_expr : ((PLS | MNS)^)? term ((PLS | MNS)^ term)*;
term       : power ((MLP | DIV)^ power )*;
power      : atom  (options{greedy=true;}: PWR^ power )*;
atom       : INT | FLOAT | VARIABLE | LBR! arith_expr^ RBR!;

logic_expr : cmp_expr ((OR | AND)^ cmp_expr)*;
cmp_expr   : (arith_expr (LSS | LSQ | GRT | GRQ | EQL | NEQ))=> arith_expr (LSS | LSQ | GRT | GRQ | EQL | NEQ)^ arith_expr
           | LBR! logic_expr^ RBR!
           ;

code_block : LBB! lines RBB! | line ;

while_expr : WHILE_KEYWORD^ logic_expr code_block ;

WS  : ( ' '| '\t'| '\r') {$channel=HIDDEN;};

WHILE_KEYWORD : 'while';

LBR :  '(' ;
RBR :  ')' ;

LBB :  '{' ; 
RBB :  '}' ;

PLS :  '+' ;
MNS :  '-' ;
MLP :  '*' ;
DIV :  '/' ;
PWR :  '^' ;

LSS :  '<'  ;
LSQ :  '<=' ;
GRT :  '>'  ;
GRQ :  '>=' ;
EQL :  '==' ;
NEQ :  '!=' ;
AND :  '&&' ;
OR  :  '||' ;
NOT :  '!'  ;

ASSIGMENT : '=' ;
PRINT_KEYW : 'print' ;
SCAN_KEYW  : 'scan' ;

SEP : '\n' | ';' ;

INT :  ('0'..'9')+;

FLOAT : INT '.' INT* EXP? | '.' INT EXP? | INT EXP;
fragment EXP : ('e'|'E') (PLS | MNS)? INT;

VARIABLE : SS (SS | '0'..'9')* ;
fragment SS : 'a'..'z' | 'A'..'Z' | '_' ;
