#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/VariableReference.hpp"
#include "AST/ast.hpp"

#include <memory>

class ReadNode : public AstNode {
  public:
    ReadNode(const uint32_t line, const uint32_t col,VariableReferenceNode *new_p_target);
    ~ReadNode() = default;

    PType get_expr_type(){
      return m_target->get_type();
    };
    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    int get_expr_error() { 
      return m_target->get_error();
    }

  public:
    std::unique_ptr<VariableReferenceNode> m_target;
};

#endif
