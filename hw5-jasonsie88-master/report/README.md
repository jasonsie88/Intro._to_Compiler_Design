# hw5 report

|||
|-:|:-|
|Name|謝承恩|
|ID|0816183|

## How much time did you spend on this project

50 小時 

## Project overview

這次的 project 需要完成 ``CodeGenerator.hpp`` 和 ``CodeGenerator.cpp``  
並對先前時做過的 ``AST`` 和 ``Symbol Table`` 做出適當的改動，以此完成這個簡易編譯器  

### 先前改動的部分
#### AST

我主要做的改動是把一些 private 的 attribute 改成 public 方便取值  
以及在 function node 新增計算 argument 數量的函式  

#### Symbol Tabl

我讓 ``SymbolEntry`` 這個類別可以儲存 offset 方便我在呼叫函式或變數的時候，取得 offset  

### CodeGenerator

這部分我新增了判斷是否 read 和是否 local 的函式與變數  
方便我撰寫組語產生器，這部分主要還是根據各種不同的 AST node 和 symbol table 的情況  
撰寫對應的 risc-V code，並用 ``dumpInstructions()`` 把組語輸出  

## What is the hardest you think in this project

這次我覺得寫 C++ 的 code 部分並不難，因為助教都已經寫的 code 就已經有很多提示了  
最難的部份應該是學習怎麼撰寫組語 risc-v 的部分，要拿到基礎分，只要能吸收助教提供的教材  
應該不會太困難，但若是想拿到 advance 或 bonus 的部分，那可能就需要自己爬文吸收網路上的資料  
這邊推薦一個中文教材 [RISC-V 手冊](http://riscvbook.com/chinese/RISC-V-Reader-Chinese-v2p1.pdf) 中文的教材吸收速度應該比英文有效率很多     

## Feedback to T.A.s

這門課從學期初的 ``lexical analysis``、``syntax analysis``，到比較進階的 ``semantic analysis`` 和 ``type checking``  
一直到最後的作業 ``code generation``，我總算實作出了一個很陽春的編譯器，這門課不僅幫助我複習了物件導向觀念，  
還讓我學習到了一些開發大型程式的觀念，在這一連串作業的洗禮下，  
我也了解到，若想獨立撰寫出一套完整且成熟好用的編譯器，  
是非常勞心勞力的，畢竟我要搞出這個簡易編譯器，就花了我好多時間 QQ    
如果要搞出像 ``gcc``、``g++``諸如此類的成熟編譯器，不知道要花多少歲月，  
在此我也非常佩服那些在開發編譯器前線，探索相關問題的研究員與工程師，  
他們付出的青春與睡眠時間，讓我們這些資訊領域的後進，能有舒適的程式開發環境 o7    






