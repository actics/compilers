tree grammar ArithmeticInterpreterTree;

options {
    tokenVocab=ArithmeticInterpreter;
    ASTLabelType=pANTLR3_BASE_TREE;
    language = C;
}

@includes {
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <string>
    #include <map>
}

@members {
    std::map<std::string, float> variables = std::map<std::string, float>();
    
    static float get_var(pANTLR3_BASE_TREE t) {
        std::string tname = std::string((const char *) t->toString(t)->chars);
        std::map<std::string, float>::iterator tname_it = variables.find(tname);
        if (tname_it == variables.end()) {
            printf("undefined variable \%s\n", tname.c_str());
            exit(1);
        } 
        else return tname_it->second;
    }
    
    static void add_var(pANTLR3_BASE_TREE t, float v) {
        variables[std::string((const char *) t->toString(t)->chars)] = v;
    }
}

axiom
    : ^(ASSIGMENT VARIABLE a=arith_expr) {add_var((pANTLR3_BASE_TREE) $VARIABLE, a);}
    | ^(PRINT_KEYW a=arith_expr)         {printf("\%f\n", a);}
    ;

arith_expr returns [float value]
    : ^(PLS e1=arith_expr {value  = e1;} (e2=arith_expr {value += e2;})?)
    | ^(MNS e1=arith_expr {value -= e1;} (e2=arith_expr {value = -value - e2;})?)
    | ^(MLP e1=arith_expr e2=arith_expr) {value = e1 * e2;}
    | ^(DIV e1=arith_expr e2=arith_expr) {value = e1 / e2;}
    | ^(PWR e1=arith_expr e2=arith_expr) {value = pow(e1, e2);}
    | INT   {pANTLR3_BASE_TREE a = (pANTLR3_BASE_TREE)$INT;   value = atof((const char*) a->toString(a)->chars);}
    | FLOAT {pANTLR3_BASE_TREE a = (pANTLR3_BASE_TREE)$FLOAT; value = atof((const char*) a->toString(a)->chars);}
    | VARIABLE {value = get_var((pANTLR3_BASE_TREE) $VARIABLE);}
    ;

