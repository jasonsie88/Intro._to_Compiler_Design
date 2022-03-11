#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/PType.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"

#include <memory>
#include <vector>

class ProgramNode : public AstNode {
  public:
    typedef std::vector<std::unique_ptr<DeclNode>> Decls;
    typedef std::vector<std::unique_ptr<FunctionNode>> Funcs;

    ProgramNode(const uint32_t line, const uint32_t col,const PType *new_p_return_type, const char *new_p_name,Decls *new_p_var_decls, Funcs *new_p_funcs,CompoundStatementNode *new_p_body);
    ~ProgramNode() = default;

    const char *getNameCString() const;
    const char *getTypeCString() const;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::unique_ptr<const PType> m_return_type;
    const std::string m_name;
    Decls m_var_decls;
    Funcs m_funcs;
    std::unique_ptr<CompoundStatementNode> m_body;
};

#endif
