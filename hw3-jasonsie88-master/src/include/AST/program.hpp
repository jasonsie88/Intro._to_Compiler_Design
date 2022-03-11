#ifndef AST_PROGRAM_NODE_H
#define AST_PROGRAM_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"


class ProgramNode final : public AstNode {
  private:
    const std::string name;
    const std::string return_type;
    std::vector<AstNode *> *declarations;
    std::vector<AstNode *> *functions;
    AstNode *compound_statement; 
    // TODO: return type, declarations, functions, compound statement
    

  public:
    ~ProgramNode() = default;
    ProgramNode(const uint32_t line, const uint32_t col,
                const char *p_name, const char *new_return_type,
                std::vector<AstNode*> *new_declarations,
                std::vector<AstNode*> *new_functions,
                AstNode *new_compound_statement);
                /* TODO: return type, declarations, functions,
                 *       compound statement */

    // visitor pattern version: 
    const char *getNameCString() const;
    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
};

#endif
