#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <string>

class Polish {
private:
    typedef std::pair<std::string, int> PriorityOperation;
    std::stack<PriorityOperation> operation_stack;
    std::vector<std::string> result_vector;
    void moveStackTop();
public:
    Polish();
    ~Polish();
    void printNotation(std::vector<std::string> notation);
    void eatToken(unsigned char* token_chars);
    std::vector<std::string> getNotation();
};
