#include "AST/assignment.hpp"
#include "visitor/AstNodeVisitor.hpp"

AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,
                               VariableReferenceNode *new_lvalue,
                               ExpressionNode *new_expr)
    : AstNode{line, col}, m_lvalue(new_lvalue), m_expr(new_expr) {}

void AssignmentNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_lvalue->accept(p_visitor);
    m_expr->accept(p_visitor);
}
