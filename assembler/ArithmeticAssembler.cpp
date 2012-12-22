#include "ArithmeticAssembler.hpp"

ArithmeticAssembler::~ArithmeticAssembler() {}

ArithmeticAssembler::ArithmeticAssembler() {
    this->count = 0;
    this->max_count = 0;
}

void ArithmeticAssembler::checkMaximum() {
    if (this->count > this->max_count)
        this->max_count = this->count;
}

int ArithmeticAssembler::getCount() {
    return this->max_count;
}

void ArithmeticAssembler::newExpression() {
    this->count = 0;
    this->max_count = 0;
    this->code.str(std::string());
}

std::string ArithmeticAssembler::getCode() {
    return this->code.str();
}

void ArithmeticAssembler::action(std::string action) {
    this->count--;
    this->code << "fld  qword [rsp+8*" << this->count-1 << "]" << std::endl;
    this->code << "f" << action << " qword [rsp+8*" << this->count << "]" << std::endl;
    this->code << "fstp qword [rsp+8*" << this->count-1 << "]" << std::endl;
}

void ArithmeticAssembler::pushConstant(double value) {
    char buffer[20];
    sprintf(buffer, "%#llx", *(long long int *) &value);
    this->code << "mov rax, " << buffer << std::endl;
    this->code << "mov qword [rsp+8*" << this->count++ << "], rax" << std::endl;
    this->checkMaximum();
}

void ArithmeticAssembler::pushVariable(int variable_count) {
    this->code << "mov r15, [variables+8*" << variable_count << "]" << std::endl;
    this->code << "mov qword [rsp+8*" << this->count++ << "], r15" << std::endl;
    this->checkMaximum();
}

void ArithmeticAssembler::changeSign(int target) {
    this->code << "fld  qword [rsp+8*" << this->count - target - 1 << "]" << std::endl;
    this->code << "fchs" << std::endl;
    this->code << "fstp qword [rsp+8*" << this->count - target - 1 << "]" << std::endl;
}

void ArithmeticAssembler::add() {
    this->action("add");
}

void ArithmeticAssembler::sub() {
    this->changeSign(1);
    this->action("sub");
}

void ArithmeticAssembler::mul() {
    this->action("mul");
}

void ArithmeticAssembler::div() {
    this->action("div");
}

void ArithmeticAssembler::pow() {
    this->count--;
    this->code << "movsd xmm0, qword [rsp+8*" << this->count-1 << "]" << std::endl;
    this->code << "movsd xmm1, qword [rsp+8*" << this->count   << "]" << std::endl;
    this->code << "call pow" << std::endl;
    this->code << "movsd qword [rsp+8*" << this->count-1 << "], xmm0" << std::endl;
}

