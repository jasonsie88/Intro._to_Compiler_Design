#include "AST/while.hpp"
#include "visitor/AstNodeVisitor.hpp"

WhileNode::WhileNode(const uint32_t line, const uint32_t col,ExpressionNode *new_p_condition, CompoundStatementNode *new_p_body)
    :AstNode{line, col},m_condition(new_p_condition),m_body(new_p_body) {}

void WhileNode::accept(AstNodeVisitor &p_visitor){ 
    p_visitor.visit(*this);
}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_condition->accept(p_visitor);
    m_body->accept(p_visitor);
}
