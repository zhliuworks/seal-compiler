# seal-compiler
Course Design of Compiler Principles ( IS203 )  :  complete the compiler design process for an experimental language called **SEAL** (Simple Education Accompanying Language). The manual of this language is also stored in the repository.

The whole project consists of four parts:

* **Lexical Analysis** : flex-based symbol extraction 
* **Syntax Analysis** :  bison-based AST construction
* **Semantic Analysis** :  static semantics check with AST
* **Code Generation** :  x86 code generation with AST

Test Environment :

* Linux Kernel : 5.4.0
* Operating System : Ubuntu 20.04
* GCC : 9.3.0