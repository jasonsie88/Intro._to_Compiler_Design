#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class FunctionInvocationNode : public ExpressionNode {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col,std::string new_function_name,std::vector<AstNode *>* new_expression );
                           /* TODO: function name, expressions */
    ~FunctionInvocationNode() = default;

    const char *getNameCString() const{return function_name.c_str();}

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
  private:
    // TODO: function name, expressions
    std::string function_name;
    std::vector<AstNode *>* expression;
};

#endif
