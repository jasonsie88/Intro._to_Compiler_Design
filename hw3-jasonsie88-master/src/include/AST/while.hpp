#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"

class WhileNode : public AstNode {
  public:
    WhileNode(const uint32_t line, const uint32_t col,AstNode* new_expression,AstNode* new_compound_statement);
              /* TODO: expression, compound statement */
    ~WhileNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
  private:
    // TODO: expression, compound statement
    AstNode* expression;
    AstNode* compound_statement;
};

#endif
