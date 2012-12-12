tree grammar ArithmeticInterpreterTree;

options {
    tokenVocab = ArithmeticInterpreter;
    ASTLabelType = pANTLR3_BASE_TREE;
    language = C;
}

@includes {
    #include <iostream>
    #include <cstdlib>
    #include <cmath>
    #include <string>
    #include <sstream>
    #include <map>
    #include "./../../ArithmeticExpressionAssembler.hpp"
    #include "./../../ArithmeticInterpreterAssembler.hpp"
}

@members {
    ArithmeticExpressionAssembler expression;
    ArithmeticInterpreterAssembler interpreter;
    
    int pushConst(pANTLR3_BASE_TREE tree) {
        double value = atof((const char*) tree->toString(tree)->chars);
        return expression.pushConst(value);
    }
    
    int pushVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        int variable_count = interpreter.getVariableCount(varname);
        return expression.pushVariable(variable_count);
    }
    
    void assigmentVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        interpreter.assigmentVariable(varname, expression.getExpressionName());
        interpreter.setExpression(expression.getCode());
        expression.newExpression();
    }
    
    void printExpression() {
        
    }
    
    void scanVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        interpreter.scanVariable(varname);
    }
    
}

axiom : (line)+ {std::cout<<interpreter.getCode();};

line
    : ^(ASSIGMENT VARIABLE a=arith_expr) { assigmentVariable($VARIABLE); }
    | ^(PRINT_KEYW a=arith_expr)         { printExpression();            }
    | ^(SCAN_KEYW VARIABLE)              { scanVariable($VARIABLE);      }
    ;

arith_expr returns [int pos]
    : ^(PLS e1=arith_expr {pos = e1;} (e2=arith_expr { pos = expression.add(pos, e2); })?)
    | ^(MNS e1=arith_expr {pos = expression.changeSign(e1);} (e2=arith_expr { pos = expression.sub(pos, e2); })?)
    | ^(MLP e1=arith_expr e2=arith_expr) {pos = expression.mul(e1, e2);}
    | ^(DIV e1=arith_expr e2=arith_expr) {pos = expression.div(e1, e2);}
    | ^(PWR e1=arith_expr e2=arith_expr) {pos = expression.pow(e1, e2);}
    | INT   { pos = pushConst($INT); }
    | FLOAT { pos = pushConst($FLOAT); }
    | VARIABLE { pos = pushVariable($VARIABLE);}
    ;

