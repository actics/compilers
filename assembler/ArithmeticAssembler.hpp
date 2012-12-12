#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>

class ArithmeticInterpreterAssembler {
    private:
        int stack_count;
        std::string expressions;
        std::ostringstream code;
        std::map<std::string, int> variables;
        
    public:
        ArithmeticInterpreterAssembler();
        ~ArithmeticInterpreterAssembler();
        void setExpression(std::string);
        int getVariableCount(std::string);
        void scanVariable(std::string);
        void assigmentVariable(std::string, std::string);
        void printExpression(std::string);
        std::string getCode();
};
