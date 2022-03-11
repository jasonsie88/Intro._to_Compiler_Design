#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"
class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, const uint32_t col,AstNode *new_expression,AstNode *new_if_expression,AstNode *new_else_expression );
           /* TODO: expression, compound statement, compound statement */
    ~IfNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
  private:
    // TODO: expression, compound statement, compound statement
    AstNode *expression;
    AstNode *if_expression;
    AstNode *else_expression;
    
};

#endif
