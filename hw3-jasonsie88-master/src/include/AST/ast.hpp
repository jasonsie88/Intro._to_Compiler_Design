#ifndef AST_AST_NODE_H
#define AST_AST_NODE_H

#include "visitor/AstNodeVisitor.hpp"
#include <cstdint>
#include <vector>
#include <cstring>
#include <string>

class AstNodeVisitor;

struct Location {
    uint32_t line;
    uint32_t col;

    ~Location() = default;
    Location(const uint32_t line, const uint32_t col) : line(line), col(col) {}
};

enum type_p{
	int_type,
	real_type,
	char_type,
	bool_type,
	void_type
};

union value{
	int int_value;
	bool bool_value;
	std::string string_value;
	double real_value;
};

struct value_const{
	enum type_p type;
	int col;
	int int_value;
	bool bool_value;
	std::string string_value;
	double real_value;
};

struct array_type{
	enum type_p type;
	std::vector<int> v;
};


struct id_list{
	enum type_p type;
	std::vector<int> col_list;
	std::vector<char *> var_list; 
};



class AstNode {
  protected:
    Location location;

  public:
    virtual ~AstNode() = 0;
    AstNode(const uint32_t line, const uint32_t col);
    virtual void visitChildNodes(AstNodeVisitor &p_visitor) = 0;
    const Location &getLocation() const;
    virtual void accept(AstNodeVisitor &p_visitor) = 0;
    virtual void print() = 0;
    
};

#endif
