#include "CompilerAssembler.hpp"

CompilerAssembler::~CompilerAssembler() {}

CompilerAssembler::CompilerAssembler() {
    this->stack_count    = 0;
    this->variable_count = 0;
}

void CompilerAssembler::refreshStack(const int &stack_count) {
    if (stack_count > this->stack_count)
        this->stack_count = stack_count;
}

int CompilerAssembler::getVariableCount(std::string varname) {
    std::map<std::string, int>::iterator varname_iterator = this->variables.find(varname);
    if (varname_iterator != this->variables.end()) {
        return varname_iterator->second;
    }
    else {
        return -1;
    }
}

std::string CompilerAssembler::getCode() {
    std::ostringstream head;
    head << "[bits 64]"             << std::endl;
    head << "global _start"         << std::endl;
    head << "extern scanf"          << std::endl;
    head << "extern printf"         << std::endl;
    head << "extern pow"            << std::endl;
    head << "section .bss"          << std::endl;
    head << "variables resq " << this->variable_count << std::endl;
    head << "section .data"                  << std::endl;
    head << "scanf_format db  '\%lf', 0"     << std::endl;
    head << "printf_format db '\%lf', 10, 0" << std::endl;
    head << "section .text"         << std::endl;
    head << "_start:"               << std::endl;
    head << "mov rbp, rsp"          << std::endl;
    int stack_count  = this->stack_count;
    stack_count      += this->stack_count & 1 ? 1 : 0;
    head << "sub rsp, 8*" << stack_count << std::endl;
    
    std::ostringstream tail;
    tail << "mov rax, 60" << std::endl;
    tail << "mov rdi, 0"  << std::endl;
    tail << "syscall"     << std::endl;
    
    return head.str() + this->code.str() + tail.str();
}

void CompilerAssembler::scanVariable(std::string varname) {
    int variable_number = this->getVariableCount(varname);
    if (variable_number < 0) {
        this->variables[varname] = this->variable_count;
        variable_number = this->variable_count++;
    }
    this->code << "lea rsi, [variables+8*" << variable_number << "]" << std::endl;
    this->code << "mov rdi, scanf_format" << std::endl;
    this->code << "xor rax, rax" << std::endl;
    this->code << "call scanf"   << std::endl;
}

void CompilerAssembler::assigmentVariable(std::string varname, std::string expression) {
    int variable_number = this->getVariableCount(varname);
    if (variable_number < 0) {
        this->variables[varname] = this->variable_count;
        variable_number = this->variable_count++;
    }
    this->code << expression << std::endl;
    this->code << "mov rax, [rsp]" << std::endl;
    this->code << "mov qword [variables+8*" << variable_number << "], rax" << std::endl;
}

void CompilerAssembler::printExpression(std::string expression) {
    this->code << expression << std::endl;
    this->code << "movsd xmm0, [rsp]" << std::endl;
    this->code << "mov rdi, printf_format" << std::endl;
    this->code << "mov rax, 1" << std::endl;
    this->code << "call printf" << std::endl;
}

