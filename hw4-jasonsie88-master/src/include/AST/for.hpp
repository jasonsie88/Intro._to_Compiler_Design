#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"

#include <memory>

class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col, DeclNode *new_p_var_decl,AssignmentNode *new_p_initial_statement, ConstantValueNode *new_p_condition,CompoundStatementNode *new_p_body, int new_left);
    ~ForNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  public:
    std::unique_ptr<DeclNode> m_var_decl;
    std::unique_ptr<AssignmentNode> m_initial_statement;
    std::unique_ptr<ConstantValueNode> m_condition;
    std::unique_ptr<CompoundStatementNode> m_body;
    int m_left;
};

#endif
