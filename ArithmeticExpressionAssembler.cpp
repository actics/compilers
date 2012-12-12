#include "ArithmeticExpressionAssembler.hpp"

int ArithmeticExpressionAssembler::expression_count = -1;

ArithmeticExpressionAssembler::~ArithmeticExpressionAssembler() {}

ArithmeticExpressionAssembler::ArithmeticExpressionAssembler() {
    this->stack_count = 0;
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
    head << "sub rsp, 8*" << this->stack_count << std::endl;
    
    std::ostringstream tail;
    tail << "mov rax, qword [rsp+8*" << this->stack_count-1 << "]" << std::endl;
    tail << "pop rbp"       << std::endl;
    tail << "push rsp, rbp" << std::endl;
    tail << "ret"           << std::endl;
    
    return head.str() + this->code.str() + tail.str();
}

void ArithmeticExpressionAssembler::newExpression() {
    this->code.str(std::string());
    this->stack_count = 0;
    ArithmeticExpressionAssembler::expression_count++;
}

int ArithmeticExpressionAssembler::action(const char *action, int a, int b) {
    this->code << "fld qword [rsp+8*" << a << "]" << std::endl;
    this->code << "f" << action << " qword [rsp+8*" << a << "]" << std::endl;
    this->code << "fstp qword [rsp+8*" << this->stack_count << "]" << std::endl;
    return this->stack_count++;
}

int ArithmeticExpressionAssembler::add(int a, int b) {
    return this->action("add", a, b);
}

int ArithmeticExpressionAssembler::sub(int a, int b) {
    return this->action("sub", a, b);
}

int ArithmeticExpressionAssembler::mul(int a, int b) {
    return this->action("mul", a, b);
}

int ArithmeticExpressionAssembler::div(int a, int b) {
    return this->action("div", a, b);
}

int ArithmeticExpressionAssembler::pow(int a, int b) {
    this->code << "movsd xmm0, qword [rsp+8*" << a << "]" << std::endl;
    this->code << "movsd xmm1, qword [rsp+8*" << b << "]" << std::endl;
    this->code << "call pow" << std::endl;
    this->code << "movsd qword [rsp+8*" << this->stack_count << "], xmm0" << std::endl;
    return this->stack_count++;
}

int ArithmeticExpressionAssembler::changeSign(int a) {
    this->code << "fld qword [rsp+8*" << a << "]" << std::endl;
    this->code << "fchs" << std::endl;
    this->code << "fstp qword [rsp+8*" << this->stack_count << "]" << std::endl;
    return this->stack_count++;
}

int ArithmeticExpressionAssembler::pushConst(double value) {
    this->code << "mov qword [rsp+8*" << this->stack_count << "], ";
    this->code << std::hex << value << std::dec << std::endl;
    return this->stack_count++;
}

int ArithmeticExpressionAssembler::pushVariable(int variable_count) {
    this->code << "mov r15, [variables+8*" << variable_count << "]" << std::endl;
    this->code << "mov [rsp+8*" << this->stack_count << "], " << "r15" << std::endl;
    //Возможна оптимизация созданием мапа локально сохраненных переменных
    return this->stack_count++;
}

