#include "AST/print.hpp"
#include "visitor/AstNodeVisitor.hpp"

PrintNode::PrintNode(const uint32_t line, const uint32_t col, ExpressionNode *new_p_expr)
    : AstNode{line, col}, m_target(new_p_expr) {}

void PrintNode::accept(AstNodeVisitor &p_visitor) {
     p_visitor.visit(*this); 
     }

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_target->accept(p_visitor);
}
