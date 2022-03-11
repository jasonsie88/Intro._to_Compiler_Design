#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "AST/operator.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col, Operator new_op,
                       ExpressionNode *new_p_left, ExpressionNode *new_p_right);
    ~BinaryOperatorNode() = default;

    const char *getOpCString() const {
    return kOpString[static_cast<size_t>(m_op)];
    }
    PType get_left_type(){
      return m_left_operand->get_type();
      }
    PType get_right_type(){
      return m_right_operand->get_type();
      }

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    Operator m_op;
    std::unique_ptr<ExpressionNode> m_left_operand;
    std::unique_ptr<ExpressionNode> m_right_operand;
};

#endif
