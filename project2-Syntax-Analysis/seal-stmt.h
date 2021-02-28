
#ifndef _H_seal_stmt
#define _H_seal_stmt

#include "tree.h"
#include "seal-tree.handcode.h"
#include "seal-decl.h"

class Program_class : public tree_node {
protected:
    Decls decls;
public:
    Program_class(Decls a1) {
       decls = a1;
    }
    Program copy_Program();
	tree_node *copy()		 { return copy_Program(); }
    void dump(ostream& stream, int n);
    void dump_with_types(ostream&, int);
};


class Stmt_class : public tree_node {
public:
	tree_node *copy()		 { return copy_Stmt(); }
	virtual Stmt copy_Stmt() = 0;
	virtual void dump_with_types(ostream&,int) = 0; 
	virtual void dump(ostream&,int) = 0;
};

class StmtBlock_class : public Stmt_class {
protected:
	 VariableDecls vars;
	 Stmts	stmts;
public:
	StmtBlock_class(VariableDecls a1, Stmts a2) {
		vars = a1;
	    stmts = a2;
	}
	Stmt copy_Stmt(){return copy_StmtBlock();}
	StmtBlock copy_StmtBlock();
	void dump(ostream& , int );
	void dump_with_types(ostream&,int);
};

class IfStmt_class : public Stmt_class {
protected:
    protected:
	Expr condition;
	StmtBlock thenexpr, elseexpr;
public:
    IfStmt_class(Expr a1, StmtBlock a2, StmtBlock a3) {
		condition = a1;
		thenexpr = a2;
		elseexpr = a3;
	}
    Stmt copy_Stmt();
	void dump(ostream& stream, int n);
	void dump_with_types(ostream&,int);
};




class WhileStmt_class : public Stmt_class {
protected:
	Expr condition;
	StmtBlock body;
public:
    WhileStmt_class(Expr a1, StmtBlock a2) {
		condition = a1;
		body = a2;
	}
    Stmt copy_Stmt();
	void dump(ostream& stream, int n);
	void dump_with_types(ostream&,int);
};

class ForStmt_class : public Stmt_class {
protected:
	Expr initexpr, condition, loopact;
	StmtBlock body;
public:
	ForStmt_class(Expr a1, Expr a2, Expr a3, StmtBlock a4) {
		initexpr = a1;
		condition = a2;
		loopact = a3;
		body = a4;
	}
    Stmt copy_Stmt();
	void dump(ostream& stream, int n);
	void dump_with_types(ostream&,int);
};



class ReturnStmt_class : public Stmt_class {
protected:
    Expr value;
public:
	ReturnStmt_class(Expr a2) {
        value = a2;
    }
    Stmt copy_Stmt();
    void dump_with_types(ostream&,int);
    void dump(ostream& stream, int n);
};

class ContinueStmt_class : public Stmt_class {
public:
	ContinueStmt_class() {}
    Stmt copy_Stmt();
    void dump_with_types(ostream&,int);
    void dump(ostream& stream, int n);
};


class BreakStmt_class : public Stmt_class {
public:
	BreakStmt_class() {}
    Stmt copy_Stmt();
    void dump_with_types(ostream&,int);
    void dump(ostream& stream, int n);
};

typedef class Program_class *Program;
typedef class Stmt_class *Stmt;
typedef class StmtBlock_class *StmtBlock;
typedef class IfStmt_class *IfStmt;
typedef class ForStmt_class *ForStmt;
typedef class WhileStmt_class *WhileStmt;
typedef class ReturnStmt_class *ReturnStmt;
typedef class ContinueStmt_class *ContinueStmt;
typedef class BreakStmt_class *BreakStmt;

typedef list_node<Stmt> Stmts_class;
typedef Stmts_class *Stmts;


StmtBlocks nil_StmtBlocks();
StmtBlocks single_StmtBlocks(StmtBlock);
StmtBlocks append_StmtBlocks(StmtBlocks,StmtBlocks);


Stmts nil_Stmts();
Stmts single_Stmts(Stmt);
Stmts append_Stmts(Stmts,Stmts);


IfStmts nil_IfStmts();
IfStmts single_IfStmts(IfStmt);
IfStmts append_IfStmts(IfStmts,IfStmts);


WhileStmts nil_WhileStmts();
WhileStmts single_WhileStmts(WhileStmt);
WhileStmts append_WhileStmts(WhileStmts,WhileStmts);


ForStmts nil_ForStmts();
ForStmts single_ForStmts(ForStmt);
ForStmts append_ForStmts(ForStmts,ForStmts);


ReturnStmts nil_ReturnStmts();
ReturnStmts single_ReturnStmts(ReturnStmt);
ReturnStmts append_ReturnStmts(ReturnStmts,ReturnStmts);


ContinueStmts nil_ContinueStmts();
ContinueStmts single_ContinueStmts(ContinueStmt);
ContinueStmts append_ContinueStmts(ContinueStmts,ContinueStmts);


BreakStmts nil_BreakStmts();
BreakStmts single_BreakStmts(BreakStmt);
BreakStmts append_BreakStmts(BreakStmts,BreakStmts);

Program program(Decls);
StmtBlock stmtBlock(VariableDecls, Stmts);
IfStmt ifstmt(Expr, StmtBlock, StmtBlock);
WhileStmt whilestmt(Expr, StmtBlock);
ForStmt forstmt(Expr, Expr, Expr, StmtBlock);
ReturnStmt returnstmt(Expr);
ContinueStmt continuestmt();
BreakStmt breakstmt();

#endif
