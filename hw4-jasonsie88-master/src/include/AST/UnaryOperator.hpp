#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "AST/operator.hpp"

#include <memory>

class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col, Operator new_op,ExpressionNode *new_p_operand);
    ~UnaryOperatorNode() = default;
    const char *getOpCString() const;
    PType get_oparand_type(){
        return m_operand->get_type();
    }

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    Operator m_op;
    std::unique_ptr<ExpressionNode> m_operand;
};

#endif
