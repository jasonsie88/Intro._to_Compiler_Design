# hw3 report

|||
|-:|:-|
|Name|謝承恩|
|ID|0816183|

## How much time did you spend on this project

20 小時

## Project overview
這次的 project 我實作了兩大部分:  
  
1. 用 `visitor pattern` 實作 `AST`   
2. 完成 `AST` 與 `parser` 和 `scanner` 間的溝通  

### 用 `visitor pattern` 實作 `AST`   

#### `AST` 的功能  

  使用 `AST` 結合 `parser` 可以妥善地分析程式碼架構，且 `AST` 在 `HW4` 的 `symbol tabel` 實作也會扮演重要的角色。  

#### `AST` 的架構  
  以 `BinaryOperator.hpp` 為例，因為每個 `class` 的架構其實都差不多，只是字串處理會有比較大的差異  
  ```cpp
  class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col,std::string new_operator_string,AstNode* new_left_expression,AstNode* new_right_expression);
                       /* TODO: operator, expressions */
    ~BinaryOperatorNode() = default;

    void print() override;
    void visitChildNodes(AstNodeVisitor &p_visitor)override;
    void accept(AstNodeVisitor &p_visitor)override;
    const char *return_op_string() const{return operator_string.c_str();}

  private:
    // TODO: operator, expressions
    std::string operator_string;
    AstNode* left_expression;
    AstNode* right_expression;
};
  ```
  ##### `AST` 部分  
  用建構函數 
  ```cpp  
  BinaryOperatorNode(const uint32_t line, const uint32_t col,std::string new_operator_string,AstNode* new_left_expression,AstNode* new_right_expression);
  ```  
  幫每個類別中的 ``` private attribute``` 賦值，``` ~~BinaryOperatorNode() = default; ``` 當作解構子，但在這次作業沒有特別重要，    
  ``` void print() override```我在這次作業中沒有特別使用，因為我使用 ```visitor pattern ``` 印出東西。   
  
  ##### `Visitor Pattern` 部分  
  同樣以 `BinaryOperator` 為例
  ```cpp
  void AstDumper::visit(BinaryOperatorNode &p_bin_op) {
    outputIndentationSpace(m_indentation);

    // TODO: operator string
    std::printf("binary operator <line: %u, col: %u> %s\n",
                p_bin_op.getLocation().line, p_bin_op.getLocation().col,
                p_bin_op.return_op_string());

    incrementIndentation();
    p_bin_op.visitChildNodes(*this);
    decrementIndentation();
}

  void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     left_expression->accept(p_visitor);
     right_expression->accept(p_visitor);
 }
 
 void BinaryOperatorNode::accept(AstNodeVisitor &p_visitor){
	   p_visitor.visit(*this);

 }
 ```
 
 `void AstDumper::visit(BinaryOperatorNode &p_bin_op)` 在 `AstDumper.cpp` ，  
 `void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor)`   
 和 `void BinaryOperatorNode::accept(AstNodeVisitor &p_visitor) `在 `BinaryOperator.cpp`  
 用 `visit` 函數印出字串，並呼叫 `visitChildNode()` 訪問 childnode ，並用 `accept()`呼叫`visit()`，藉此印出整棵 `AST`  
 
 ### `AST` 與 `parser` 和 `scanner` 間的溝通 
 要實作這項功能，主要需要完成的程序有  
 1.修改 `parser.y`    
 2.修改 `scanner.l`  
 #### 修改 `parser.y`   
 ##### 增加要 include 的 cpp 標頭檔  
 ```cpp
 #include "AST/AstDumper.hpp" 
 
 %code requires {
    
    #include "AST/ast.hpp"
    ...
    ...
    ...
    class vector;
}
 ```
 ##### 宣告 `yylval` 的 attribute  
 ```cpp
 %union {
    /* basic semantic value */
    char *identifier;
    int int_value;
    ...
    ...
    ...
    struct value_const *c_value;
};
 ```
 ##### 為 non-terminal 和 terminal 增加新的回傳型別  
 ```cpp
 %type <identifier> ProgramName ID FunctionName STRING_LITERAL TRUE FALSE
 %type <int_value> INT_LITERAL
 %type <real_value> REAL_LITERAL
 %type <bool_value> NegOrNot
 %type <list_id>  IdList
 ...
 ...
 ...
 ```
 ##### 為每個 Grammer rule 增加新的 action  (中括弧部分)
 ```cpp
 Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        root = new ProgramNode(@1.first_line,   //uint32_t line
			       @1.first_column,   //uint32_t col
                               $1, //char *p_name (字串)
			       "void", // const char *new_return_type (回傳型態)
			       $3, //vector<AstNode*> *new_declarations (新的宣告)
			       $4, //vector<AstNode*> *new_functions (新的函示)
			       $5 // AstNode *new_compound_statement (新的 compound statement)
			       );

        free($1);
    }
;
 ```
 以此類推完成整棵 `AST` 的訪問  
 ##### 增加印出 `AST` 的部分  
 ```cpp
 if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        //root->print();
        AstDumper *p_visitor = new AstDumper;
        ProgramNode *PNode = (ProgramNode*)root;
        p_visitor->visit(*PNode);
    }
 ```
 
 
 
 #### 修改 `scanner.l`  
 主要修改的部分是 `yylval` 的賦值    
 這次是要賦予 `yylval` 的 attribute
 以 `integer` 為例，
 ```lex
 {integer} { TOKEN_STRING(integer, yytext); yylval.int_value=atoi(yytext);return INT_LITERAL; }
 ```
 用 `atoi()` 將 `yytext` 字串轉為整數型，賦予 `yylval.int_value` 值    
## What is the hardest you think in this project

我認為這次作業寫扣的部分，沒有到非常困難，會讓我 debug 花那麼久的原因是我缺乏維護這種中、大型程式的經驗，  
當出現 bug 的時候，很難知道是哪一個檔案的程式碼出錯、檔案和檔案之間的關聯性又是什麼 ? 這是我在 debug 時最煩惱的部分，  
我想下次作業我會嘗試用 [source trail](https://www.sourcetrail.com/) 這個幫助我釐清檔案和程式碼關聯性的工具，幫助我 debug 吧  
[Source trail 示意圖]  
![image](https://github.com/compiler-f21/hw3-jasonsie88/blob/master/report/SourceTrail.png)


## Feedback to T.A.s

感謝助教精闢的講解影片，幫助我順利完成這次的 project。
