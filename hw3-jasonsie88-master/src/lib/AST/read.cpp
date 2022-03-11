#include "AST/read.hpp"

// TODO
ReadNode::ReadNode(const uint32_t line, const uint32_t col,AstNode *new_variable_ref)
    : AstNode{line, col} {
    	variable_ref=new_variable_ref;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ReadNode::print() {}

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
   // TODO
   
   variable_ref->accept(p_visitor);
 }
 void ReadNode::accept(AstNodeVisitor &p_visitor){
 	p_visitor.visit(*this);
 }
