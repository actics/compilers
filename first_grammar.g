grammar first_grammar;

WS  : ( ' '| '\t'| '\r'| '\n') {$channel=HIDDEN;};

PM  : '+'|'-';
MD  : '*'|'/';

INT : ('0'..'9')+;
FLOAT : INT '.' INT* EXP? | '.' INT EXP? | INT EXP;
fragment EXP : ('e'|'E') PM? INT;

axiom : expr EOF;
expr  : PM? term (PM term)*;
term  : pow (MD pow)*;
pow   : atom (options{greedy=true;}:'^' pow)*;
atom  : INT | FLOAT | '('expr')';

