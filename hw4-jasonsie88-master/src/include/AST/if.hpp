#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ast.hpp"
#include "AST/expression.hpp"

#include <memory>

class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, const uint32_t col, ExpressionNode *new_p_condition,CompoundStatementNode *new_p_body, CompoundStatementNode *new_p_else);
    ~IfNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  public:
    std::unique_ptr<ExpressionNode> m_condition;
    std::unique_ptr<CompoundStatementNode> m_body;
    std::unique_ptr<CompoundStatementNode> m_else_body;
};

#endif
