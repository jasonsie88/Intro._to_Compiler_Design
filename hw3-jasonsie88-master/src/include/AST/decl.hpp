#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
#include "AST/variable.hpp"
#include "visitor/AstNodeVisitor.hpp"

class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(const uint32_t line, const uint32_t col,struct id_list *identifiers,struct array_type *types);
             /* TODO: identifiers, type */

    // constant variable declaration
    DeclNode(const uint32_t, const uint32_t col,struct id_list *identifiers,struct value_const *constants);
             /* TODO: identifiers, constant */

    ~DeclNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
    const char *return_prototype_string();
    std::vector<AstNode *> *variables_list;
    std::vector<VariableNode *> *function_list;


    // TODO: variables
};

#endif
