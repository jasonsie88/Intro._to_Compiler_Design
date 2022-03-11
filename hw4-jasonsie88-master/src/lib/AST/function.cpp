#include "AST/function.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <cassert>

FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,
                           const char *new_p_name, Decls *new_p_parameters,
                           const PType *new_p_type, CompoundStatementNode *new_p_body)
    : AstNode{line, col}, m_name(new_p_name), m_parameters(std::move(*new_p_parameters)),
      m_return_type(PTypeSharedPtr(new_p_type)), m_body(new_p_body) {}

const char *FunctionNode::getNameCString() const { 
    return m_name.c_str(); 
}

const char *FunctionNode::getTypeCString() const {
    return m_return_type->getPTypeCString();
}

const char *FunctionNode::getPrototypeCString() const {
    if (!m_prototype_string_is_valid) {
        m_prototype_string += m_return_type->getPTypeCString();

        m_prototype_string += " (";
        for (const auto &m_parameter : m_parameters) {
            for (const auto &var : m_parameter->variables()) {
                m_prototype_string.append(var->getTypeCString()).append(", ");
            }
        }

        if (!m_parameters.empty()) {
            // remove the trailing ", "
            m_prototype_string.erase(m_prototype_string.end() - 2,m_prototype_string.end());
        }

        m_prototype_string += ")";
        m_prototype_string_is_valid = true;
    }

    return m_prototype_string.c_str();
}
const char *FunctionNode::getParatypeCString() const {
    if (!m_paratype_string_is_valid) {
        for (const auto &m_parameter : m_parameters) {
            for (const auto &var : m_parameter->variables()) {
                m_paratype_string.append(var->getTypeCString()).append(", ");
            }
        }

        if (!m_parameters.empty()) {
            // remove the trailing ", "
            m_paratype_string.erase(m_paratype_string.end() - 2,m_paratype_string.end());
        }

        m_paratype_string_is_valid = true;
    }

    return m_paratype_string.c_str();
}

std::vector<PType> FunctionNode::getParatype(){
    std::vector<PType> vec;
    for (const auto &parameter : m_parameters) {
        for (const auto &var : parameter->variables()) {
            vec.push_back(var->getType());
        }
    }
    return vec;
}

bool FunctionNode::isDefined() const { 
    if(m_body){
        return true;
    }else{
        return false;
    }
    }

void FunctionNode::accept(AstNodeVisitor &p_visitor) { 
    p_visitor.visit(*this); 
    }

void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &p : m_parameters) {
        p->accept(p_visitor);
    }
    if (m_body) {
        m_body->accept(p_visitor);
    }
}
