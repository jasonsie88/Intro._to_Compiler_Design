#include "sema/SemanticAnalyzer.hpp"
#include "visitor/AstNodeInclude.hpp"
#define MAX_LINE_NUM        201
#define MAX_LINE_LENG       512

/*
* TODO:
*
* 1. Push a new symbol table if this node forms a scope.
* 2. Insert the symbol into current symbol table if this node is related to
*    declaration (ProgramNode, VariableNode, FunctionNode).
* 3. Travere child nodes of this node.
* 4. Perform semantic analyses of this node.
* 5. Pop the symbol table pushed at the 1st step.
*/

extern int32_t opt_dump_symbol;
extern bool error;
extern char code[MAX_LINE_NUM][MAX_LINE_LENG];
extern int in_func;

void SemanticAnalyzer::visit(ProgramNode &p_program) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    SymbolTable* new_table =new SymbolTable(1);
    symbol_manager_.push_scope(new_table);
    
    symbol_manager_.increase_symbol(p_program.getNameCString(),ENTRY_KIND_PROGRAM,PType::PrimitiveTypeEnum::kVoidType,"", 0);
    std::vector<PType> temp;
    PType temp2(PType::PrimitiveTypeEnum::kVoidType);
    function_manager_.increase_function(p_program.getNameCString(),temp, temp2);
    p_program.visitChildNodes(*this);
   
    symbol_manager_.pop_scope(opt_dump_symbol);

    

}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    int local_error = 0;
    int put = 0;
    if(symbol_manager_.look_up(p_variable.getNameCString(), 0) != ENTRY_KIND_NO_FOUND || symbol_manager_.look_up_for(p_variable.getNameCString())){
        print_error(p_variable.getLocation().line, p_variable.getLocation().col);
        fprintf(stderr, "symbol '%s' is redeclared\n", p_variable.getNameCString());
        print_error_code(p_variable.getLocation().line, p_variable.getLocation().col);
        local_error = 1;
        error = 1;
    }
    std::vector<uint64_t> dim_vet = p_variable.getType().get_dim();
    for(int i = 0; i < dim_vet.size(); i++){
        if(dim_vet[i] <= 0){
            print_error(p_variable.getLocation().line, p_variable.getLocation().col);
            fprintf(stderr, "'%s' declared as an array with an index that is not greater than 0\n", p_variable.getNameCString());
            print_error_code(p_variable.getLocation().line, p_variable.getLocation().col);
            put = 1;
            error = 1;
        }
    }

    int kind =  0;
    if(p_variable.is_constant()){
        kind = ENTRY_KIND_CONSTANT;
    }else if(symbol_manager_.get_top_kind() == TABLE_KIND_FUNCTION){
        kind = ENTRY_KIND_PARAMETER;
    }else if(symbol_manager_.get_top_kind() == TABLE_KIND_FORLOOP){
        kind = ENTRY_KIND_LOOP_VAR;
    }else{
        kind = ENTRY_KIND_VARIABLE;
    }
            
    
    if(local_error == 0 && put == 0){
        symbol_manager_.increase_symbol(p_variable.getNameCString(),kind,p_variable.getType(),p_variable.getconstantCString(),0);
    }
    else if(local_error == 0 && put == 1) {
        symbol_manager_.increase_symbol(p_variable.getNameCString(),kind,p_variable.getType(),p_variable.getconstantCString(),1);
    }
    p_variable.visitChildNodes(*this);//3
    
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_constant_value.visitChildNodes(*this);//3

    p_constant_value.set_type(p_constant_value.getTypePtr());

    
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    int local_error = 0;
    if(symbol_manager_.look_up(p_function.getNameCString(), 0) != ENTRY_KIND_NO_FOUND){
        print_error(p_function.getLocation().line, p_function.getLocation().col);
        fprintf(stderr, "symbol '%s' is redeclared\n", p_function.getNameCString());
        print_error_code(p_function.getLocation().line, p_function.getLocation().col);
        local_error = 1;
        error = 1;
    }
    
    std::vector<PType> temp;
    function_manager_.increase_function(p_function.getNameCString(),p_function.getParatype(), p_function.getReturnType());
    function_manager_.add_num();


    if(local_error == 0)
    {    
        symbol_manager_.increase_symbol(p_function.getNameCString(),ENTRY_KIND_FUNCTION,p_function.getReturnType(),p_function.getParatypeCString(),0);
    }
    SymbolTable* new_table =new SymbolTable(2);
    symbol_manager_.push_scope(new_table);

    
    p_function.visitChildNodes(*this);
    symbol_manager_.pop_scope(opt_dump_symbol);
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    int kind = symbol_manager_.get_top_kind();
    if(kind != 2){
        SymbolTable* new_table= new SymbolTable(0);
        symbol_manager_.push_scope(new_table);
    }
    symbol_manager_.state_change(TABLE_KIND_REST);
    p_compound_statement.visitChildNodes(*this);
    
    if(kind != 2){
        symbol_manager_.pop_scope(opt_dump_symbol);
    }
        

}

