#include "AST/return.hpp"
#include "visitor/AstNodeVisitor.hpp"

ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,ExpressionNode *new_p_ret_val)
    :AstNode{line, col}, m_ret_val(new_p_ret_val){}

void ReturnNode::accept(AstNodeVisitor &p_visitor){ 
        p_visitor.visit(*this); 
}

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_ret_val->accept(p_visitor);
}
