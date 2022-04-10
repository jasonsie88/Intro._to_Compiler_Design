#ifndef CODEGEN_CODE_GENERATOR_H
#define CODEGEN_CODE_GENERATOR_H

#include "sema/SymbolTable.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>

class CodeGenerator final : public AstNodeVisitor {
  private:
    const SymbolManager *m_symbol_manager_ptr;
    std::string m_source_file_path;
    std::unique_ptr<FILE> m_output_file;
    int label=0;
    int offset = 0;
    bool is_local = 0;
    bool is_read = 0;

  public:
    ~CodeGenerator();
    CodeGenerator(const std::string source_file_name,
                  const std::string save_path,
                  const SymbolManager *const p_symbol_manager);

    void visit(ProgramNode &p_program) override;
    void visit(DeclNode &p_decl) override;
    void visit(VariableNode &p_variable) override;
    void visit(ConstantValueNode &p_constant_value) override;
    void visit(FunctionNode &p_function) override;
    void visit(CompoundStatementNode &p_compound_statement) override;
    void visit(PrintNode &p_print) override;
    void visit(BinaryOperatorNode &p_bin_op) override;
    void visit(UnaryOperatorNode &p_un_op) override;
    void visit(FunctionInvocationNode &p_func_invocation) override;
    void visit(VariableReferenceNode &p_variable_ref) override;
    void visit(AssignmentNode &p_assignment) override;
    void visit(ReadNode &p_read) override;
    void visit(IfNode &p_if) override;
    void visit(WhileNode &p_while) override;
    void visit(ForNode &p_for) override;
    void visit(ReturnNode &p_return) override;
    void dumpInstructions(const char *format, ...);
    int newlabel(){return label++;}
    void function_prologue();
    void function_epilogue(const char*  function_name);
    void offset_plus(){offset+=4;}
    void offset_minus(){offset-=4;}
    void set_is_local(){is_local=1;}
    void clear_is_local(){is_local=0;}
    bool get_is_local(){return is_local;}
    void set_is_read(){is_read=1;}
    void clear_is_read(){is_read=0;}
    bool get_is_read(){return is_read;}

};

#endif
