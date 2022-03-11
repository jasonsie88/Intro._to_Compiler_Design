#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"
class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col,const char * new_variable_string,enum type_p new_type,AstNode* new_constant);
    VariableNode(const uint32_t line, const uint32_t col,const char * new_variable_string,struct array_type *new_type,AstNode* new_constant);
                 /* TODO: variable name, type, constant value */
    ~VariableNode() = default;
    const char *getNameCString();
    const char *return_type_string();
    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;

  private:
    // TODO: variable name, type, constant value
    
    const std::string variable_string;
    AstNode* constant;
    struct array_type *type;
};

#endif
