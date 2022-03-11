#include "AST/decl.hpp"
#include "AST/PType.hpp"
#include "visitor/AstNodeVisitor.hpp"

static void init(DeclNode::VarNodes &vars, const std::vector<IdInfo> *new_p_id_list,
                 const PTypeSharedPtr &type, ConstantValueNode *constant) {
    std::shared_ptr<ConstantValueNode> shared_constant(constant);
        for (auto id_info : *new_p_id_list) {
            vars.emplace_back(new VariableNode(id_info.location.line,id_info.location.col, id_info.id,type, shared_constant));
        }
}

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   const std::vector<IdInfo> *new_p_id_list, const PType *new_p_type)
    : AstNode{line, col} {
        init(m_vars, new_p_id_list, PTypeSharedPtr(new_p_type), nullptr);
}

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   const std::vector<IdInfo> *new_p_id_list,
                   ConstantValueNode *new_p_constant)
    : AstNode{line, col} {
        init(m_vars, new_p_id_list, new_p_constant->getTypePtr(), new_p_constant);
}

const DeclNode::VarNodes &DeclNode::variables() const { return m_vars; }

void DeclNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &v : m_vars) {
        v->accept(p_visitor);
    }
}
