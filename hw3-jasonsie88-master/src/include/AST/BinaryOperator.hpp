#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col,std::string new_operator_string,AstNode* new_left_expression,AstNode* new_right_expression);
                       /* TODO: operator, expressions */
    ~BinaryOperatorNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
    const char *return_op_string() const{return operator_string.c_str();}

  private:
    // TODO: operator, expressions
    std::string operator_string;
    AstNode* left_expression;
    AstNode* right_expression;
};

#endif
