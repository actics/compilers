#pragma once

#include <string>
#include <sstream>

class ywLogicalAssembler {
    private:
        static int expression_count;
        std::ostringstream code;

    public:
        LogicalAssembler();
        ~LogicalAssembler();
        std::string getExpressionName();
        std::string getCode();
        void newExpression();
        void less();
        void lessOrEqual();
        void great();
        void greatOrEqual();
        void equal();
        void noEqual();
};
