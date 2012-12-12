#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>

class ArithmeticExpressionAssembler {
    private:
        int stack_count;
        std::ostringstream code;
        int action(const char*, int, int);
        
    public:
        static int expression_count;
        ArithmeticExpressionAssembler();
        ~ArithmeticExpressionAssembler();
        std::string getExpressionName();
        std::string getCode();
        void newExpression();
        int add(int, int);
        int sub(int, int);
        int mul(int, int);
        int div(int, int);
        int pow(int, int);
        int changeSign(int);
        int pushConst(double);
        int pushVariable(int);
};
