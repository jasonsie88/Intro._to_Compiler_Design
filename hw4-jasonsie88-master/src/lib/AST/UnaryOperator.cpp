#include "AST/UnaryOperator.hpp"
#include "visitor/AstNodeVisitor.hpp"

UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,Operator new_op, ExpressionNode *new_p_operand)
    : ExpressionNode{line, col},m_op(new_op),m_operand(new_p_operand) {}

const char *UnaryOperatorNode::getOpCString() const {
    return kOpString[static_cast<size_t>(m_op)];
}

void UnaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_operand->accept(p_visitor);
}
