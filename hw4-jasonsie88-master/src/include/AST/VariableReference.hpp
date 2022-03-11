#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"

#include <memory>
#include <vector>

class VariableReferenceNode : public ExpressionNode {
  public:
    typedef std::vector<std::unique_ptr<ExpressionNode>> Exprs;

    VariableReferenceNode(const uint32_t line, const uint32_t col,const char *new_p_name);
    VariableReferenceNode(const uint32_t line, const uint32_t col,const char *new_p_name, Exprs *new_p_indices);
    ~VariableReferenceNode() = default;

    const char *getNameCString() const;

    int get_error_inner();
    int get_dim(){
        return m_indices.size();
      }
    std::vector<PType::PrimitiveTypeEnum> get_Exprs();
    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  public:
    const std::string m_name;
    Exprs m_indices;
};

#endif
