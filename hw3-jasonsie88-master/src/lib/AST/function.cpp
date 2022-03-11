#include "AST/function.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include<iostream>
using namespace std;
// TODO
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,char *new_name,std::vector<DeclNode *> *new_declaration,enum type_p *new_type,AstNode *new_compound_statement)
    : AstNode{line, col} {
    	name = new_name;
    	declaration = new_declaration;
    	type = *new_type;
    	compound_statement = new_compound_statement;
    }

// TODO: You may use code snippets in AstDumper.cpp
void FunctionNode::print() {}
const char *FunctionNode::return_prototype_string() const{
	char * str = new char[512];
	if(type==int_type){
		strcat(str,"integer ");
	}else if(type==real_type){
		strcat(str,"real ");
	}else if(type==char_type){
		strcat(str,"string ");
	}else if(type==bool_type){
		strcat(str,"boolean ");
	}else{
		strcat(str,"void ");
	}
	
	strcat(str,"(");
	if(declaration!=NULL){
		for(int i=0;i<declaration->size();i++){
		//cout<<"function i: "<<i<<endl;
		//cout<<"function size: "<<declaration->size()<<endl;
		//cout<<declaration->at(0)<<endl;
		//cout<<declaration->at(0)->return_prototype_string()<<endl;
			if(i>0){
				strcat(str,", ");
			}
			strcat(str,declaration->at(i)->return_prototype_string());
		
		}
	
	}
	strcat(str,")");
	return str;
}

const char *FunctionNode::getNameCString() const { return name.c_str(); }
void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     if(declaration!=NULL){
     	for(auto &d:*declaration){
     		d->accept(p_visitor);
     	}
     }
     if(compound_statement!=NULL){
     	compound_statement->accept(p_visitor);
     }
     
     
}
void FunctionNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);

}
