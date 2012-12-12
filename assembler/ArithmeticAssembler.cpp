#include "ArithmeticAssembler.hpp"

ArithmeticInterpreterAssembler::~ArithmeticInterpreterAssembler() {}

ArithmeticInterpreterAssembler::ArithmeticInterpreterAssembler() {
    this->stack_count = 0;
}

int ArithmeticInterpreterAssembler::getVariableCount(std::string varname) {
    std::map<std::string, int>::iterator varname_iterator = this->variables.find(varname);
    if (varname_iterator != this->variables.end()) {
        return varname_iterator->second;
    }
    else {
        return -1;
    }
}

void ArithmeticInterpreterAssembler::setExpression(std::string expression) {
    this->expressions += expression;
}

std::string ArithmeticInterpreterAssembler::getCode() {
    std::ostringstream head;
    head << "[bits 64]"             << std::endl;
    head << "global _start"         << std::endl;
    head << "extern scanf"          << std::endl;
    head << "extern printf"         << std::endl;
    head << "extern pow"            << std::endl;
    head << "section .bss"          << std::endl;
    head << "variables resq " << this->stack_count << std::endl;
    head << "section .data"         << std::endl;
    head << "scanf_format db  '\%lf', 0" << std::endl;
    head << "printf_format db '\%lf', 10, 0" << std::endl;
    head << "section .text"         << std::endl;
    head << this->expressions;
    head << "_start:"               << std::endl;
    head << "mov rbp, rsp"          << std::endl;
    head << "sub rsp, 0x20"         << std::endl;
    
    std::ostringstream tail;
    tail << "mov rax, 60" << std::endl;
    tail << "mov rdi, 0"  << std::endl;
    tail << "syscall"     << std::endl;
    
    return head.str() + this->code.str() + tail.str();
}

void ArithmeticInterpreterAssembler::scanVariable(std::string varname) {
    int variable_count = this->getVariableCount(varname);
    if (variable_count < 0) {
        this->variables[varname] = this->stack_count;
        variable_count = this->stack_count++;
    }
    this->code << "lea rsi, [variables+8*" << variable_count << "]" << std::endl;
    this->code << "mov rdi, scanf_format" << std::endl;
    this->code << "xor rax, rax" << std::endl;
    this->code << "call scanf"   << std::endl;
}

void ArithmeticInterpreterAssembler::assigmentVariable(std::string varname, std::string function_name) {
    int variable_count = this->getVariableCount(varname);
    if (variable_count < 0) {
        this->variables[varname] = this->stack_count;
        variable_count = this->stack_count++;
    }
    this->code << "call " << function_name << std::endl;
    this->code << "mov qword [variables+8*" << variable_count << "], rax" << std::endl;
}

void ArithmeticInterpreterAssembler::printExpression(std::string function_name) {
    this->code << "call " << function_name << std::endl;
    this->code << "mov [rsp-8], rax" << std::endl;
    this->code << "movsd xmm0, [rsp-8]" << std::endl;
    this->code << "mov rdi, printf_format" << std::endl;
    this->code << "mov rax, 1" << std::endl;
    this->code << "call printf" << std::endl;
}

