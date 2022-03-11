#include "AST/BinaryOperator.hpp"
#include "visitor/AstNodeVisitor.hpp"

// TODO
BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col,std::string new_operator_string,AstNode* new_left_expression,AstNode* new_right_expression)
    : ExpressionNode{line, col} {
    operator_string=new_operator_string;
    left_expression=new_left_expression;
    right_expression=new_right_expression;
    
    }

// TODO: You may use code snippets in AstDumper.cpp
void BinaryOperatorNode::print() {}

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     left_expression->accept(p_visitor);
     right_expression->accept(p_visitor);
 }

void BinaryOperatorNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);

}

