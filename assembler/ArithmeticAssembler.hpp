#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

class ArithmeticAssembler {
    private:
       int count;
       int max_count;
       std::ostringstream code;
       void action(std::string);

    public:
        ArithmeticAssembler();
        ~ArithmeticAssembler();
        void checkMaximum();
        int getCount();
        void newExpression();
        std::string getCode();
        void pushConstant(double);
        void pushVariable(int);
        void changeSign(int);
        void add();
        void sub();
        void mul();
        void div();
        void pow();
};
