#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col,struct value_const *new_constant);
                      /* TODO: constant value */
    ~ConstantValueNode() = default;
    const char *return_const_string();
    void print() override;
    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
  private:
    // TODO: constant value
	enum type_p type;
	int int_value;
	bool bool_value;
	std::string string_value;
	double real_value;
};

#endif
