#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"


class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col,std::string new_operator_string,AstNode *new_expression);
                      /* TODO: operator, expression */
    ~UnaryOperatorNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
    const char *return_op_string() const{return operator_string.c_str();}
    	
  private:
    // TODO: operator, expression
    std::string operator_string;
    AstNode* expression;
};

#endif
