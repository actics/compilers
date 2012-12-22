tree grammar   ArithmeticInterpreterTree;

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
    #include "./../../assembler/ArithmeticAssembler.hpp"
    #include "./../../assembler/CompilerAssembler.hpp"
}

@members {
    ArithmeticAssembler expression;
    CompilerAssembler interpreter;
    
    void pushConstant(pANTLR3_BASE_TREE tree) {
        double value = atof((const char*) tree->toString(tree)->chars);
        expression.pushConstant(value);
    }
    
    void pushVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        int variable_count = interpreter.getVariableCount(varname);
        expression.pushVariable(variable_count);
    }
    
    void assigmentVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        interpreter.assigmentVariable(varname, expression.getCode());
        interpreter.refreshStack( expression.getCount() );
        expression.newExpression();
    }
    
    void printExpression() {
        interpreter.printExpression(expression.getCode());
        interpreter.refreshStack( expression.getCount() );
        expression.newExpression();
    }
    
    void scanVariable(pANTLR3_BASE_TREE tree) {
        std::string varname((const char*) tree->toString(tree)->chars);
        interpreter.scanVariable(varname);
    }
    
}

axiom returns [std::string code] : (line)+ {code = interpreter.getCode();};

code_block:
    (line)+ ; 

line
    : ^(ASSIGMENT VARIABLE arith_expr) { assigmentVariable($VARIABLE); }
    | ^(PRINT_KEYW arith_expr)         { printExpression();            }
    | ^(SCAN_KEYW VARIABLE)            { scanVariable($VARIABLE);      }
    | ^(WHILE_KEYWORD logic_expr code_block)
    ;

arith_expr
    : ^(PLS arith_expr ( arith_expr { expression.add(); } )? )
    | ^(MNS arith_expr {expression.changeSign(0);} ( arith_expr { expression.sub(); } )? )
    | ^(MLP arith_expr arith_expr) { expression.mul(); }
    | ^(DIV arith_expr arith_expr) { expression.div(); }
    | ^(PWR arith_expr arith_expr) { expression.pow(); }
    | INT      { pushConstant($INT);      }
    | FLOAT    { pushConstant($FLOAT);    }
    | VARIABLE { pushVariable($VARIABLE); }
    ;

logic_expr
    : ^(OR  logic_expr logic_expr)
    | ^(AND logic_expr logic_expr)
    | ^(LSS arith_expr arith_expr)
    | ^(LSQ arith_expr arith_expr)
    | ^(GRT arith_expr arith_expr)
    | ^(GRQ arith_expr arith_expr)
    | ^(EQL arith_expr arith_expr)
    | ^(NEQ arith_expr arith_expr)
    ;
