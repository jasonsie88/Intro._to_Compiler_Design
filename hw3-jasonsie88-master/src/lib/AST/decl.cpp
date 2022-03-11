#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include<iostream>
using namespace std;
// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col,struct id_list *identifiers,struct array_type *types)
    : AstNode{line, col} {
    	variables_list=new std::vector<AstNode *>;
    	function_list = new std::vector<VariableNode *>;
    	for(int i=0;i<identifiers->var_list.size();i++){
    		AstNode *new_variable = new VariableNode(line,identifiers->col_list.at(i),identifiers->var_list.at(i),types,NULL);
    		variables_list->push_back(new_variable);
    		VariableNode *new_function = new VariableNode(line,identifiers->col_list.at(i),identifiers->var_list.at(i),types,NULL);
    		function_list->push_back(new_function);
    	}
    
    }

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col,struct id_list *identifiers,struct value_const *constants)
    : AstNode{line, col} {
    	variables_list=new std::vector<AstNode *>;
        for(int i=0;i<identifiers->var_list.size();i++){
    		AstNode *new_constant = new ConstantValueNode(line,constants->col,constants);
    		AstNode *new_variable = new VariableNode(line,identifiers->col_list.at(i),identifiers->var_list.at(i),constants->type,new_constant);
    		variables_list->push_back(new_variable);
    	
    	}
    }

// TODO: You may use code snippets in AstDumper.cpp
void DeclNode::print() {}

const char *DeclNode::return_prototype_string(){
	char *str= new char[512];
	if(function_list!=NULL){
		for(int i=0;i<function_list->size();i++){
			//cout<<"i: "<<i<<endl;
			//cout<<"function_list: "<<function_list->size()<<endl;
			if(i>0){
				strcat(str,", ");
			}
			strcat(str,function_list->at(i)->return_type_string());
		}
	
	}
	//cout<<str<<endl;
	return str;

}

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     for(auto &var:*variables_list){
     	var->accept(p_visitor);
     }
}

void DeclNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);

}
