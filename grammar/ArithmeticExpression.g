grammar ArithmeticExpression;

options {
    language = C;
}

@parser::includes {
    #include <stdlib.h>
    #include <math.h>
    #include <stdio.h>
    
    #include <map>
    #include <string>
}

@parser::members {
    std::map<std::string, float> variables = std::map<std::string, float>();
    
    float get_var(pANTLR3_COMMON_TOKEN t) {
        std::string tname = std::string((const char *) t->getText(t)->chars);
        std::map<std::string, float>::iterator tname_it = variables.find(tname);
        if (tname_it == variables.end()) {
            printf("undefined variable \%s\n", tname.c_str());
            exit(1);
        } 
        else return tname_it->second;
    }
}

WS  : ( ' '| '\t'| '\r' ) {$channel=HIDDEN;};

LBR :  '(' ;
RBR :  ')' ;
PLS :  '+' ;
MNS :  '-' ;
MLP :  '*' ;
DIV :  '/' ;
PWR :  '^' ;

ASSIGMENT : '=' ;
PRINT_KEYW : 'print' ;

SEP : '\n' | ';' ;

INT :  ('0'..'9')+;
FLOAT :  INT '.' INT* EXP? 
      |  '.' INT EXP?
      |  INT EXP
      ;
fragment EXP : ('e'|'E') (PLS | MNS)? INT;

VARIABLE : START_SYMBOL (START_SYMBOL | '0'..'9')* ;
fragment START_SYMBOL : 'a'..'z' | 'A'..'Z' | '_' ;

axiom : lines EOF ;
lines : line SEP lines? ;
line  : (def_var | print_expr)? ;

def_var : VARIABLE ASSIGMENT a=arith_expr {variables[std::string((const char *)$VARIABLE.text->chars)] = a;};
print_expr : PRINT_KEYW a=arith_expr {printf("\%f\n", a);};

arith_expr returns [float value]
    :   PLS? t1=term              {$value = t1;}
        (
            PLS t2=term           {$value += t2;}
        |
            MNS t2=term           {$value -= t2;}
        )* 
    |   
        MNS t1=term               {$value = -t1;}
        (
            PLS t2=term           {$value += t2;}
        |
            MNS t2=term           {$value -= t2;}
        )* 
    ;

term returns [float value]
    :   p1=power                  {$value = p1;}
        (                         options{greedy=true;}:
            (MLP p2=power         {$value *= p2;})
        |
            (DIV p2=power         {$value /= p2;})
        )*
    ;

power returns [float value]
    :   a=atom                    {$value = a;}
        (                         options{greedy=true;}:
           PWR p=power            {$value = pow($value, p);}
        )*
    ;

atom returns [float value]  
    :   INT                       {$value = atof((const char *)$INT.text->chars);}
    |   FLOAT                     {$value = atof((const char *)$FLOAT.text->chars);}
    |   VARIABLE                  {$value = get_var($VARIABLE);}
    |   LBR a=arith_expr RBR      {$value = a;}
    ;

