#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"


class AssignmentNode : public AstNode {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col,AstNode *new_variable,AstNode *new_expression);
                   /* TODO: variable reference, expression */
    ~AssignmentNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    void accept(AstNodeVisitor &p_visitor) override;
  private:
    AstNode *variable;
    AstNode *expression;
    // TODO: variable reference, expression
};

#endif
