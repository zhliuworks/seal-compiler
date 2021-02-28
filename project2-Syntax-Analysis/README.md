README file for Programming Assignment 2 (C++ edition)
======================================================

目录下的文件

```
handle_flags.cc             请勿修改，用语定义运行参数
parser-phase.cc             主入口，main所在地
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
Makefile包括了语法分析器的编译选项，请勿修改

seal.y 是bison的规则文件，请阅读相关参考资料结合Seal的语法规则，填写相关空白

很多文件是 `bison' 产生的.
`seal-parse.cc' 即为bison产生的语法分析器文件.

```

指令
------------

```
生成语法分析器请利用命令:

% make parser

会产生一个名为parser的语法分析器

测试某个文件，利用

% ./parser < test.seal

当需要清除生成的临时文件，请利用
% make clean
请在每次生成分析器之前清除临时文件，因为有时候代码的修改不能及时反映在临时文件中


GOOD LUCK!
```

---8<------8<------8<------8<---cut here---8<------8<------8<------8<---

Write-up for PA3
----------------


### Write Parsing Rules

根据 `seal-tree.aps`, 应该可以很容易写 `seal.y`. 例如 `variable` 在 `seal-tree.aps` 如下:

```cpp
 constructor variable(obj: Symbol; tp : Symbol) : Variable;
```

则在 `seal.y` 的规则为:

```cpp
	variable	:	OBJECTID TYPEID {
					$$ = variable($1, $2);
				}
				;
```

对于有LIST类型的非终结符，函数`single_***()` 能够产生一个长度为1的list类型，`append_***()`把两个LIST合在一起，例如类型为`Decls`名为`decl_list`，定义在`seal.y`中的非终结符变量：


```cpp
    decl_list	: decl	{ 
					$$ = single_Decls($1);
				}
				| decl_list decl { 
					$$ = append_Decls($1, single_Decls($2)); 
				}
				;
```

当完成了`seal.y`, 利用`make parser` 构建分析器. Yacc会生成一个人类可读的LALR(1) 分析表和状态机，在`seal.output`文件中。 你可以查看相关的移进-规约冲突。

## Judge

`judge.sh` 是测试脚本。运行`./judge.sh` 或者`bash judge.sh`

如果正确的话，应该能得到如下输出

```
[root@VM_0_5_centos syntax]# ./judge.sh 
--------Test using test10.seal --------
passed
--------Test using test1.seal --------
passed
--------Test using test2.seal --------
passed
--------Test using test3.seal --------
passed
--------Test using test4.seal --------
passed
--------Test using test5.seal --------
passed
--------Test using test6.seal --------
passed
--------Test using test7.seal --------
passed
--------Test using test8.seal --------
passed
--------Test using test9.seal --------
passed
```
