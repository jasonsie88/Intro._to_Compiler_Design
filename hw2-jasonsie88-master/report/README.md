# hw2 report

|||
|-:|:-|
|Name|謝承恩|
|ID|0816183|

## How much time did you spend on this project

10 hours

## Project overview

這個 project 有兩個部分需要完成:  

1. parser 與 scanner 的溝通  
2. context-free grammer 的撰寫  

### Parser 與 Scanner 的溝通: 

這部分其實不複雜，只需要去修改 scanner.l ，讓 scanner 做 lexical analyze 時會 return token 給 parser    
ex:  
```
"begin"   { TOKEN(KWbegin); return KWBEGIN;}  
```
並在 parser.y 的部分宣告使用了哪些 token  
以下是我宣告的 token  
```
%token COMMA SEMI COLON R_B L_B R_P L_P
%token PLUS_OP MINUS_OP STAR_OP SLASH_OP MOD_OP ASSIGN_OP GT_OP LT_OP LE_OP NEQ_OP GE_OP EQ_OP AND_OP OR_OP NOT_OP
%token VAR ARRAY OF BOOLEAN INTEGER REAL STRING TRUE FALSE DEF RETURN KWBEGIN END WHILE DO IF THEN ELSE FOR TO PRINT READ
%token ID
%token DEC_INT OCT_INT STRING_ID FLOAT_NUM SCIENTIFIC_NUM

```
完成以上流程， scanner 與 parser 的溝通就算是完成了。  

### Context-free grammer 的撰寫

大寫的部分為 terminal，小寫的部分為 non-terminal  
我把 context-free grammer 的語法架構分成五大部分: 

1. program unit    
2. function  
3. variable and const  
4. statement  
5. expression
 
至於處理優先級，依照 spec 提供的優先級，我用 ```%left``` ( left-associtivity ) 關鍵字幫我決定優先級，優先度由下往上，從最低排到最高

```
%left AND_OP OR_OP NOT_OP
%left LT_OP LE_OP NEQ_OP GE_OP EQ_OP GT_OP
%left MINUS_OP
%left PLUS_OP
%left MOD_OP SLASH_OP
%left STAR_OP
```

至於處理負數 ( - ) 的優先級，我用```%prec``` 關鍵字解決，這部分我在介紹 expression 時再詳述 
#### Program unit
按照 spec 的說明，nonterminal: ```Programname``` 必須符合以下格式
```
ProgramName: ID SEMI zero_or_variable_const_declaration zero_or_function_declaration compound_statement END;
```
``` ID ``` 代表 idtentifier  
``` SEMI``` 代表分號    
``` zero_or_variable_const_declaration ``` 代表有變數宣告或不宣告  
``` zero_or_function_declaration ``` 代表有函式宣告或不宣告  
``` compound_statement ``` 使用符合 compound statement 定義的文法，在 statement 部分會詳述  
至於如何實作**不宣告**的部分，我用 ``` |    ; ``` 解決此問題  
ex:
```
zero_or_variable_const_declaration: variable_const_list 
                                    | 
                                    ;
```

#### Function
在處理多個 function definition 和 function declaration 我用以下 grammer 滿足要求  
讓同樣的 non-terminal 同時出現在兩端，達到地回的效果
```
function_declaration_definition_list: function_declaration_definition_list function_declaration
                                    | function_declaration_definition_list function_definition
                                    | function_declaration
                                    | function_definition
                                    ;
```

Function 分成 function definition 和 function declaration  
function declaration 部分我用以下 grammer 滿足要求  
```
function_declaration: ID L_P zero_or_arguments_declaration R_P SEMI 
                    | ID L_P zero_or_arguments_declaration R_P COLON scalar SEMI
                    ;
                    
 scalar: INTEGER
    | REAL
    | STRING
    | BOOLEAN
    ;                   
```
```L_P``` 代表左括號   
```R_P``` 代表右括號   
```COLON``` 代表冒號   
```scalar``` 代表可能的變數型態  
```zero_or_arguments_declaration``` 代表宣告 argument 或不宣告  

function definition 部分我用以下 grammer 滿足要求  
```
function_definition: ID L_P zero_or_arguments_declaration R_P compound_statement END
                    | ID L_P zero_or_arguments_declaration R_P COLON scalar compound_statement END
                    ;
```
arguments 部分則是用以下 grammer 滿足要求
```
arguments_declaration: arguments_declaration arguments
                    | arguments
                    ;
arguments: id_list COLON scalar
        ;
id_list: ID COMMA id_list
        | ID
        ;
```
```arguments_declaration``` 代表宣告多個 arguments 的遞迴語法  
```arguments``` 為滿足 argument 的宣告語法  
```id_list``` 代表宣告多個 identifiers 的遞迴語法  
```COMMA``` 代表逗號  

#### Variable and Const
此部分我分為 variable declaration 和 constant declaration  
variable declaration 我用以下文法滿足  
```
variable_declaration: VAR id_list COLON ARRAY const_integer OF type SEMI
                    | VAR id_list COLON scalar SEMI
                    ;
                    
type: ARRAY const_integer OF type
    | scalar
    ;                   
const_integer: DEC_INT
            | OCT_INT
            ;     
variable_name: ID index
            | ID 
            ;
index: index L_B expression R_B
    | L_B expression R_B
    ;
```
```VAR``` 代表 keyword var   
```ARRAY```代表 keyword array    
```OF``` 代表 keyword of  
```L_B``` 代表```[```  
```R_B``` 代表```]```  
```type``` 代表可能宣告的 type 或是陣列  
```const_type``` 代表讀入的整數是八進位整數或十進位整數  
```viriable_name``` 讀入的 identifier  
```index``` 陣列的取值語法  

