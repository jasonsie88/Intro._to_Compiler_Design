#include "AST/variable.hpp"

// TODO
VariableNode::VariableNode(const uint32_t line, const uint32_t col,const char * new_variable_string,enum type_p new_type,AstNode* new_constant)
    : AstNode{line, col},variable_string(new_variable_string){
    	//variable_string = new_variable_string;
    	constant = new_constant;
    	type = new struct array_type;
    	type->type=new_type;
    }

VariableNode::VariableNode(const uint32_t line, const uint32_t col,const char * new_variable_string,struct array_type *new_type,AstNode* new_constant)
    : AstNode{line, col},variable_string(new_variable_string) {
    	//variable_string = new_variable_string;
    	constant = new_constant;
    	type = new_type;
    }

// TODO: You may use code snippets in AstDumper.cpp
void VariableNode::print() {}


const char *VariableNode::getNameCString(){
	return variable_string.c_str();
	
}
const char *VariableNode::return_type_string(){
	std::string str;
	if(type->type==int_type){
		str="integer";
	}else if(type->type==char_type){
		str="string";
	}else if(type->type==bool_type){
		str="boolean";
	}else if(type->type==real_type){
		str="real";
	}else if(type->type==void_type){
		str="";
	}
	for(int i=0;i<type->v.size();i++){
		if(i==0){
			str+=" ";
		}
		str+="[";
        	str+=std::to_string(type->v[i]);
        	str+="]";
	
	}
	return str.c_str();
}


void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
   // TODO
   	if(constant!=NULL){
   		constant->accept(p_visitor);
   	}
}

void VariableNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);

}
