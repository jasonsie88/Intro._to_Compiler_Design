#include "AST/while.hpp"

// TODO
WhileNode::WhileNode(const uint32_t line, const uint32_t col,AstNode* new_expression,AstNode* new_compound_statement)
    : AstNode{line, col} {
    	expression = new_expression;
    	compound_statement = new_compound_statement;
    }

// TODO: You may use code snippets in AstDumper.cpp
void WhileNode::print() {}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    
    	expression->accept(p_visitor);
    
    
    	compound_statement->accept(p_visitor);
    
}

void WhileNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);

}
