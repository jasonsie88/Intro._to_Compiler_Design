#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"

#include <memory>
#include <vector>

class FunctionInvocationNode : public ExpressionNode {
  public:
    typedef std::vector<std::unique_ptr<ExpressionNode>> Exprs;

    FunctionInvocationNode(const uint32_t line, const uint32_t col,
                           const char *new_p_name, Exprs *new_p_arguments);
    ~FunctionInvocationNode() = default;

    const char *getNameCString() const;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::vector<PType> getParatype();

  public:
    const std::string m_name;
    Exprs m_arguments;
};

#endif
