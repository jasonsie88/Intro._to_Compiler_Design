#include "AST/FunctionInvocation.hpp"
#include "visitor/AstNodeVisitor.hpp"

FunctionInvocationNode::FunctionInvocationNode(const uint32_t line,const uint32_t col,const char *new_p_name,Exprs *new_p_arguments):ExpressionNode{line, col}, m_name(new_p_name),m_arguments(std::move(*new_p_arguments)) {}

const char *FunctionInvocationNode::getNameCString() const {
    return m_name.c_str();
}

void FunctionInvocationNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &arg : m_arguments) {
        arg->accept(p_visitor);
    }
}

std::vector<PType> FunctionInvocationNode::getParatype(){
    std::vector<PType> vec;
    for (auto &arg : m_arguments) {
        vec.push_back(arg->get_type());
    }
    return vec;
}
