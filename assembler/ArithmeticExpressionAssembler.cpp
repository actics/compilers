#include "ArithmeticExpressionAssembler.hpp"

int ArithmeticExpressionAssembler::expression_count = 0;

ArithmeticExpressionAssembler::~ArithmeticExpressionAssembler() {}

ArithmeticExpressionAssembler::ArithmeticExpressionAssembler() {
    ArithmeticExpressionAssembler::expression_count++;
}

void ArithmeticExpressionAssembler::newExpression() {
    this->code.str(std::string());
    ArithmeticExpressionAssembler::expression_count++;
}

std::string ArithmeticExpressionAssembler::getExpressionName() {
    std::ostringstream name;
    name << "arithmetic_expression_" << ArithmeticExpressionAssembler::expression_count;
    return name.str();
}

std::string ArithmeticExpressionAssembler::getCode() {
    std::ostringstream head;
    head << this->getExpressionName() << ":" << std::endl;
    head << "push rbp" << std::endl;
    head << "mov rbp, rsp" << std::endl;
    
    std::ostringstream tail;
    tail << "mov rax, qword [rsp]" << std::endl;
    tail << "mov rsp, rbp"  << std::endl;
    tail << "pop rbp"       << std::endl;
    tail << "ret"           << std::endl;
    
    return head.str() + this->code.str() + tail.str();
}

void ArithmeticExpressionAssembler::action(const char *action) {
    this->code << "fld qword [rsp+8]" << std::endl;
    this->code << "f" << action << " qword [rsp]" << std::endl;
    this->code << "fstp qword [rsp+8]" << std::endl;
    this->code << "add rsp, 8" << std::endl;
}

void ArithmeticExpressionAssembler::pow() {
    this->code << "movsd xmm0, qword [rsp+8]" << std::endl;
    this->code << "movsd xmm1, qword [rsp]" << std::endl;
    this->code << "call pow" << std::endl;
    this->code << "movsd qword [rsp+8], xmm0" << std::endl;
    this->code << "add rsp, 8" << std::endl;
}

void ArithmeticExpressionAssembler::changeSign(int target) {
    this->code << "fld qword [rsp+8*" << target << "]" << std::endl;
    this->code << "fchs" << std::endl;
    this->code << "fstp qword [rsp+8*" << target << "]" << std::endl;
}

void ArithmeticExpressionAssembler::pushConst(double value) {
    char buffer[20];
    sprintf(buffer, "%#llx", *(long long int *) &value);
    this->code << "mov rax, " << buffer << std::endl;
    this->code << "push rax" << std::endl;
}

void ArithmeticExpressionAssembler::pushVariable(int variable_count) {
    this->code << "mov r15, [variables+8*" << variable_count << "]" << std::endl;
    this->code << "push r15" << std::endl;
}

void ArithmeticExpressionAssembler::add() {
    this->action("add");
}

void ArithmeticExpressionAssembler::sub() {
    this->changeSign(1);
    this->action("sub");
}

void ArithmeticExpressionAssembler::mul() {
    this->action("mul");
}

void ArithmeticExpressionAssembler::div() {
    this->action("div");
}

