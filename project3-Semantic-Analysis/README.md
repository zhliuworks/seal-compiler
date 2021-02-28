README file for Programming Assignment 3 (C++ edition)
======================================================

目录下的文件：

```
handle_flags.cc             请勿修改，用语定义运行参数
semant-phase.cc             主入口，main所在地
semant.h                    语义分析器头文件
semant.cc                   语义分析器实现
seal-expr.cc                expr的AST节点声明定义
seal-lex.cc                 词法分析文件
seal-stmt.cc                stmt的AST节点声明定义
seal-tree.handcode.h        AST相关头文件
stringtab.h                 字符串表头文件
tree.h                      树头文件
cgen_gc.h                   cgen选项
judge.sh                    判断脚本
README.md                   说明文件
seal-expr.h                 expr的AST节点声明头文件
seal.output                 bison产生的状态机信息文件
seal-stmt.h                 stmt的AST节点声明
seal.y                      语法分析规则文件
utilities.cc                杂项函数
copyright.h                 版权
list.h                      链表
seal-decl.cc                decl的AST节点声明定义
seal.h                      seal相关头文件
seal-parse.cc               bison生成的文件
seal.tab.h                  bison生成的文件
stringtab.cc                字符串表实现
utilities.h                 杂项函数头文件
dumptype.cc                 AST输出实现
Makefile                    make规则文件
seal-decl.h                 decl的AST节点声明头文件
seal-io.h                   seal相关文件
seal-parse.h                bison生成的文件
seal-tree.aps               seal的AST树节点介绍文件
stringtab_functions.h       字符串表函数头文件
tree.cc                     树实现
*.*			                其他文件
```

```
你可以任意修改已知的代码，然后便于实现语义分析

semant.cc是你需要添加内容的文件.  main()函数调用 `ast_root'的`semant'方法
, `ast_root'是语法分析产生的AST树的树根（不必理会这个树是如何生成的，代码已经是给出的）. 
semant.cc中提供了错误输出的函数，请不要修改这些函数，直接拿来调用就行了
```

Instructions
------------

```
构建语义分析器:

% make semant

测试某个样例

% ./semant < test.seal

清理临时文件

% make clean


GOOD LUCK!
```

---8<------8<------8<------8<---cut here---8<------8<------8<------8<---

Write-up for PA4
----------------

所有的检查项目:

### 函数

* [x] 是否有`main`函数
* [x] `main`函数返回值为Void
* [x] `main`函数没有参数
* [x] 没有重复的函数声明
* [x] `printf`不可重定义
* [x] `printf`必须有至少一个参数，且第一个参数类型必须为`String`
* [x] 返回值仅限于`Int` `Void` `String` `Float` 和`Bool`
* [x] `return`语句返回的类型和声明的一致
* [x] 每一个函数声明在最外层逻辑结束前（即函数体的最外层右大括号闭合前）一定要有一个`return`语句
* [x] 形参没有重复命名的
* [x] 形参类型不可以为`Void`
* [x] 全局变量没有重复命名的
* [x] 变量类型不可以为`Void`
* [x] 局部变量一定先声明再使用
* [x] install_calls函数将预先配置所有函数的声明（这样写在前面的函数也可以调用后面才声明的函数）
* [x] install_globalVars函数将预先配置所有全局变量的声明（这样全局变量在文件哪里定义就无所谓了）
* [x] 赋值语句右值和左值类型相符
* [x] 函数调用时的实参一定要和声明的形参一致，且数目相同
* [x] `if`、`while`条件部分的类型一定是`Bool`，`for`的条件部分可以置空语句
* [x] 运算符的操作数必须满足类型约束，即四则运算和比较运算可以在`Int`和`Float`之间混合发生，布尔运算只能在`Bool`之间发生，位运算只能在`Int`之间发生
* [x] `continue`和`break`必须位于某一个循环体内


## 测试

`judge.sh`是测试脚本.

运行 `./judge.sh` 或者 `bash judge.sh`, 应该得到类似下列输出:

```
--------Test using test10.seal --------
Passed
--------Test using test1.seal --------
Passed
--------Test using test2.seal --------
20: object c has not been defined.
Compilation halted due to static semantic errors.
Passed
--------Test using test3.seal --------
12: Returns Bool , but need Void
Compilation halted due to static semantic errors.
Passed
--------Test using test4.seal --------
Passed
--------Test using test5.seal --------
Passed
--------Test using test6.seal --------
6: main function should have return type Void.
9: break must be used in a loop sentence.
Compilation halted due to static semantic errors.
Passed
--------Test using test7.seal --------
Passed
--------Test using test8.seal --------
13: Function loop, type Float of parameter a does not conform to declared type Bool.
7: Function main must have an overall return statement.
Compilation halted due to static semantic errors.
Passed
--------Test using test9.seal --------
Passed
```
