#include "AST/variable.hpp"
#include "visitor/AstNodeVisitor.hpp"

VariableNode::VariableNode(const uint32_t line, const uint32_t col,const std::string &new_name,const PTypeSharedPtr &new_p_type,const std::shared_ptr<ConstantValueNode> &new_p_constant)
    :AstNode{line, col},m_name(std::move(new_name)),m_type(new_p_type),m_constant(new_p_constant){}

const char *VariableNode::getNameCString()const{ 
    return m_name.c_str(); 
}

const char *VariableNode::getTypeCString()const {
    return m_type->getPTypeCString();
}

void VariableNode::accept(AstNodeVisitor &p_visitor){ 
        p_visitor.visit(*this); 
}

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (m_constant) {
        m_constant->accept(p_visitor);
    }
}
