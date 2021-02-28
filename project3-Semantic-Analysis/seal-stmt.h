
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

	void semant();
	// for semantic analysis
};


class Stmt_class : public tree_node {
public:
	tree_node *copy()		 { return copy_Stmt(); }
	virtual Stmt copy_Stmt() = 0;
	virtual void dump_with_types(ostream&,int) = 0; 
	virtual void dump(ostream&,int) = 0;
	virtual void check(Symbol) = 0;
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
	Stmts getStmts(){return stmts;}

	VariableDecls getVariableDecls(){return vars;};
	StmtBlock copy_StmtBlock();
	void check(Symbol);
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
	Expr getCondition(){return condition;}
	StmtBlock getThen(){return thenexpr;}
	StmtBlock getElse(){return elseexpr;}
    Stmt copy_Stmt();
	void check(Symbol);
	void dump(ostream& stream, int n);
	void dump_with_types(ostream&,int);
};


typedef class WhileStmt_class *WhileStmt;

class WhileStmt_class : public Stmt_class {
protected:
	Expr condition;
	StmtBlock body;
public:
    WhileStmt_class(Expr a1, StmtBlock a2) {
		condition = a1;
		body = a2;
	}
	Expr getCondition(){return condition;}
	StmtBlock getBody(){return body;}
    Stmt copy_Stmt();
	void check(Symbol);
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
	Expr getInit(){return initexpr;}
	Expr getCondition(){return condition;}
	Expr getLoop(){return loopact;}
	StmtBlock getBody(){return body;}
	void check(Symbol);
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
	Expr getValue(){return value;}
    Stmt copy_Stmt();
	void check(Symbol);
    void dump_with_types(ostream&,int);
    void dump(ostream& stream, int n);
};

class ContinueStmt_class : public Stmt_class {
public:
	ContinueStmt_class() {}
    Stmt copy_Stmt();
	void check(Symbol);
    void dump_with_types(ostream&,int);
    void dump(ostream& stream, int n);
};


class BreakStmt_class : public Stmt_class {
public:
	BreakStmt_class() {}
    Stmt copy_Stmt();
	void check(Symbol);
    void dump_with_types(ostream&,int);
    void dump(ostream& stream, int n);
};

typedef class Program_class *Program;
typedef class Stmt_class *Stmt;
typedef class StmtBlock_class *StmtBlock;
typedef class IfStmt_class *IfStmt;
typedef class ForStmt_class *ForStmt;
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
