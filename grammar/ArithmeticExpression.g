grammar ArithmeticExpression;

options {
    language = C;
}

@parser::includes {
    #include <stdio.h>
    static void fg_print(pANTLR3_COMMON_TOKEN t) {
        printf("\%s ", t->getText(t)->chars);
    }
    static void fg_print(const char *t) {
        printf("\%s ", t);
    }
}

WS  : ( ' '| '\t'| '\r'| '\n') {$channel=HIDDEN;};

LBR :  '(' ;
RBR :  ')' ;
PLS :  '+' ;
MNS :  '-' ;
MLP :  '*' ;
DIV :  '/' ;
PWR :  '^' ;

INT :  ('0'..'9')+;

FLOAT :  INT '.' INT* EXP? 
      |  '.' INT EXP?
      |  INT EXP
      ;
fragment EXP : ('e'|'E') (PLS | MNS)? INT;

//options{greedy=true;}:

axiom : 
         expr EOF
      ;

expr  :  term 
         (
             a=(PLS | MNS) 
             term             {fg_print($a);}
         )* 
      |  
         s=(PLS | MNS)        {fg_print("0");} 
         term                 {fg_print($s);} 
         (
             a=(PLS | MNS) 
             term             {fg_print($a);}
         )*
      ;
   
term  :  pow 
         (                    options{greedy=true;}:
             m=(MLP | DIV) 
             pow              {fg_print($m);}
         )*
      ;
        
pow   :  atom
         (                    options{greedy=true;}:
             PWR pow          {fg_print("^");}
         )*
      ;
        
atom  :  INT                  {fg_print($INT);}
      |  FLOAT                {fg_print($FLOAT);}
      |  LBR expr RBR
      ;

