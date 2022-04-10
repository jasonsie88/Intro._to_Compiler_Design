#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdio>

CodeGenerator::CodeGenerator(const std::string source_file_name,
                             const std::string save_path,
                             const SymbolManager *const p_symbol_manager)
    : m_symbol_manager_ptr(p_symbol_manager),
      m_source_file_path(source_file_name) {
    // FIXME: assume that the source file is always xxxx.p
    const std::string &real_path =
        (save_path == "") ? std::string{"."} : save_path;
    auto slash_pos = source_file_name.rfind("/");
    auto dot_pos = source_file_name.rfind(".");

    if (slash_pos != std::string::npos) {
        ++slash_pos;
    } else {
        slash_pos = 0;
    }
    std::string output_file_path(
        real_path + "/" +
        source_file_name.substr(slash_pos, dot_pos - slash_pos) + ".S");
    m_output_file.reset(fopen(output_file_path.c_str(), "w"));
    assert(m_output_file.get() && "Failed to open output file");
}

CodeGenerator::~CodeGenerator() {
   fclose(this->m_output_file.get());
}

void CodeGenerator::function_prologue(){
    offset = 0;
    for(int i=0;i<2;i++){
        offset_minus();
    }
    dumpInstructions("addi sp, sp, -128\n");
    dumpInstructions("sw ra, 124(sp)\n");
    dumpInstructions("sw s0, 120(sp)\n");
    dumpInstructions("addi s0, sp, 128\n");
}

void CodeGenerator:: function_epilogue(const char* function_name){
    dumpInstructions("lw ra, 124(sp)\n");
    dumpInstructions("lw s0, 120(sp)\n");
    dumpInstructions("addi sp, sp, 128\n");
    dumpInstructions("jr ra\n");
    dumpInstructions(".size %s, .-%s\n",function_name,function_name);
    for(int i=0;i<2;i++){
        offset_plus();
    }
}


void CodeGenerator:: dumpInstructions(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(this->m_output_file.get(), format, args);
    va_end(args);
}

void CodeGenerator::visit(ProgramNode &p_program) {
    // Generate RISC-V instructions for program header
    // clang-format off
    /*
    constexpr const char*const riscv_assembly_file_prologue =
        "    .file \"%s\"\n"
        "    .option nopic\n"
        ".section    .text\n"
        "    .align 2\n";
    // clang-format on
    dumpInstructions(m_output_file.get(), riscv_assembly_file_prologue,
                     m_source_file_path.c_str());
    */
    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_program.getSymbolTable());

    dumpInstructions("%s%s%s%s","   .file \"", this->m_source_file_path, "\"\n","   .option nopic\n");

    auto visit_ast_node = [&](auto &ast_node) { ast_node->accept(*this); };
    for_each(p_program.getDeclNodes().begin(), p_program.getDeclNodes().end(),
             visit_ast_node);
    for_each(p_program.getFuncNodes().begin(), p_program.getFuncNodes().end(),
             visit_ast_node);
    
    dumpInstructions("\n.text\n");
    dumpInstructions("  .align  2\n");
    dumpInstructions("  .global main\n");
    dumpInstructions("  .type   main, @function\n");
    dumpInstructions("\nmain:\n");
    function_prologue();
    const_cast<CompoundStatementNode &>(p_program.getBody()).accept(*this);
    function_epilogue("main");

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_program.getSymbolTable());
}

void CodeGenerator::visit(DeclNode &p_decl) {p_decl.visitChildNodes(*this);}