void SemanticAnalyzer::visit(PrintNode &p_print) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_print.visitChildNodes(*this);
    
    bool error_flag = p_print.get_expr_error();

    if(!error_flag){
        PType type_expr = p_print.get_expr_type();
        if( type_expr.getPrimitiveType() == PType::PrimitiveTypeEnum::kVoidType|| type_expr.get_dim().size() != 0 ){
            print_error(p_print.m_target->getLocation().line, p_print.m_target->getLocation().col);
            fprintf(stderr, "expression of print statement must be scalar type\n");
            print_error_code(p_print.m_target->getLocation().line, p_print.m_target->getLocation().col);
            error_flag = 1;
            error = 1;
        }
    }
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_bin_op.visitChildNodes(*this);
    std::string op(p_bin_op.getOpCString());

    PType left_op = p_bin_op.get_left_type();
    PType right_op = p_bin_op.get_right_type();
 
    
    if(op == "-" || op =="+" || op == "/" || op == "*"){
        if((right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType && right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType)
            || right_op.get_dim().size() > 0
            ||(left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType && left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType) 
            || left_op.get_dim().size() > 0
        ){
            if(op != "+" || left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kStringType
            || left_op.get_dim().size() > 0
            || right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kStringType
            || right_op.get_dim().size() > 0){
                print_error(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
                fprintf(stderr, "invalid operands to binary operator '%s' ('%s' and '%s')\n", op.c_str(), left_op.getPTypeCString(), right_op.getPTypeCString());
                print_error_code(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
                error = 1;
                p_bin_op.set_error();
                PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kStringType));
                p_bin_op.set_type(ptr);
            }
            else{
                PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kStringType));
                p_bin_op.set_type(ptr);
            }
        }
        else if( right_op.getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType|| left_op.getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType ){
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kRealType));
            p_bin_op.set_type(ptr);
        }
        else{
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kIntegerType));
            p_bin_op.set_type(ptr);
        }
    }
    else if(op == "mod"){
        if(right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType
           || right_op.get_dim().size() > 0 
           || left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType 
           || left_op.get_dim().size() > 0
        ){
            print_error(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
            fprintf(stderr, "invalid operands to binary operator '%s' ('%s' and '%s')\n", op.c_str(), left_op.getPTypeCString(), right_op.getPTypeCString());
            print_error_code(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
            error = 1;
            p_bin_op.set_error();
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kIntegerType));
            p_bin_op.set_type(ptr);
        }
        else{
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kIntegerType));
            p_bin_op.set_type(ptr);
        }
    }
    else if(op == "or" || op == "not" || op == "and"){
        if(right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType
            || right_op.get_dim().size() > 0
            || left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType
            || left_op.get_dim().size() > 0
        ){
            print_error(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
            fprintf(stderr, "invalid operands to binary operator '%s' ('%s' and '%s')\n", op.c_str(), left_op.getPTypeCString(), right_op.getPTypeCString());
            print_error_code(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
            error = 1;
            p_bin_op.set_error();
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kBoolType));
            p_bin_op.set_type(ptr);
        }
        else{
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kBoolType));
            p_bin_op.set_type(ptr);
        }
    }
    else if(op == ">" || op == ">=" || op == "=" || op == "<=" || op == "<" || op == "<>"){
        if((right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType && right_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType)
           || right_op.get_dim().size() > 0
           || (left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType && left_op.getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType) 
           || left_op.get_dim().size() > 0
       ){
            print_error(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
            fprintf(stderr, "invalid operands to binary operator '%s' ('%s' and '%s')\n", op.c_str(), left_op.getPTypeCString(), right_op.getPTypeCString());
            print_error_code(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
            error = 1;
            p_bin_op.set_error();
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kBoolType));
            p_bin_op.set_type(ptr);
        }
        else{
            PTypeSharedPtr ptr(new PType(PType::PrimitiveTypeEnum::kBoolType));
            p_bin_op.set_type(ptr);
        }
    }
    
    
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */

    p_un_op.visitChildNodes(*this);
    std::string op(p_un_op.getOpCString());
    PType oparand = p_un_op.get_oparand_type();
    if(op == "neg"){
        if(oparand.getPrimitiveType() == PType::PrimitiveTypeEnum::kIntegerType && oparand.get_dim().size() == 0){
            p_un_op.set_error();
            PTypeSharedPtr temp(new PType(PType::PrimitiveTypeEnum::kIntegerType));
            p_un_op.set_type(temp);
        }else if(oparand.getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType&& oparand.get_dim().size() == 0){
            p_un_op.set_error();
            PTypeSharedPtr temp(new PType(PType::PrimitiveTypeEnum::kRealType));
            p_un_op.set_type(temp);
        }else{
            print_error(p_un_op.getLocation().line, p_un_op.getLocation().col);
            fprintf(stderr, "invalid operand to unary operator '%s' ('%s')\n", op.c_str(), oparand.getPTypeCString());
            print_error_code(p_un_op.getLocation().line, p_un_op.getLocation().col);
            error = 1;
            p_un_op.set_error();
            PTypeSharedPtr temp(new PType(PType::PrimitiveTypeEnum::kRealType));
            p_un_op.set_type(temp);
        }
    }else if(op == "not"){
        if(oparand.getPrimitiveType() == PType::PrimitiveTypeEnum::kBoolType && oparand.get_dim().size() == 0){
            PTypeSharedPtr temp(new PType(PType::PrimitiveTypeEnum::kBoolType));
            p_un_op.set_type(temp);
        }
        else{
            print_error(p_un_op.getLocation().line, p_un_op.getLocation().col);
            fprintf(stderr, "invalid operand to unary operator '%s' ('%s')\n", op.c_str(), oparand.getPTypeCString());
            print_error_code(p_un_op.getLocation().line, p_un_op.getLocation().col);
            error = 1;
            p_un_op.set_error();
            PTypeSharedPtr temp(new PType(PType::PrimitiveTypeEnum::kBoolType));
            p_un_op.set_type(temp);
        }
    }

    
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_func_invocation.visitChildNodes(*this);

    bool error_flag = 0;
    int kind = symbol_manager_.look_up(p_func_invocation.getNameCString(), 1);
    
    //fprintf(stderr, "\nline: %d, name: %s", p_func_invocation.getLocation().line, p_func_invocation.getNameCString());

    if(kind == ENTRY_KIND_NO_FOUND){
        print_error(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        fprintf(stderr, "use of undeclared symbol '%s'\n", p_func_invocation.getNameCString());
        print_error_code(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        p_func_invocation.set_error();
        error_flag = 1;
        error = 1;
    }else if(kind != ENTRY_KIND_FUNCTION){
        print_error(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        fprintf(stderr, "call of non-function symbol '%s'\n", p_func_invocation.getNameCString());
        print_error_code(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        p_func_invocation.set_error();
        error_flag = 1;
        error = 1;
    }else{
        FunctionEntry f_e = function_manager_.look_up(p_func_invocation.getNameCString());
        std::vector<PType> para_vec = p_func_invocation.getParatype();
        if(f_e.paremeter_type.size() != para_vec.size()){
            print_error(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
            fprintf(stderr, "too few/much arguments provided for function '%s'\n", p_func_invocation.getNameCString());
            print_error_code(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
            p_func_invocation.set_error();
            error_flag = 1;
            error = 1;
        }else{
            for(int i = 0; i < para_vec.size(); i++){
                if(f_e.paremeter_type[i] != para_vec[i]){
                    print_error(p_func_invocation.m_arguments[i]->getLocation().line, p_func_invocation.m_arguments[i]->getLocation().col);
                    fprintf(stderr, "incompatible type passing '%s' to parameter of type '%s'\n"
                                , para_vec[i].getPTypeCString(), f_e.paremeter_type[i].getPTypeCString());
                    print_error_code(p_func_invocation.m_arguments[i]->getLocation().line, p_func_invocation.m_arguments[i]->getLocation().col);
                    p_func_invocation.set_error();
                    error_flag = 1;
                    error = 1;
                    break;
                }
            }
        }
    }
    
    if(error_flag ==0 && symbol_manager_.get_symbol_entry(p_func_invocation.getNameCString()).pass == 0){
        PType* ptr = new PType(symbol_manager_.get_symbol_entry(p_func_invocation.getNameCString()).type);
        std::vector<uint64_t> new_dim;
        ptr->setDimensions(new_dim);
        PTypeSharedPtr ptrr = (PTypeSharedPtr) ptr;
        p_func_invocation.set_type(ptrr);
    }

    
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */

    p_variable_ref.visitChildNodes(*this);

    bool error_flag = p_variable_ref.get_error_inner();
    int kind = symbol_manager_.look_up(p_variable_ref.getNameCString(), 1);
    if(kind == ENTRY_KIND_NO_FOUND){
        print_error(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        fprintf(stderr, "use of undeclared symbol '%s'\n", p_variable_ref.getNameCString());
        print_error_code(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        p_variable_ref.set_error();
        error_flag = 1;
        error = 1;
    }else if(kind == ENTRY_KIND_PROGRAM || kind == ENTRY_KIND_FUNCTION){

        print_error(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        fprintf(stderr, "use of non-variable symbol '%s'\n", p_variable_ref.getNameCString());
        print_error_code(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        p_variable_ref.set_error();
        error_flag = 1;
        error = 1;

    }else if(symbol_manager_.get_symbol_entry(p_variable_ref.getNameCString()).pass == 1){

        p_variable_ref.set_error();

    }
        
    if(!error_flag && symbol_manager_.get_symbol_entry(p_variable_ref.getNameCString()).pass==0){

        std::vector<uint64_t> tmp_dim = symbol_manager_.get_symbol_entry(p_variable_ref.getNameCString()).type.get_dim();
        std::vector<uint64_t> new_dim;
        for(int i = p_variable_ref.get_dim(); i < tmp_dim.size(); i++){
            new_dim.push_back(tmp_dim[i]);
        }

        PType* ptr = new PType(symbol_manager_.get_symbol_entry(p_variable_ref.getNameCString()).type);
        ptr->setDimensions(new_dim);

        PTypeSharedPtr ptrr = (PTypeSharedPtr) ptr;
        p_variable_ref.set_type(ptrr);

        std::vector<PType::PrimitiveTypeEnum> variable_ref_expr = p_variable_ref.get_Exprs();
        for(int i = 0; i < variable_ref_expr.size(); i++){
            if(variable_ref_expr[i] != PType::PrimitiveTypeEnum::kIntegerType){
                print_error(p_variable_ref.getLocation().line, p_variable_ref.m_indices[i]->getLocation().col);
                fprintf(stderr, "index of array reference must be an integer\n");
                print_error_code(p_variable_ref.getLocation().line, p_variable_ref.m_indices[i]->getLocation().col);
                p_variable_ref.set_error();
                error_flag= 1;
                error = 1;
            }
        }

        std::vector<uint64_t> dim2 = symbol_manager_.get_symbol_entry(p_variable_ref.getNameCString()).type.get_dim();
        if(p_variable_ref.get_dim() > dim2.size()){
            print_error(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
            fprintf(stderr, "there is an over array subscript on '%s'\n", p_variable_ref.getNameCString());
            print_error_code(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
            p_variable_ref.set_error();
            error_flag = 1;
            error = 1;
        }
    }
    
    
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_assignment.visitChildNodes(*this);

    int kind = symbol_manager_.look_up(p_assignment.m_lvalue->getNameCString(), 1);

    bool error_flag = p_assignment.m_lvalue->get_error();
    bool error_flag1 = p_assignment.m_expr->get_error();

    if(error_flag == 0){
        PType var_type = p_assignment.m_lvalue->get_type();
        if(p_assignment.m_lvalue->get_type().get_dim().size() != 0){
            print_error(p_assignment.m_lvalue->getLocation().line, p_assignment.m_lvalue->getLocation().col);
            fprintf(stderr, "array assignment is not allowed\n");
            print_error_code(p_assignment.m_lvalue->getLocation().line, p_assignment.m_lvalue->getLocation().col);
            error_flag = 1;
            error = 1;
        }
        else if(kind == ENTRY_KIND_CONSTANT){
            print_error(p_assignment.m_lvalue->getLocation().line, p_assignment.m_lvalue->getLocation().col);
            fprintf(stderr, "cannot assign to variable '%s' which is a constant\n", p_assignment.m_lvalue->getNameCString());
            print_error_code(p_assignment.m_lvalue->getLocation().line, p_assignment.m_lvalue->getLocation().col);
            error_flag = 1;
            error = 1;
        }
        else if(kind == ENTRY_KIND_LOOP_VAR && symbol_manager_.get_top_kind() != TABLE_KIND_FORLOOP){
            print_error(p_assignment.m_lvalue->getLocation().line, p_assignment.m_lvalue->getLocation().col);
            fprintf(stderr, "the value of loop variable cannot be modified inside the loop body\n");
            print_error_code(p_assignment.m_lvalue->getLocation().line, p_assignment.m_lvalue->getLocation().col);
            error_flag = 1;
            error = 1;
        }
    }
    if(error_flag1 == 0 && error_flag == 0){
        PType var = p_assignment.m_lvalue->get_type();
        PType expr = p_assignment.m_expr->get_type();
        if(p_assignment.m_expr->get_type().get_dim().size() != 0){
            print_error(p_assignment.m_expr->getLocation().line, p_assignment.m_expr->getLocation().col);
            fprintf(stderr, "array assignment is not allowed\n");
            print_error_code(p_assignment.m_expr->getLocation().line, p_assignment.m_expr->getLocation().col);
            error_flag = 1;
            error = 1;
        }
        else if(( expr.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType && var.getPrimitiveType() == PType::PrimitiveTypeEnum::kIntegerType )
            || ( expr.getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType && var.getPrimitiveType() == PType::PrimitiveTypeEnum::kBoolType )
          ||  (expr.getPrimitiveType() != PType::PrimitiveTypeEnum::kStringType && var.getPrimitiveType() == PType::PrimitiveTypeEnum::kStringType )
        ||((expr.getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType) && (expr.getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType)
             &&var.getPrimitiveType() == PType::PrimitiveTypeEnum::kRealType 
            )
        ){
            print_error(p_assignment.getLocation().line, p_assignment.getLocation().col);
            fprintf(stderr, "assigning to '%s' from incompatible type '%s'\n", var.getPTypeCString(), expr.getPTypeCString());
            print_error_code(p_assignment.getLocation().line, p_assignment.getLocation().col);
            error_flag = 1;
            error = 1;
        }

    }

    
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_read.visitChildNodes(*this);


    bool error_flag = p_read.get_expr_error();
    int kind = symbol_manager_.look_up(p_read.m_target->getNameCString(),1);

    if(!error_flag){
        PType type_expr = p_read.get_expr_type();
        if(type_expr.get_dim().size() != 0){
            print_error(p_read.m_target->getLocation().line, p_read.m_target->getLocation().col);
            fprintf(stderr, "variable reference of read statement must be scalar type\n");
            print_error_code(p_read.m_target->getLocation().line, p_read.m_target->getLocation().col);
            error = 1;
            error_flag = 1;
        }else if(kind == ENTRY_KIND_LOOP_VAR || kind == ENTRY_KIND_CONSTANT){
            print_error(p_read.m_target->getLocation().line, p_read.m_target->getLocation().col);
            fprintf(stderr, "variable reference of read statement cannot be a constant or loop variable\n");
            print_error_code(p_read.m_target->getLocation().line, p_read.m_target->getLocation().col);
            error = 1;
            error_flag = 1;
        }
    }

    
}

void SemanticAnalyzer::visit(IfNode &p_if) {
       /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_if.visitChildNodes(*this);

    bool error_flag = p_if.m_condition->get_error();

    if(error_flag == 0){
        PType expr_type = p_if.m_condition->get_type();
        if(expr_type.getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType || expr_type.get_dim().size() != 0){
            print_error(p_if.m_condition->getLocation().line, p_if.m_condition->getLocation().col);
            fprintf(stderr, "the expression of condition must be boolean type\n");
            print_error_code(p_if.m_condition->getLocation().line, p_if.m_condition->getLocation().col);
            error_flag = 1;
            error = 1;
        }
    }

    
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_while.visitChildNodes(*this);

    bool error_flag = p_while.m_condition->get_error();

    if(!error_flag){
        PType type_expr = p_while.m_condition->get_type();
        if(type_expr.get_dim().size()!= 0 || type_expr.getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType ){
            print_error(p_while.m_condition->getLocation().line,p_while.m_condition->getLocation().col);
            fprintf(stderr, "the expression of condition must be boolean type\n");
            print_error_code(p_while.m_condition->getLocation().line,p_while.m_condition->getLocation().col);
            error_flag= 1;
            error= 1;
        }
    }

    
}

void SemanticAnalyzer::visit(ForNode &p_for) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    SymbolTable* new_table=new SymbolTable(3);
    symbol_manager_.push_scope(new_table);
    p_for.visitChildNodes(*this);

    int left = p_for.m_left;
    int right = p_for.m_condition->m_constant->m_value.integer;
    if(left > right){
        print_error(p_for.getLocation().line, p_for.getLocation().col);
        fprintf(stderr, "the lower bound and upper bound of iteration count must be in the incremental order\n");
        print_error_code(p_for.getLocation().line, p_for.getLocation().col);
        error = 1;
    }

    symbol_manager_.pop_scope(opt_dump_symbol);
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
        /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_return.visitChildNodes(*this);

    bool error_flag = 0;
    int look = function_manager_.get_func_num();
    if(in_func==0){
        look = 0;
    }
    FunctionEntry f_e = function_manager_.look_up(look);
    if(f_e.return_type.getPrimitiveType()== PType::PrimitiveTypeEnum::kVoidType && p_return.m_ret_val->get_type_ptr() != nullptr){
        print_error(p_return.getLocation().line, p_return.getLocation().col);
        fprintf(stderr, "program/procedure should not return a value\n");
        print_error_code(p_return.getLocation().line, p_return.getLocation().col);
        error_flag = 1;
        error = 1;
    }
    if(!error_flag){
        if((f_e.return_type.getPrimitiveType() == PType::PrimitiveTypeEnum::kIntegerType
        && (p_return.m_ret_val->get_type().getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType && p_return.m_ret_val->get_type().getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType))
        ||f_e.return_type != p_return.m_ret_val->get_type()){
            print_error(p_return.m_ret_val->getLocation().line, p_return.m_ret_val->getLocation().col);
            fprintf(stderr, "return '%s' from a function with return type '%s'\n",p_return.m_ret_val->get_type().getPTypeCString(),f_e.return_type.getPTypeCString());
            print_error_code(p_return.m_ret_val->getLocation().line, p_return.m_ret_val->getLocation().col);
            error_flag=1;
            error=1;
        }
    }
}

void SemanticAnalyzer::print_error(const uint32_t line, const uint32_t col)
{
    fprintf(stderr, "<Error> Found in line %d, column %d: ", line, col);
}

void SemanticAnalyzer::print_error_code(const uint32_t line, const uint32_t col)
{
    fprintf(stderr, "    %s\n    ", code[line]);
    for(int i = 0; i < col - 1; i++)
        fprintf(stderr, " ");
    fprintf(stderr, "^\n");
}