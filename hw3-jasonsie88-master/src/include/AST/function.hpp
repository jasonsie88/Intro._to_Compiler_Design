#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "visitor/AstNodeVisitor.hpp"

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col,char *new_name,std::vector<DeclNode *> * new_declaration,enum type_p *new_type,AstNode *new_compound_statement);
                 /* TODO: name, declarations, return type,
                  *       compound statement (optional) */
    ~FunctionNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
    const char *return_prototype_string() const;
    const char *getNameCString() const;
  private:
    // TODO: name, declarations, return type, compound statement
    std::string name;
    std::vector<DeclNode *> *declaration;
    enum type_p type;
    AstNode  *compound_statement;
    
    
};

#endif