void CodeGenerator::visit(VariableNode &p_variable) {
    if(get_is_local()==0){
        if(m_symbol_manager_ptr->lookup(p_variable.getName())->getKind()==SymbolEntry::KindEnum::kVariableKind){
            dumpInstructions("\n.comm %s, 4 ,4\n",p_variable.getNameCString());
        }else if(m_symbol_manager_ptr->lookup(p_variable.getName())->getKind() == SymbolEntry::KindEnum::kConstantKind){
            dumpInstructions("\n.section    .rodata \n");
            dumpInstructions("    .align 2\n");
            dumpInstructions("    .globl %s\n", p_variable.getNameCString());
            dumpInstructions("    .type %s, @object\n", p_variable.getNameCString());
            dumpInstructions("%s:\n", p_variable.getNameCString());
            dumpInstructions("    .word %s \n",p_variable.getConstantPtr()->getConstantValueCString());
        }
    }else{
        SymbolEntry *d_entry=(SymbolEntry*) m_symbol_manager_ptr->lookup(p_variable.getName());
        if(d_entry->getKind()== SymbolEntry::KindEnum::kVariableKind){
            offset_minus();
            d_entry->set_offset(offset);
            //dumpInstructions("#Declare local const variable:%s, offset: %d\n",d_entry->getNameCString(),d_entry->get_offset());
        }else if(d_entry->getKind() == SymbolEntry::KindEnum::kConstantKind ){
            offset_minus();
            d_entry->set_offset(offset);
            //dumpInstructions("#Declare local const variable:%s, offset: %d\n",d_entry->getNameCString(),d_entry->get_offset());
            dumpInstructions("addi t0, s0, %d\n", offset);
            dumpInstructions("addi sp, sp, -4\n");
            dumpInstructions("sw t0, 0(sp)\n");
            dumpInstructions("li t0, %s\n", p_variable.getConstantPtr()->getConstantValueCString());
            dumpInstructions("addi sp, sp, -4\n");
            dumpInstructions("sw t0, 0(sp)\n");
            dumpInstructions("lw t0, 0(sp)\n");
            dumpInstructions("addi sp, sp, 4\n");
            dumpInstructions("lw t1, 0(sp)\n");
            dumpInstructions("addi sp, sp, 4\n");
            dumpInstructions("sw t0, 0(t1)\n");
        }else if(d_entry->getKind() == SymbolEntry::KindEnum::kParameterKind ){
            offset_minus();
            d_entry->set_offset(offset);
        }else if(d_entry->getKind() == SymbolEntry::KindEnum::kLoopVarKind ){
            offset_minus();
            d_entry->set_offset(offset);
        }
    }

}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    if(p_constant_value.getTypePtr()->isInteger()){
        dumpInstructions("li t0, %s\n", p_constant_value.getConstantValueCString());
        dumpInstructions("addi sp, sp, -4\n");
        dumpInstructions("sw t0, 0(sp)\n");
    }else if(p_constant_value.getTypePtr()->isBool()){
        if(p_constant_value.getConstantValueCString()==std::string("false")){
            dumpInstructions("li t0, 0\n");
        }else{
            dumpInstructions("li t0, 1\n");
        }
        dumpInstructions("addi sp, sp, -4\n");
        dumpInstructions("sw t0, 0(sp)\n");
    }else if(p_constant_value.getTypePtr()->isString()){

    }
}

