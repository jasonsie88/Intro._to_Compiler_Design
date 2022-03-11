#include "AST/for.hpp"
#include "visitor/AstNodeVisitor.hpp"

ForNode::ForNode(const uint32_t line, const uint32_t col, DeclNode *new_p_var_decl,
                 AssignmentNode *new_p_initial_statement,
                 ConstantValueNode *new_p_condition, CompoundStatementNode *new_p_body, int new_left)
    : AstNode{line, col}, m_var_decl(new_p_var_decl),
      m_initial_statement(new_p_initial_statement), m_condition(new_p_condition),
      m_body(new_p_body), m_left(new_left) {}

void ForNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    m_var_decl->accept(p_visitor);
    m_initial_statement->accept(p_visitor);
    m_condition->accept(p_visitor);
    m_body->accept(p_visitor);
}
