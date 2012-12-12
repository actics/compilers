#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>

class ArithmeticExpressionAssembler {
    private:
        static int expression_count;
        std::ostringstream code;
        void action(const char*);
        
    public:
        ArithmeticExpressionAssembler();
        ~ArithmeticExpressionAssembler();
        std::string getExpressionName();
        std::string getCode();
        void newExpression();
        void add();
        void sub();
        void mul();
        void div();
        void pow();
        void changeSign(int);
        void pushConst(double);
        void pushVariable(int);
};
