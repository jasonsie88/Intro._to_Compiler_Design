#include "AST/UnaryOperator.hpp"

// TODO
UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,std::string new_operator_string,AstNode *new_expression)
    : ExpressionNode{line, col} {
    operator_string=new_operator_string;
    expression=new_expression;
    
    }

// TODO: You may use code snippets in AstDumper.cpp
void UnaryOperatorNode::print() {}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
   // TODO
   if(expression!=NULL){
   	expression->accept(p_visitor);
   
   }
}

void UnaryOperatorNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);
}
