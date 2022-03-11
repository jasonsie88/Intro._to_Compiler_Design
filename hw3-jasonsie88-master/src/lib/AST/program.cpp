#include "AST/program.hpp"

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         const char *p_name, const char *new_return_type,
                std::vector<AstNode *> * new_declarations,
                std::vector<AstNode *> * new_functions,
                AstNode *new_compound_statement)
    : AstNode{line, col}, name(p_name),return_type(new_return_type) {
    	//return_type=new_return_type;
    	declarations = new_declarations;
    	functions = new_functions;
    	compound_statement = new_compound_statement;
    }

// visitor pattern version: 
const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::print() {
    // TODO
    // outputIndentationSpace();

    //std::printf("program <line: %u, col: %u> %s %s\n",
    //            location.line, location.col,
    //            name.c_str(), "void");

    // TODO
    // incrementIndentation();
    // visitChildNodes();
    // decrementIndentation();
}


void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
   // TODO
	
	if(declarations!=NULL){
		for(auto &d: *declarations){
			d->accept(p_visitor);
		}
	}	
	if(functions!=NULL){
		for(auto &f: *functions){
			f->accept(p_visitor);
		}
	
	}

	compound_statement->accept(p_visitor);

}

void ProgramNode::accept(AstNodeVisitor &p_visitor){
	p_visitor.visit(*this);
}