void CodeGenerator::visit(FunctionNode &p_function) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_function.getSymbolTable());
    set_is_local();
    //p_function.visitChildNodes(*this);
    dumpInstructions("\n.text\n");
    dumpInstructions("  .align  2\n");
    dumpInstructions("  .global %s\n", p_function.getNameCString());
    dumpInstructions("  .type   %s, @function\n", p_function.getNameCString());
    dumpInstructions("\n%s:\n", p_function.getNameCString());
    function_prologue();
    //dumpInstructions("#Parameter num: %d\n", p_function.get_parameter_num());
    for (auto &p:p_function.m_parameters){
        p->accept(*this);
    }
    if(p_function.get_parameter_num()<=8){
        for(int i=0;i<p_function.get_parameter_num();i++){
            std::string p_name=p_function.getSymbolTable()->getEntries().at(i)->getName();
            SymbolEntry* p_entry = (SymbolEntry* ) m_symbol_manager_ptr->lookup(p_name);
            dumpInstructions("sw a%d, %d(s0)\n", i, p_entry->get_offset());
        }
    }else{
        int size_8 = p_function.get_parameter_num()*4;
        for(int i=0;i<p_function.get_parameter_num()/*size_8*/;i++){
            std::string p_name=p_function.getSymbolTable()->getEntries().at(i)->getName();
            SymbolEntry* p_entry = (SymbolEntry* ) m_symbol_manager_ptr->lookup(p_name);
            dumpInstructions("lw t0, %d(s0)\n",size_8-4);
            dumpInstructions("sw t0, %d(s0)\n", p_entry->get_offset());
            size_8-=4;
        }
    }
    if(p_function.m_body){
        p_function.m_body->accept(*this);
    }
    function_epilogue(p_function.getNameCString());
    // Remove the entries in the hash table
    clear_is_local();
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_compound_statement.getSymbolTable());
    set_is_local();
    p_compound_statement.visitChildNodes(*this);

    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(
        p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {
    //dumpInstructions("#Print start\n");
    p_print.visitChildNodes(*this);
    dumpInstructions("lw a0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("jal ra, printInt\n");
    //dumpInstructions("#Print end\n");
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);
    std::string op_(p_bin_op.getOpCString());
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("lw t1, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    if(op_=="+"){
        dumpInstructions("add t0, t1, t0 \n");
    }else if(op_=="-"){
        dumpInstructions("sub t0, t1, t0 \n");
    }else if(op_=="*"){
        dumpInstructions("mul t0, t1, t0 \n");
    }else if(op_=="/"){
        dumpInstructions("div t0, t1, t0 \n");
    }else if(op_=="mod"){
        dumpInstructions("rem t0, t1, t0 \n");
    }else if(op_=="<"){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("blt t1, t0, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_=="<="){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("ble t1, t0, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_=="="){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("beq t0, t1, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_==">="){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("bge t1, t0, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_==">"){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("bgt t1, t0, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_=="<>"){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("bne t1, t0, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_=="and"){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("beqz t1, L%d\n",true_label);
        dumpInstructions("beqz t0, L%d\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("L%d:\n",false_label);
    }else if(op_=="or"){
        int false_label = newlabel();
        int true_label = newlabel();
        dumpInstructions("bneqz t1, L%d\n",true_label);
        dumpInstructions("bneqz t0, L%d\n",true_label);
        dumpInstructions("li t0, 0\n");
        dumpInstructions("j L%d\n",false_label);
        dumpInstructions("L%d:\n",true_label);
        dumpInstructions("li t0, 1\n");
        dumpInstructions("L%d:\n",false_label);
    }
    dumpInstructions("addi sp, sp, -4\n");
    dumpInstructions("sw t0, 0(sp)\n");
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {

    p_un_op.visitChildNodes(*this);
    std::string op_(p_un_op.getOpCString());
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    if(op_=="neg"){
        dumpInstructions("li t1, 0\n");
        dumpInstructions("sub t0, t1, t0 \n");
    }else if(op_=="not"){
        dumpInstructions("li t1, 1\n");
        dumpInstructions("sub t0, t1, t0 \n");
    }
    dumpInstructions("addi sp, sp, -4\n");
    dumpInstructions("sw t0, 0(sp)\n");
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    //dumpInstructions("#Function call start\n");
    p_func_invocation.visitChildNodes(*this);
    int arg_num = p_func_invocation.get_arg_num();
    int size_8 = 0;
    if(arg_num <= 8){
        for(int i=arg_num -1;i>=0;i--){
            dumpInstructions("lw a%d, 0(sp)\n", i);
            dumpInstructions("addi sp, sp, 4\n");
        }
    }else{
        size_8 = 4*arg_num;
    }
    dumpInstructions("jal ra, %s\n", p_func_invocation.getNameCString());
    if(size_8>0){
        dumpInstructions("addi sp, sp, %d\n",size_8);
    }
    dumpInstructions("mv t0, a0\n");
    dumpInstructions("addi sp, sp, -4\n");
    dumpInstructions("sw t0, 0(sp)\n");
    //dumpInstructions("#Function call end\n");
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    p_variable_ref.visitChildNodes(*this);
    SymbolEntry *v_entry = (SymbolEntry*) m_symbol_manager_ptr->lookup(p_variable_ref.getName());
    //dumpInstructions("#Variable reference:%s, offset: %d\n",v_entry->getNameCString(),v_entry->get_offset());
    if(get_is_read()){
        if(v_entry->getLevel()==0){
            dumpInstructions("la t0, %s\n",p_variable_ref.getNameCString());
            dumpInstructions("addi sp, sp, -4\n");
            dumpInstructions("sw t0, 0(sp)\n");
        }else{
            dumpInstructions("addi t0, s0, %d\n",v_entry->get_offset());
            dumpInstructions("addi sp, sp, -4\n",p_variable_ref.getNameCString());
            dumpInstructions("sw t0, 0(sp)\n");
        }

    }else{
        if(v_entry->getLevel()==0){
            dumpInstructions("la t0, %s\n", p_variable_ref.getNameCString());
            dumpInstructions("lw t1, 0(t0)\n");
            dumpInstructions("mv t0, t1\n");
            dumpInstructions("addi sp, sp, -4\n");
            dumpInstructions("sw t0, 0(sp)\n");
        }else{
            dumpInstructions("lw t0, %d(s0)\n",v_entry->get_offset());
            dumpInstructions("addi sp, sp, -4 \n");
            dumpInstructions("sw t0, 0(sp)\n");
        }
    }
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    //dumpInstructions("#Assignment start\n");
    set_is_read();
    p_assignment.m_lvalue->accept(*this);
    clear_is_read();
    p_assignment.m_expr->accept(*this);
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("lw t1, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("sw t0, 0(t1)\n");
    //dumpInstructions("#Assignment end\n");
}

void CodeGenerator::visit(ReadNode &p_read) {
    //dumpInstructions("#Read start\n");
    set_is_read();
    p_read.visitChildNodes(*this);
    clear_is_read();
    dumpInstructions("jal ra, readInt\n");
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("sw a0, 0(t0)\n");
    //dumpInstructions("#Read end\n");
}

void CodeGenerator::visit(IfNode &p_if) {
    int if_label = newlabel();
    int else_label = newlabel();
    int end_label = newlabel();
    p_if.m_condition->accept(*this);
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("beqz t0, L%d\n",else_label);
    //dumpInstructions("#Label for if\n");
    dumpInstructions("L%d:\n",if_label);
    p_if.m_body->accept(*this);
    dumpInstructions("j L%d\n",end_label);
    //dumpInstructions("#Label for else\n");
    dumpInstructions("L%d:\n", else_label);
    if(p_if.m_else_body){
        p_if.m_else_body->accept(*this);
    }
    //dumpInstructions("#Label for out\n");
    dumpInstructions("L%d:\n",end_label);
}

void CodeGenerator::visit(WhileNode &p_while) {
    int while_label = newlabel();
    int end_label =newlabel();
    dumpInstructions("L%d:\n",while_label);
    p_while.m_condition->accept(*this);
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("beqz t0, L%d\n",end_label);
    p_while.m_body->accept(*this);
    dumpInstructions("j L%d\n", while_label);
    dumpInstructions("L%d:\n",end_label);
}

void CodeGenerator::visit(ForNode &p_for) {
    // Reconstruct the hash table for looking up the symbol entry
    m_symbol_manager_ptr->reconstructHashTableFromSymbolTable(
        p_for.getSymbolTable());
    SymbolEntry * loop_entry = (SymbolEntry*) m_symbol_manager_ptr->lookup(p_for.m_loop_var_decl-> get_first_Cstring());
    //dumpInstructions("#For start\n");
    //dumpInstructions("#For_var name: %s\n", loop_entry->getNameCString());
    //p_for.visitChildNodes(*this);
    int for_label = newlabel();
    int end_label = newlabel();
    p_for.m_loop_var_decl->accept(*this);
    p_for.m_init_stmt->accept(*this);
    dumpInstructions("L%d:\n",for_label);
    p_for.m_end_condition->accept(*this);
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("lw t1, %d(s0)\n",loop_entry->get_offset());
    dumpInstructions("bge t1, t0, L%d\n",end_label);
    p_for.m_body->accept(*this);
    dumpInstructions("lw t1, %d(s0)\n",loop_entry->get_offset());
    dumpInstructions("addi t1, t1, 1\n");
    dumpInstructions("sw t1, %d(s0)\n",loop_entry->get_offset());
    dumpInstructions("j L%d\n",for_label);
    //dumpInstructions("#For end\n");
    dumpInstructions("L%d:\n",end_label);
    // Remove the entries in the hash table
    m_symbol_manager_ptr->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {
    p_return.visitChildNodes(*this);
    //dumpInstructions("#Return start\n");
    dumpInstructions("lw t0, 0(sp)\n");
    dumpInstructions("addi sp, sp, 4\n");
    dumpInstructions("mv a0, t0\n");
    //dumpInstructions("#Return end\n");
}
