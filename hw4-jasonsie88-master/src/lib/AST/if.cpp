#include "AST/if.hpp"
#include "visitor/AstNodeVisitor.hpp"

IfNode::IfNode(const uint32_t line, const uint32_t col,
               ExpressionNode *new_p_condition, CompoundStatementNode *new_p_body,
               CompoundStatementNode *new_p_else)
    : AstNode{line, col}, m_condition(new_p_condition), m_body(new_p_body),m_else_body(new_p_else) {}

void IfNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_condition->accept(p_visitor);
    m_body->accept(p_visitor);
    if (m_else_body) {
        m_else_body->accept(p_visitor);
    }
}
