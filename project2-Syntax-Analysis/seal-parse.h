#ifndef _SEAL_PARSE_H
#define _SEAL_PARSE_H
//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"

#ifndef _SEAL_H_
#define _SEAL_H_

#include "seal-io.h"

/* a type renaming */
typedef int Boolean;
class Entry;
typedef Entry *Symbol;

Boolean copy_Boolean(Boolean);
void assert_Boolean(Boolean);
void dump_Boolean(ostream &,int,Boolean);

Symbol copy_Symbol(Symbol);
void assert_Symbol(Symbol);
void dump_Symbol(ostream &,int,Symbol);

#endif
#include "tree.h"
typedef class Decl_class *Decl;
typedef class Variable_class *Variable;
typedef class VariableDecl_class *VariableDecl;
typedef class CallDecl_class *CallDecl;
typedef class Expr_class *Expr;
typedef class Actual_class *Actual;
typedef class Object_class *Object;
typedef class Call_class *Call;
typedef class Program_class *Program;
typedef class Stmt_class *Stmt;
typedef class StmtBlock_class *StmtBlock;
typedef class IfStmt_class *IfStmt;
typedef class ForStmt_class *ForStmt;
typedef class ReturnStmt_class *ReturnStmt;
typedef class ContinueStmt_class *ContinueStmt;
typedef class BreakStmt_class *BreakStmt;
typedef class WhileStmt_class *WhileStmt;

typedef list_node<Decl> Decls_class;
typedef Decls_class *Decls;
typedef list_node<VariableDecl> VariableDecls_class;
typedef VariableDecls_class *VariableDecls;
typedef list_node<Variable> Variables_class;
typedef Variables_class *Variables;
typedef list_node<Expr> Exprs_class;
typedef Exprs_class *Exprs;
typedef list_node<Actual> Actuals_class;
typedef Actuals_class *Actuals;
typedef list_node<Stmt> Stmts_class;
typedef Stmts_class *Stmts;
/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
      // condition
     IF = 258,
     ELSE = 260,
      // loop
     WHILE = 261,
     FOR = 262,
     BREAK = 263,
     CONTINUE = 264,
      // func
     FUNC = 265,
     RETURN = 266,
      // const
     CONST_BOOL = 267,
     CONST_INT = 268,
     CONST_STRING = 269,
     CONST_FLOAT = 270,
      // utils
     VAR = 271,
     STRUCT = 272,
     ERROR = 273,
      // multi-charactor operators
     AND = 274,
     OR = 275,
     EQUAL = 276,
     NE = 277, // !=
     GE = 278, // >=
     LE = 279, // <=
     // basic type
     OBJECTID = 284,
     TYPEID = 285
   };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 260
#define WHILE 261
#define FOR 262
#define BREAK 263
#define CONTINUE 264
#define FUNC 265
#define RETURN 266
#define CONST_BOOL 267
#define CONST_INT 268
#define CONST_STRING 269
#define CONST_FLOAT 270
#define VAR 271
#define STRUCT 272
#define ERROR 273
#define AND 274
#define OR 275
#define EQUAL 276
#define NE 277
#define GE 278
#define LE 279
#define OBJECTID 284
#define TYPEID 285

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 46 "seal.y"
{
      Boolean boolean;
      Symbol symbol;
      Program program;
      Decl decl;
      Decls decls;
      VariableDecl variableDecl;
      VariableDecls variableDecls;
      Variable variable;
      Variables variables;
      CallDecl callDecl;
      StmtBlock stmtBlock;
      Stmt stmt;
      Stmts stmts;
      IfStmt ifStmt;
      WhileStmt whileStmt;
      ForStmt forStmt;
      ReturnStmt returnStmt;
      ContinueStmt continueStmt;
      BreakStmt breakStmt;
      Expr expr;
      Exprs exprs;
      Call call;
      Actual actual;
      Actuals actuals;
      
      char *error_msg;
}
/* Line 1489 of yacc.c.  */
#line 124 "seal.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE seal_yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE seal_yylloc;
#endif
