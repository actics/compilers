#include "polish.h"
    
enum {
    LEFT_BRACKET_PRIORITY,
    RIGHT_BRACKET_RIORITY,
    PLUS_MINUS_PRIORITY,
    MULTIPLY_DIVISION_PRIORITY,
    POWER_PRIORITY,
    OPERAND_PRIORITY,
};
    
static char prioritize(std::string token) {
    if (token == "(")                  return LEFT_BRACKET_PRIORITY;
    if (token == ")")                  return RIGHT_BRACKET_RIORITY;
    if (token == "+" || token ==  "-") return PLUS_MINUS_PRIORITY;
    if (token == "*" || token ==  "/") return MULTIPLY_DIVISION_PRIORITY;
    if (token == "^")                  return POWER_PRIORITY;
    
    return OPERAND_PRIORITY;
}

Polish::Polish() {
    this->operation_stack = std::stack<PriorityOperation>();
    this->result_vector   = std::vector<std::string>();
}

Polish::~Polish() {}

void Polish::printNotation(std::vector<std::string> notation) {
    std::vector<std::string>::iterator token;
    for (token=notation.begin(); token != notation.end(); token++) {
        std::cout<<*token<<' ';
    }
}

void Polish::moveStackTop() {
    this->result_vector.push_back(this->operation_stack.top().first);
    this->operation_stack.pop();
}

void Polish::eatToken(unsigned char* token_chars) {
    std::string token = std::string(reinterpret_cast<char*>(token_chars));
    std::cout<<token<<'\n';
    char priority = prioritize(token);
    
    if (priority == OPERAND_PRIORITY) {
        this->result_vector.push_back(token);
        return;
    }
    
    if (token != "(") {
        while (!this->operation_stack.empty()) {
            if (this->operation_stack.top().second >= priority)
                this->moveStackTop();
            else break;
        }
    }
    
    if (token != ")")
        this->operation_stack.push(PriorityOperation(token, priority));
    else
        this->operation_stack.pop();
}

std::vector<std::string> Polish::getNotation() {
    while (!this->operation_stack.empty())
        this->moveStackTop();
    
    std::vector<std::string> ret = std::vector<std::string>(this->result_vector);
    this->result_vector.clear();
    
    return ret;
}
