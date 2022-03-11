#include "AST/program.hpp"
#include "visitor/AstNodeVisitor.hpp"

extern int in_func;
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,const PType *new_p_return_type, const char *new_p_name,Decls *new_p_var_decls, Funcs *new_p_funcs,CompoundStatementNode *new_p_body)
    : AstNode{line, col}, m_return_type(new_p_return_type), m_name(new_p_name),m_var_decls(std::move(*new_p_var_decls)), m_funcs(std::move(*new_p_funcs)),m_body(new_p_body) {}

const char *ProgramNode::getNameCString() const { 
        return m_name.c_str(); 
    }

const char *ProgramNode::getTypeCString() const {
    return m_return_type->getPTypeCString();
}

void ProgramNode::accept(AstNodeVisitor &p_visitor) { 
    p_visitor.visit(*this); 
}

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &d : m_var_decls) {
        d->accept(p_visitor);
    }
    in_func = 1;
    for (auto &f : m_funcs) {
        f->accept(p_visitor);
    }
    in_func = 0;
    m_body->accept(p_visitor);
}
