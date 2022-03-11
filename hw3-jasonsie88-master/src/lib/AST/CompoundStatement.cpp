#include "AST/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"
// TODO
CompoundStatementNode::CompoundStatementNode(const uint32_t line,
                                             const uint32_t col,std::vector<AstNode *> *new_declaration,std::vector<AstNode *> *new_statements)
    : AstNode{line, col} {
    declaration=new_declaration;
    statements=new_statements;
    
    }

// TODO: You may use code snippets in AstDumper.cpp
void CompoundStatementNode::print() {}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     if(declaration!=NULL){
     	for(auto &d : *declaration){
     		d->accept(p_visitor);
     	}
     }
     
     if(statements!=NULL){
     	for(auto &s:*statements){
     		s->accept(p_visitor);
     	}
     	
     }
}

void CompoundStatementNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);
	
}
