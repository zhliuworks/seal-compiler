#include "tree.h"
#include "seal-decl.h"
#include "seal-expr.h"
#include "seal-stmt.h"



Decl VariableDecl_class::copy_Decl()
{
   return new VariableDecl_class(variable->copy_Variable());
}


void VariableDecl_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_variableDecl\n";
   variable->dump(stream, n+2);
}


Variable Variable_class::copy_Variable()
{
   return new Variable_class(copy_Symbol(name), copy_Symbol(type));
}


void Variable_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_variable\n";
   dump_Symbol(stream, n+2, name);
   dump_Symbol(stream, n+2, type);
}


Decl CallDecl_class::copy_Decl()
{
   return new CallDecl_class(copy_Symbol(name), paras->copy_list(), copy_Symbol(returnType), body->copy_StmtBlock());
}


void CallDecl_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_callDecl\n";
   dump_Symbol(stream, n+2, name);
   paras->dump(stream, n+2);
   body->dump(stream, n+2);
   dump_Symbol(stream, n+2, returnType);
}


Decls nil_Decls()
{
   return new nil_node<Decl>();
}
Decls single_Decls(Decl e)
{
   return new single_list_node<Decl>(e);
}
Decls append_Decls(Decls p1, Decls p2)
{
   return new append_node<Decl>(p1, p2);
}


VariableDecls nil_VariableDecls()
{
   return new nil_node<VariableDecl>();
}
VariableDecls single_VariableDecls(VariableDecl e)
{
   return new single_list_node<VariableDecl>(e);
}
VariableDecls append_VariableDecls(VariableDecls p1, VariableDecls p2)
{
   return new append_node<VariableDecl>(p1, p2);
}


Variables nil_Variables()
{
   return new nil_node<Variable>();
}
Variables single_Variables(Variable e)
{
   return new single_list_node<Variable>(e);
}
Variables append_Variables(Variables p1, Variables p2)
{
   return new append_node<Variable>(p1, p2);
}


VariableDecl variableDecl(Variable v)
{
  return new VariableDecl_class(v);
}


Variable variable(Symbol a1, Symbol a2)
{
  return new Variable_class(a1, a2);
}

CallDecl callDecl(Symbol a1, Variables a2, Symbol a3, StmtBlock a4)
{
  return new CallDecl_class(a1, a2, a3, a4);
}