#include "AST/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

CompoundStatementNode::CompoundStatementNode(const uint32_t line,
                                             const uint32_t col,
                                             Decls *new_var_decls,
                                             Nodes *new_statements)
    : AstNode{line, col}, m_var_decls(std::move(*new_var_decls)),
      m_statements(std::move(*new_statements)) {}

void CompoundStatementNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &d : m_var_decls) {
        d->accept(p_visitor);
    }
    for (auto &s : m_statements) {
        s->accept(p_visitor);
    }
}
