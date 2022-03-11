#ifndef __AST_EXPRESSION_NODE_H
#define __AST_EXPRESSION_NODE_H

#include "AST/PType.hpp"
#include "AST/ast.hpp"

class ExpressionNode : public AstNode {
  public:
    ExpressionNode(const uint32_t line, const uint32_t col);
    ~ExpressionNode() = default;

    PType get_type(){
        return *m_type;
      }
    PTypeSharedPtr get_type_ptr(){
        return m_type;
      }
    void set_type(PTypeSharedPtr new_type){
          m_type = new_type;
      }
    void set_error(){
        error = 1;
      }
    int get_error(){ 
        return error;
      }

  protected:
    // for carrying type of result of an expression
    PTypeSharedPtr m_type = nullptr;
    int error = 0;
};

#endif
