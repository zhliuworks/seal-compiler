README file for Programming Assignment 4  (C++ edition)
=======================================================

目录下的文件

handle_flags.cc             请勿修改，用语定义运行参数
cgen-phase.cc             	主入口，main所在地
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
cgen.cc						代码生成器文件
cgen.h						代码生成器头文件
*.*			                其他文件
semant.o					部分AST类声明的实现

	cgen.cc是此次要编写的主要文件，主要包括了以下3个内容:

		- 生成全局变量和常量代码函数;
		 
		- 递归的生成函数声明的函数；

	cgen.h 是头文件，请在里面添加任何用于辅助的类或者函数等.

	cgen_supp.cc也是辅助函数文件，你可以添加或者修改，但是不要动下面三个函数:

		o byte_mode
		o ascii_mode
		o emit_string_constant

	emit.h 定义了一些X86-64下的常量，便于编写emit_***函数.

        cgen-phase.cc 是主入口

Instructions
------------

	生成代码生成器（实际上是完整的编译器）:

	% make cgen

	编译某个文件:

	% ./cgen < test.seal > test.asm

	清理临时文件

	% make clean

	GOOD LUCK!

---8<------8<------8<------8<---cut here---8<------8<------8<------8<---

Result for PA5
----------------
```
[root@VM_0_5_centos code-generate]# ./judge.sh 
--------Test using Euclidean.seal --------
passed
--------Test using Fibonacci.seal --------
passed
--------Test using generator.seal --------
passed
--------Test using helloworld.seal --------
passed
--------Test using tan.seal --------
passed
```