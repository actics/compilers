#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>

class CompilerAssembler {
    private:
        int stack_count;
        int variable_count;
        std::ostringstream code;
        std::map<std::string, int> variables;
        
    public:
        CompilerAssembler();
        ~CompilerAssembler();
        int getVariableCount(std::string);
        void refreshStack(const int&);
        void scanVariable(std::string);
        void assigmentVariable(std::string, std::string);
        void printExpression(std::string);
        std::string getCode();
};
