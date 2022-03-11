#include "AST/for.hpp"
#include "visitor/AstNodeVisitor.hpp"
// TODO
ForNode::ForNode(const uint32_t line, const uint32_t col, AstNode* new_declaration,AstNode* new_assignment,AstNode* new_expression,AstNode* new_statement)
    : AstNode{line, col} {
    	declaration = new_declaration;
    	assignment = new_assignment;
    	expression = new_expression;
    	statement = new_statement;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ForNode::print() {}

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     declaration->accept(p_visitor);
     assignment->accept(p_visitor);
     expression->accept(p_visitor);
     statement->accept(p_visitor);
 }
 
void ForNode::accept(AstNodeVisitor &p_visitor){

	p_visitor.visit(*this);

}
