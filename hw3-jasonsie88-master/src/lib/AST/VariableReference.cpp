#include "AST/VariableReference.hpp"

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,std::string new_variable_name,std::vector<AstNode*>* new_expressions)
    : ExpressionNode{line, col} {
    	variable_name=new_variable_name;
    	expressions = new_expressions;
    }

// TODO
// VariableReferenceNode::VariableReferenceNode(const uint32_t line,
//                                              const uint32_t col)
//     : ExpressionNode{line, col} {}

// TODO: You may use code snippets in AstDumper.cpp
void VariableReferenceNode::print() {}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
  // TODO
  if(expressions!=NULL){
  	for(int i=0;i<expressions->size();i++){
  		expressions->at(i)->accept(p_visitor);
  	
  	}
  }
  	
  
 }
 
 void VariableReferenceNode::accept(AstNodeVisitor &p_visitor){
 
 	p_visitor.visit(*this);
 }