constant declaration 我用以下文法滿足   
```
const_declaration: VAR id_list COLON const_literal SEMI
;
const_literal: DEC_INT 
            | OCT_INT
            | FLOAT_NUM
            | SCIENTIFIC_NUM
            | STRING_ID
            | FALSE
            | TRUE
            ;
```
```const_literal``` 代表讀入的值，可能為八進位整數、十進位整數 、浮點數、科學記號、字串、布林值 ( 真、偽 )   

#### Statement
statement 部分主要為 ```compound_statentment``` 和 ```statement```  
```compound_statement``` 滿足 spec 定義的語法，我用了以下文法
```
compound_statement: KWBEGIN zero_or_variable_const_declaration zero_or_more_statements END;
```
```KWBEGIN``` 代表 keyword begin  
```zero_or_more_statements``` 代表宣告 statement 或不宣告  
```END``` 代表 keyword end  
```statement``` 有七個主要部分
1. Compound statement  
2. Simple statement  
3. For statement  
4. Return statement   
5. While statement  
6. Conditional statement  
7. Function Call statement  
```statement ```部分則是用以下文法滿足要求  
```
statements: compound_statement
        | basic_statement
        | for_loop_statement
        | return_statement
        | while_loop_statement
        | if_else_statement
        | function_call_statement
        ;
basic_statement: 
ASSIGN_OP expression SEMI
                | PRINT expression SEMI
                | PRINT variable_name SEMI
                | READ variable_name SEMI
                ;
for_loop_statement: FOR ID ASSIGN_OP const_integer TO const_integer DO compound_statement END DO ;
return_statement: RETURN expression SEMI;
while_loop_statement: WHILE expression DO compound_statement END DO ;
if_else_statement: IF expression THEN compound_statement END IF
                | IF expression THEN compound_statement ELSE compound_statement END IF
                ;
function_call: ID L_P expressions_by_comma_or_not R_P

```
使用 ```compound_statement``` 的理由是實作遞迴架構  
```basic_statement``` 實作了把 ```expression``` assign 給變數、輸出變數或 ```expresssion``` 、 輸入變數，屬於 simple statement 部分  
```for_loop_statement``` 實作了 for 迴圈的語法與遞迴架構，屬於 for statement 部分   
```return_statement``` 實作了 return 語法，屬於 return statement 部分    
```while_loop_statement``` 實作了 while 迴圈的語法與遞迴架構，屬於 while statement 部分   
```if_else_statement``` 實作了 if-else 語法與遞迴架構，屬於 Conditional statement 部分   
```for_loop_statement``` 實作了 for 迴圈的語法與遞迴架構，屬於 for statement 部分   
```function_call``` 實作了呼叫函式的語法，屬於 function call statement 部分  
```expressions_by_comma_or_not``` 用逗號隔開或不隔開  
```PRINT``` 代表 keyword print    
```READ``` 代表 keyword read  
```FOR``` 代表 keyword for
```WHILE``` 代表 keyword while  
```IF``` 代表 keyword if  
```THEN``` 代表 keyword then  
```ELSE``` 代表 keyword else  
```TO``` 代表 keyword to  
```DO``` 代表 keyword do  
```ASSIGN_OP``` 代表運算子 :=  

#### Expression
expression 主要處理語法的運算與優先度  
我用以下 grammer 滿足要求  
```
expression: expression OR_OP expression
            | expression AND_OP expression
            | NOT_OP expression
            | expression GE_OP expression
            | expression GT_OP expression
            | expression EQ_OP expression
            | expression LE_OP expression
            | expression LT_OP expression
            | expression NEQ_OP expression
            | expression PLUS_OP expression
            | expression MINUS_OP expression
            | expression SLASH_OP expression
            | expression MOD_OP expression
            | expression STAR_OP expression
            | MINUS_OP expression %prec STAR_OP
            | L_P expression R_P
            | const_literal
            | variable_name
            | function_call
            ;
```
用 ```%prec``` 完成負號 (unary) 的優先權  
```OR_OP``` 代表運算子 or  
```AND_OP``` 代表運算子 and  
```NOT_OP``` 代表運算子 not  
``` GE_OP``` 代表運算子 >=  
```GT_OP``` 代表運算子 >  
```EQ_OP``` 代表運算子 =  
```LE_OP``` 代表運算子 <=  
```LT_OP``` 代表運算子 <  
```NEQ_OP``` 代表運算子 <>  
```PLUS_OP``` 代表運算子 +  
```MINUS_OP``` 代表運算子 -  
```SLASH_OP``` 代表運算子 /  



## What is the hardest you think in this project

有一個實作上的細節，如果想把 keyword BEGIN 的 scanner 回傳的 token 回傳值設為 return BEGIN 會報錯，  
會報錯的原因推估是 yacc/lex 本身有變數叫做 BEGIN 導致報錯，解決方案是把 return BEGIN 改為 return KWBEGIN 便可解決此 issue.  
此外要靠自己構思一個 context-free grammer 起初真的摸不著頭緒，幸好 yacc 可以支援 ambiguous grammer，因此義語法沒有想像中困難，  
在參考 spec 和自己思考後，有成功摸索出實作方法  
起初報錯時，覺得很困惑，但在仔細觀察報錯資訊後，debug 成功  
![image](https://github.com/compiler-f21/hw2-jasonsie88/blob/master/report/BEGIN_bug.jpg)


## Feedback to T.A.s

No suggestions. 
