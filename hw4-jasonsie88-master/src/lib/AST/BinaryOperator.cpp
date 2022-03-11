#include "AST/BinaryOperator.hpp"
#include "visitor/AstNodeVisitor.hpp"

BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col,
                                       Operator new_op, ExpressionNode *new_p_left,
                                       ExpressionNode *new_p_right)
    : ExpressionNode{line, col}, m_op(new_op), m_left_operand(new_p_left), m_right_operand(new_p_right) {}



void BinaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_left_operand->accept(p_visitor);
    m_right_operand->accept(p_visitor);
}
