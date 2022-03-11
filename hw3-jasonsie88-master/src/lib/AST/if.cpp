#include "AST/if.hpp"

// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col,AstNode *new_expression,AstNode *new_if_expression,AstNode *new_else_expression)
    : AstNode{line, col} {
    	expression = new_expression;
    	if_expression = new_if_expression;
    	else_expression = new_else_expression;
    
    }

// TODO: You may use code snippets in AstDumper.cpp
void IfNode::print() {}

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     expression->accept(p_visitor);
     if_expression->accept(p_visitor);
     if(else_expression!=NULL){
     	else_expression->accept(p_visitor);
     }
     
     
 }
void IfNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);

}
