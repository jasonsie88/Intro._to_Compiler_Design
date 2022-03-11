#include<vector>
#include<string>
#include"AST/PType.hpp"
#include"AST/constant.hpp"
#define TABLE_KIND_PROGRAM 1
#define TABLE_KIND_FUNCTION 2
#define TABLE_KIND_FORLOOP 3
#define TABLE_KIND_REST 0

#define ENTRY_KIND_PROGRAM 0
#define ENTRY_KIND_FUNCTION 1
#define ENTRY_KIND_PARAMETER 2
#define ENTRY_KIND_VARIABLE 3
#define ENTRY_KIND_LOOP_VAR 4
#define ENTRY_KIND_CONSTANT 5
#define ENTRY_KIND_NO_FOUND 6


class SymbolEntry {
public:
    SymbolEntry(std::string new_p_name, int new_p_kind, int new_p_level, PType new_p_type, std::string new_p_attribute, int new_pass);
    char* get_kind_cstr();
public:
    std::string name;
    int kind;
    int level;
    PType type;
    std::string attribute;
    int pass;
};

class SymbolTable {
public:
    SymbolTable(int new_p_kind);
    void increase_symbol(std::string new_p_name, int new_p_kind, int new_p_level, PType new_p_type, std::string new_p_attribute, int new_pass);
    int look_up(std::string name);
    int get_kind(){
        return kind;
    }
    SymbolEntry get_symbol_entry(std::string name);
    void state_change(int n){
        kind = n;
    }
    void print_table();
    void dumpDemarcation(const char ch);
private:
    std::vector<SymbolEntry> entries;
    int kind;
};

class SymbolManager {
public:
    void increase_symbol(std::string p_name, int p_kind, PType p_type, std::string p_attribute, int pass);
    int look_up(std::string name, bool global);
    bool look_up_for(std::string name);
    int get_top_kind(){
        return s_tables.back()->get_kind();
    }
    SymbolEntry get_symbol_entry(std::string name);
    void state_change(int n){
        s_tables.back()->state_change(n);
    }
    void push_scope(SymbolTable *new_scope);
    void pop_scope(bool print);
private:
    std::vector <SymbolTable *> s_tables;
};


class FunctionEntry{
public:
    FunctionEntry(std::string name_n, std::vector<PType> paremeter_type_p, PType return_type_p);
public:
    std::string name;
    std::vector<PType> paremeter_type;
    PType return_type;
};

class FunctionManager{
public:
    FunctionManager(){f_num = 0;};

    void increase_function(std::string name_n, std::vector<PType> paremeter_type_p, PType return_type_p);
    FunctionEntry look_up(std::string name);
    FunctionEntry look_up(int n){
        return f_table[n];
    }
    void add_num(){
        f_num++;
    }
    void minus_num(){
        f_num--;
    }
    int get_func_num(){
        return f_num;
    }
public:
    std::vector<FunctionEntry> f_table;
    int f_num;
};
