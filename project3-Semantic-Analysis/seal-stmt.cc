#include "tree.h"
#include "seal-decl.h"
#include "seal-stmt.h"
#include "seal-expr.h"

Program Program_class::copy_Program()
{
   return new Program_class(decls->copy_list());
}

void Program_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_program\n";
   decls->dump(stream, n+2);
}

StmtBlock StmtBlock_class::copy_StmtBlock()
{
   return new StmtBlock_class(vars->copy_list(), stmts->copy_list());
}

void StmtBlock_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_stmtBlock\n";
   vars->dump(stream, n+2);
   stmts->dump(stream, n+2);
}


Stmt IfStmt_class::copy_Stmt()
{
   return new IfStmt_class(condition->copy_Expr(), thenexpr->copy_StmtBlock(), elseexpr->copy_StmtBlock());
}


void IfStmt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_ifStmt\n";
   condition->dump(stream, n+2);
   thenexpr->dump(stream, n+2);
   elseexpr->dump(stream, n+2);
}


Stmt WhileStmt_class::copy_Stmt()
{
   return new WhileStmt_class(condition->copy_Expr(), body->copy_StmtBlock());
}


void WhileStmt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_whileStmt\n";
   condition->dump(stream, n+2);
   body->dump(stream, n+2);
}


Stmt ForStmt_class::copy_Stmt()
{
   return new ForStmt_class(initexpr->copy_Expr(), condition->copy_Expr(), loopact->copy_Expr(), body->copy_StmtBlock());
}


void ForStmt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_forStmt\n";
   initexpr->dump(stream, n+2);
   condition->dump(stream, n+2);
   loopact->dump(stream, n+2);
   body->dump(stream, n+2);
}


Stmt BreakStmt_class::copy_Stmt()
{
   return new BreakStmt_class();
}


void BreakStmt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_breakStmt\n";
}


Stmt ContinueStmt_class::copy_Stmt()
{
   return new ContinueStmt_class();
}


void ContinueStmt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_continueStmt\n";
}

Stmt ReturnStmt_class::copy_Stmt()
{
   return new ReturnStmt_class(value->copy_Expr());
}


void ReturnStmt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_returnStmt\n";
   value->dump(stream, n+2);
}


Stmts nil_Stmts()
{
   return new nil_node<Stmt>();
}
Stmts single_Stmts(Stmt e)
{
   return new single_list_node<Stmt>(e);
}
Stmts append_Stmts(Stmts p1, Stmts p2)
{
   return new append_node<Stmt>(p1, p2);
}


Program program(Decls a1)
{
   return new Program_class(a1);
};

StmtBlock stmtBlock(VariableDecls a1, Stmts a2)
{
  return new StmtBlock_class(a1, a2);
}

IfStmt ifstmt(Expr a1, StmtBlock a2, StmtBlock a3)
{
  return new IfStmt_class(a1, a2, a3);
}

WhileStmt whilestmt(Expr a1, StmtBlock a2)
{
  return new WhileStmt_class(a1, a2);
}

ForStmt forstmt(Expr a1, Expr a2, Expr a3, StmtBlock a4)
{
	return new ForStmt_class(a1,a2,a3,a4);
}

ReturnStmt returnstmt(Expr a1)
{
  return new ReturnStmt_class(a1);
}

ContinueStmt continuestmt()
{
  return new ContinueStmt_class();
}

BreakStmt breakstmt()
{
  return new BreakStmt_class();
}