//
// The following include files must come first.

#ifndef SEAL_TREE_HANDCODE_H
#define SEAL_TREE_HANDCODE_H

#include <iostream>
#include "tree.h"
#include "seal.h"
#include "stringtab.h"
#define yylineno curr_lineno;
extern int yylineno;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
typedef Program_class *Program;
class Decl_class;
typedef Decl_class *Decl;
class VariableDecl_class;
typedef VariableDecl_class *VariableDecl;
class Variable_class;
typedef Variable_class *Variable;
class Type_class;
typedef Type_class *Type;
class CallDecl_class;
typedef CallDecl_class *CallDecl;
class StmtBlock_class;
typedef StmtBlock_class *StmtBlock;
class Stmt_class;
typedef Stmt_class *Stmt;
class IfStmt_class;
typedef IfStmt_class *IfStmt;
class WhileStmt_class;
typedef WhileStmt_class *WhileStmt;
class ForStmt_class;
typedef ForStmt_class *ForStmt;
class ReturnStmt_class;
typedef ReturnStmt_class *ReturnStmt;
class ContinueStmt_class;
typedef ContinueStmt_class *ContinueStmt;
class BreakStmt_class;
typedef BreakStmt_class *BreakStmt;
class Expr_class;
typedef Expr_class *Expr;
class LValue_class;
typedef LValue_class *LValue;
class Call_class;
typedef Call_class *Call;
class Actual_class;
typedef Actual_class *Actual;
class Constant_class;
typedef Constant_class *Constant;


typedef list_node<VariableDecl> VariableDecls_class;
typedef VariableDecls_class *VariableDecls;
typedef list_node<Variable> Variables_class;
typedef Variables_class *Variables;
typedef list_node<Type> Types_class;
typedef Types_class *Types;
typedef list_node<CallDecl> CallDecls_class;
typedef CallDecls_class *CallDecls;
typedef list_node<StmtBlock> StmtBlocks_class;
typedef StmtBlocks_class *StmtBlocks;
typedef list_node<Stmt> Stmts_class;
typedef Stmts_class *Stmts;
typedef list_node<IfStmt> IfStmts_class;
typedef IfStmts_class *IfStmts;
typedef list_node<WhileStmt> WhileStmts_class;
typedef WhileStmts_class *WhileStmts;
typedef list_node<ForStmt> ForStmts_class;
typedef ForStmts_class *ForStmts;
typedef list_node<ReturnStmt> ReturnStmts_class;
typedef ReturnStmts_class *ReturnStmts;
typedef list_node<ContinueStmt> ContinueStmts_class;
typedef ContinueStmts_class *ContinueStmts;
typedef list_node<BreakStmt> BreakStmts_class;
typedef BreakStmts_class *BreakStmts;
typedef list_node<Expr> Exprs_class;
typedef Exprs_class *Exprs;
typedef list_node<LValue> LValues_class;
typedef LValues_class *LValues;
typedef list_node<Call> Calls_class;
typedef Calls_class *Calls;
typedef list_node<Actual> Actuals_class;
typedef Actuals_class *Actuals;
typedef list_node<Constant> Constants_class;
typedef Constants_class *Constants;


#endif
