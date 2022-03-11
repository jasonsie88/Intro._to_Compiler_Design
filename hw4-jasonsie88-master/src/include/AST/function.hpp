#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/PType.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"

#include <memory>
#include <vector>

class FunctionNode : public AstNode {
  public:
    typedef std::vector<std::unique_ptr<DeclNode>> Decls;

    FunctionNode(const uint32_t line, const uint32_t col, const char *new_p_name,Decls *new_p_parameters, const PType *new_p_type,CompoundStatementNode *new_p_body);
    ~FunctionNode() = default;

    const char *getNameCString() const;
    const char *getTypeCString() const;
    const char *getPrototypeCString() const;
    const char *getParatypeCString() const;

    std::vector<PType> getParatype();
    PType getReturnType(){return *m_return_type;}
    bool isDefined() const;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    const std::string m_name;
    Decls m_parameters;
    PTypeSharedPtr m_return_type;
    mutable std::string m_prototype_string;
    mutable std::string m_paratype_string;
    mutable bool m_prototype_string_is_valid = false;
    mutable bool m_paratype_string_is_valid = false;
    std::unique_ptr<CompoundStatementNode> m_body;
};

#endif
