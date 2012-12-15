#include "LogicalAssembler"

int LogicalAssembler::expression_count = 0;

LogicalAssembler::~LogicalAssembler() {}

LogicalAssembler::LogicalAssembler() {
    LogicalAssembler::expression_count++;
}

LogicalAssembler::newExpression() {
    this->code.str(std::string());
    LogicalAssembler::expression_count++;
}

std::string LogicalAssembler::getExpressionName() {
    std::ostringstream name;
    name << "logical_expression_" << LogicalAssembler::expression_count;
    return name.str();
}

std::string LogicalAssembler::getCode() {
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

LogicalAssembler::

LogicalAssembler 

LogicalAssembler 

LogicalAssembler 

LogicalAssembler 

