#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"

#include <memory>

class ReturnNode : public AstNode {
  public:
    ReturnNode(const uint32_t line, const uint32_t col,ExpressionNode *new_p_ret_val);
    ~ReturnNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  public:
    std::unique_ptr<ExpressionNode> m_ret_val;
};

#endif
