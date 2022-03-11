#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include <sstream>
// TODO
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,struct value_const *new_constant)
    : ExpressionNode{line, col} {
    type = new_constant->type;
    if (new_constant->type==int_type){
    	int_value=new_constant->int_value;
    }else if(new_constant->type==char_type){
    	string_value=new_constant->string_value;
    }else if(new_constant->type==real_type){
    	real_value=new_constant->real_value;
    }else{
    	bool_value=new_constant->bool_value;
    
    }
    
    
   }

const char *ConstantValueNode::return_const_string(){
	if(type==int_type){
		return (std::to_string(int_value)).c_str();
	}else if(type==char_type){
		return string_value.c_str();
	}else if(type==real_type){
		return (std::to_string(real_value)).c_str();
	}else{
		if(bool_value==false){
			return "false";
		}else{
		
			return "true";
		}
	}

}

// TODO: You may use code snippets in AstDumper.cpp
void ConstantValueNode::print() {}
void ConstantValueNode::visitChildNodes(AstNodeVisitor &p_visitor) {}
void ConstantValueNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);
}
