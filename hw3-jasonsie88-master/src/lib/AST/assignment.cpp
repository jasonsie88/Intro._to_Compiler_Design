#include "AST/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,AstNode *new_variable,AstNode *new_expression)
    : AstNode{line, col} {
    	variable=new_variable;
    	expression=new_expression;
    }

// TODO: You may use code snippets in AstDumper.cpp
void AssignmentNode::print() {}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     variable->accept(p_visitor);
     expression->accept(p_visitor);
 }
 
 void AssignmentNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}
