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
    #include "./../../assembler/ArithmeticExpressionAssembler.hpp"
    #include "./../../assembler/ArithmeticAssembler.hpp"
}

@members {
    ArithmeticExpressionAssembler expression;
    ArithmeticInterpreterAssembler interpreter;
    
    void pushConst(pANTLR3_BASE_TREE tree) {
        double value = atof((const char*) tree->toString(tree)->chars);
        expression.pushConst(value);
    }
    
    void pushVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        int variable_count = interpreter.getVariableCount(varname);
        expression.pushVariable(variable_count);
    }
    
    void assigmentVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        interpreter.assigmentVariable(varname, expression.getExpressionName());
        interpreter.setExpression(expression.getCode());
        expression.newExpression();
    }
    
    void printExpression() {
        interpreter.printExpression(expression.getExpressionName());
        interpreter.setExpression(expression.getCode());
        expression.newExpression();
    }
    
    void scanVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        interpreter.scanVariable(varname);
    }
    
}

axiom returns [std::string code] : (line)+ {code = interpreter.getCode();};

line
    : ^(ASSIGMENT VARIABLE arith_expr) { assigmentVariable($VARIABLE); }
    | ^(PRINT_KEYW arith_expr)         { printExpression();            }
    | ^(SCAN_KEYW VARIABLE)            { scanVariable($VARIABLE);      }
    ;

arith_expr
    : ^(PLS arith_expr ( arith_expr { expression.add(); } )? )
    | ^(MNS arith_expr {expression.changeSign(0);} ( arith_expr { expression.sub(); } )? )
    | ^(MLP arith_expr arith_expr) { expression.mul(); }
    | ^(DIV arith_expr arith_expr) { expression.div(); }
    | ^(PWR arith_expr arith_expr) { expression.pow(); }
    | INT      { pushConst($INT);         }
    | FLOAT    { pushConst($FLOAT);       }
    | VARIABLE { pushVariable($VARIABLE); }
    ;

