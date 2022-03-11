#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"


class VariableReferenceNode : public ExpressionNode {
  public:
    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,std::string new_variable_name,std::vector<AstNode*>* new_expressions);
                          /* TODO: name */
    // array reference
    // VariableReferenceNode(const uint32_t line, const uint32_t col
    //                       /* TODO: name, expressions */);
    ~VariableReferenceNode() = default;
    const char *getNameCString() const { return variable_name.c_str(); }
    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
  private:
    // TODO: variable name, expressions
    std::string variable_name;
    std::vector<AstNode*>* expressions;
};

#endif
