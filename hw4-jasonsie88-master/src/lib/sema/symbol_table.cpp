#include "sema/symbol_table.hpp"

SymbolEntry::SymbolEntry(std::string new_p_name,int new_p_kind,int new_p_level,PType new_p_type,std::string new_p_attribute,int new_p_pass):name(new_p_name),kind(new_p_kind),level(new_p_level),type(new_p_type),attribute(new_p_attribute),pass(new_p_pass){}


SymbolTable::SymbolTable(int new_p_kind):kind(new_p_kind){}

void SymbolTable::increase_symbol(std::string new_p_name,int new_p_kind,int new_p_level,PType new_p_type,std::string new_p_attribute,int new_pass){
        SymbolEntry newSymbolEntry(new_p_name,new_p_kind,new_p_level,new_p_type,new_p_attribute,new_pass);
        entries.push_back(newSymbolEntry);
}

int SymbolTable::look_up(std::string name){       
        for(auto e:entries){
                if(name==e.name){
                        return e.kind;
                }
        }
        
        return ENTRY_KIND_NO_FOUND;
}

SymbolEntry SymbolTable::get_symbol_entry(std::string name){
         for(auto e:entries){
                if(name==e.name){
                        return e;
                }
        }
}

void SymbolTable::dumpDemarcation(const char ch){
        for (size_t i = 0; i < 110; ++i) {
                printf("%c",ch);
        }
        puts("");
}

void SymbolTable::print_table(){
        char * kind_str[]= {"program", "function", "parameter", "variable", "loop_var", "constant"};
        dumpDemarcation('=');
        printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type","Attribute");
        dumpDemarcation('-');
        for(auto e:entries){
                printf("%-33s", e.name.c_str());
                printf("%-11s", kind_str[e.kind]);
                printf("%d", e.level);
                if(e.level){
                        printf("%-10s", "(local)");
                }
                else{
                        printf("%-10s", "(global)");
                }
                        
                printf("%-17s", e.type.getPTypeCString());
                printf("%-11s", e.attribute.c_str());
                puts("");  
        }
        dumpDemarcation('-');
}


void SymbolManager::push_scope(SymbolTable *new_scope){
        s_tables.push_back(new_scope);
}

void SymbolManager::pop_scope(bool print){
        SymbolTable* to_free = s_tables.back();
        if(print){
                s_tables.back()->print_table();
        }
        s_tables.pop_back();
        free(to_free);
}

SymbolEntry SymbolManager::get_symbol_entry(std::string name){
        for(int i = s_tables.size()-1;i >= 0;i--){
                if(s_tables[i]->look_up(name)!= ENTRY_KIND_NO_FOUND){
                        return s_tables[i]->get_symbol_entry(name);
                }
        }
}

void SymbolManager::increase_symbol(std::string p_name, int p_kind, PType p_type, std::string p_attribute, int pass){
        s_tables.back()->increase_symbol(p_name, p_kind,s_tables.size()-1,p_type,p_attribute,pass);
}

int SymbolManager::look_up(std::string name, bool global){
        if(global){
                for(int i=s_tables.size()-1;i>=0;i--){
                        if(s_tables[i]->look_up(name)!= ENTRY_KIND_NO_FOUND){
                                return s_tables[i]->look_up(name);
                        }
                }
        }
        else{
                return s_tables.back()->look_up(name);
        }
        return ENTRY_KIND_NO_FOUND;
}

bool SymbolManager::look_up_for(std::string name){
        for(int i = s_tables.size()-1;i>=0;i--){
                if(s_tables[i]->get_kind() != 3){
                         continue;
                }
                       
                if(s_tables[i]->look_up(name) != ENTRY_KIND_NO_FOUND){
                        return true;
                }
                        
        }
        return false;
}



FunctionEntry::FunctionEntry(std::string name_n, std::vector<PType> paremeter_type_p, PType return_type_p):name(name_n), paremeter_type(paremeter_type_p), return_type(return_type_p){}


void FunctionManager::increase_function(std::string name_n, std::vector<PType> paremeter_type_p, PType return_type_p){
        FunctionEntry new_entry(name_n, paremeter_type_p, return_type_p);
        f_table.push_back(new_entry);
}

FunctionEntry FunctionManager::look_up(std::string name){
        for(auto f:f_table){
                if(f.name==name){
                        return f;
                }
        }
}


