#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"
class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col, AstNode* new_declaration,AstNode* new_assignment,AstNode* new_expression,AstNode* new_statement);
            /* TODO: declaration, assignment, expression,
             *       compound statement */
    ~ForNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
  private:
    // TODO: declaration, assignment, expression, compound statement
    AstNode* declaration;
    AstNode* assignment;
    AstNode* expression;
    AstNode* statement;
};

#endif
