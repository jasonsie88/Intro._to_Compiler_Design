#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     const Constant *new_p_constant)
    : ExpressionNode{line, col}, m_constant(new_p_constant) {}

const PTypeSharedPtr &ConstantValueNode::getTypePtr() const {
    return m_constant->getTypePtr();
}

const char *ConstantValueNode::getConstantValueCString() const {
    return m_constant->getConstantValueCString();
}

void ConstantValueNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}
