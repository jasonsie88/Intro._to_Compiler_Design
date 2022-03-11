#include "AST/VariableReference.hpp"
#include "visitor/AstNodeVisitor.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line,const uint32_t col,const char *new_p_name)
    : ExpressionNode{line, col}, m_name(new_p_name) {}

VariableReferenceNode::VariableReferenceNode(const uint32_t line,const uint32_t col,const char *new_p_name,Exprs *new_p_indices)
    : ExpressionNode{line, col}, m_name(new_p_name), m_indices(std::move(*new_p_indices)) {}

const char *VariableReferenceNode::getNameCString() const {
    return m_name.c_str();
}

int VariableReferenceNode::get_error_inner()
{
    for(auto &i:m_indices){
        if(i->get_error()){
            this->set_error();
            return true;
        }
    }
    return 0;
}

std::vector<PType::PrimitiveTypeEnum> VariableReferenceNode::get_Exprs()
{
    std::vector<PType::PrimitiveTypeEnum> vec;
    for(auto &i: m_indices){
        vec.push_back(i->get_type().getPrimitiveType());
    }
    return vec;
}

void VariableReferenceNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &i : m_indices) {
        i->accept(p_visitor);
    }
}
