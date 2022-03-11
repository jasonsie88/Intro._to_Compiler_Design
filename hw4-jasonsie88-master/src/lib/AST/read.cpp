#include "AST/read.hpp"
#include "visitor/AstNodeVisitor.hpp"

ReadNode::ReadNode(const uint32_t line, const uint32_t col,VariableReferenceNode *new_p_target)
    : AstNode{line, col}, m_target(new_p_target) {}

void ReadNode::accept(AstNodeVisitor &p_visitor) {
     p_visitor.visit(*this); 
    }

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_target->accept(p_visitor);
}
