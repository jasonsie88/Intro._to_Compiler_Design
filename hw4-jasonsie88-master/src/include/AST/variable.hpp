#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ConstantValue.hpp"
#include "AST/PType.hpp"
#include "AST/ast.hpp"

#include <memory>

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col,
                 const std::string &name, const PTypeSharedPtr &p_type,
                 const std::shared_ptr<ConstantValueNode> &p_constant);
    ~VariableNode() = default;

    const char *getNameCString() const;
    const char *getTypeCString() const;
    const char *getconstantCString() const{ 
        if(m_constant == NULL){
              return "";
        }else{
          return m_constant->getConstantValueCString();
        }
    }
    bool is_constant(){
      if(m_constant!=NULL){
        return true;
      }else{
        return false;
      }
     // return (constant != NULL);
      }
    
    PType getType(){
          return *m_type; 
    }

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    const std::string m_name;
    const PTypeSharedPtr m_type;
    std::shared_ptr<ConstantValueNode> m_constant;
};

#endif
