# hw4 report

|||
|-:|:-|
|Name|謝承恩|
|ID|0816183|

## How much time did you spend on this project

> 大概 40 個小時...

## Project overview

這次我做出的改動有三個  
1.把 ``AST`` 部分 ``private`` 改成 ``public`` 方便我取值  
2.用 ``symbol_table.hpp`` 和 ``symbol_table.cpp``  
3.在 ``SemanticAnalyzer.cpp`` 做 ``type checking``  
若有錯誤則用 ``print_error()`` 和 ``print_error_code()`` 印出錯誤訊息  
``symbol_table.cpp`` 主要宣告五種 ``class``  
``SymbolEntry``、``SymbolTable``、``SymbolManager``  
``FunctionEntry``、``FunctionManager``  
### SymbolManager  
維護管理多個 ``SymbolTable``的 class   
### SymbolTable  
每個 symbol table 基本架構的 class，有 ``SymbolEntry`` vector  
### SymbolEntry  
有 symbol entry 基本架構的 class  
### FunctionManager  
維護 function table 的 class  
### FunctionEntry  
維護 function entry 基本架構的 class  

## What is the hardest you think in this project

這次的作業我花最多時間 de 的 bug 是 segment fault ，會出現此 bug ，  
通常是 pointer 沒有維護好，存取到非法記憶體，才會導致此錯誤，  
當初為了釐清這個錯誤，花了我很多時間，反覆檢查程式碼，  
原本想用 ``GDB`` 這個 debugger 解決問題，但沒有時間熟悉此工具 @@  
不然一定可以更有效率的解決問題   

## Feedback to T.A.s

這次的作業難度偏高，又跟很多期末考和期末 project 撞期，整體時間非常緊迫，  

有種蠟燭兩頭燒的感覺，是一次很特(ㄊㄨㄥˋ)殊(ㄎㄨˇ)的體驗。    
