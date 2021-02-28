//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"

#include "seal.h"
#include "tree.h"
#include "seal-decl.h"
#include "seal-stmt.h"
#include "seal-expr.h"
#include "utilities.h"

// defined in stringtab.cc
void dump_Symbol(ostream& stream, int padding, Symbol b); 

// defined in seal.h
void dump_Boolean(ostream& stream, int padding, Boolean b);

//////////////////////////////////////////////////////////////////
//
//  dumptype.cc
//
//  dumptype defines a simple recursive traversal of the abstract
//  syntax tree (AST) that prints each node and any associated
//  type information.  Use dump_with_types to inspect the results of
//  type inference.
//
//  dump_with_types takes two argumenmts:
//     an output stream
//     an indentation "n", the number of blanks to insert at the beginning of
//                         a new line.
//   
//  dump_with_types is just a simple pretty printer, formatting the output
//  to show the AST relationships between nodes and their types.
//  dump_type is a virtual function, with a separate implementation for
//  each kind of AST node.  Using virtual functions is an easy way to
//  implement recursive tree traversals in C++; each kind of tree node
//  has a virtual function that "knows" how to perform the part of
//  the traversal for that one node.  It may help to know the inheritance hierarchy
//  of the declarations that define the structure of the Seal AST. 
//   
//  Program_class
//     Program_class
// 
//  Decl_class
//     VariableDecl_class
//     CallDecl_class
//   
//  Variable_class
//     variable_class
//
//  Stmt_class
//     StmtBlock_class
//     IfStmt_class
//     WhileStmt_class
//     ForStmt_class
//     BreakStmt_class
//     ContinueStmt_class
//     ReturnStmt_class
//
//  Expr_class
//     Assign_class
//     Add_class
//     Minus_class
//     Multi_class
//     Divide_class
//     Mod_class
//     Neg_class
//     Lt_class
//     Le_class
//     Equ_class
//     Neq_class
//     Ge_class
//     Gt_class
//     And_class
//     Or_class
//     Not_class
//     Bnot_class
//     Band_class
//     Bor_class
//     Bxor_class
//     No_expr_class
//     Object_class
//     Call_class
//     Constant_int_class
//     Constant_float_class
//     Constant_bool_class
//     Constant_string_class
//     Actual_class
//  
//
// 
//  All of the Phyla inherit from the tree_node class, which has a member
//  called type.  The type member holds the type of the AST node.
//
//  Some AST nodes have lists of other tree nodes as components.  Lists in the
//  AST are built using the class list_node defined in tree.h.  The list
//  classes in the Seal AST are:
//
//  Decls            a list of Decl
//  VariableDecls    a list of VariableDecl (Variable Declaration)
//  Variables        a list of Variable
//  Stmts            a list of Stmt
//  Actuals          a list of Actual
//  Exprs            a list of Exprs
//


//
//  dump_type prints the type of an Expression on the output stream,
//  after indenting the correct number of spaces.  A check is made to
//  see if no type is assigned to the node.
//
//

void Expr_class::dump_type(ostream& stream, int n)
{
  if (type)
    { stream << pad(n) << ": " << type << endl; }
  else
    { stream << pad(n) << ": _no_type" << endl; }
}


void Call_class::dump_type(ostream& stream, int n)
{
  if (type)
    { stream << pad(n) << ": " << type << endl; }
  else
    { stream << pad(n) << ": _no_type" << endl; }
}

void Actual_class::dump_type(ostream& stream, int n)
{
  if (type)
    { stream << pad(n) << ": " << type << endl; }
  else
    { stream << pad(n) << ": _no_type" << endl; }
}

void dump_line(ostream& stream, int n, tree_node *t)
{
  stream << pad(n) << "#" << t->get_line_number() << "\n";
}

//
//  program_class prints "program" and then each of the
//  component classes of the program, one at a time, at a
//  greater indentation. The recursive invocation on
//  "classes->nth(i)->dump_with_types(...)" shows how useful
//  and compact virtual functions are for this kind of computation.
//
//  Note the use of the iterator to cycle through all of the
//  classes.  The methods first, more, next, and nth on AST lists
//  are defined in tree.h.
//
void Program_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Program\n";
   for(int i = decls->first(); decls->more(i); i = decls->next(i)){
      decls->nth(i)->dump_with_types(stream, n+2);
   }
     
}

void VariableDecl_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Variable Declaration\n";
   variable->dump_with_types(stream, n+2);
}

void Variable_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Variable\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, name);
   stream << pad(n+2) << "(type)\n";
   dump_Symbol(stream, n+2, type);
}


void CallDecl_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Call Declaration\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, name);
   stream << pad(n+2) << "(parameters)\n";
   stream << pad(n+2) << "(\n";
   for(int i = paras->first(); paras->more(i); i = paras->next(i))
     paras->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
   stream << pad(n+2) << "(return type)\n";
   dump_Symbol(stream, n+2, returnType);
   stream << pad(n+2) << "(body)\n";
   body->dump_with_types(stream, n+2);
   
}

void StmtBlock_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Statement Block\n";
   stream << pad(n+2) << "(variable declarations)\n";
   stream << pad(n+2) << "(\n";
   for(int i = vars->first(); vars->more(i); i = vars->next(i))
     vars->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
   stream << pad(n+2) << "(statements)\n";
   stream << pad(n+2) << "(\n";
   for(int i = stmts->first(); stmts->more(i); i = stmts->next(i))
     stmts->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
}

void IfStmt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "IfStmt\n";
   stream << pad(n+2) << "(condition)\n";
   condition->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(then)\n";
   thenexpr->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(else)\n";
   elseexpr->dump_with_types(stream, n+2); 
}

void WhileStmt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "WhileStmt\n";
   stream << pad(n+2) << "(condition)\n";
   condition->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(body)\n";
   body->dump_with_types(stream, n+2);
}

void ForStmt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "ForStmt\n";
   stream << pad(n+2) << "(init)\n";
   initexpr->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(condition)\n";
   condition->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(loop)\n";
   loopact->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(body)\n";
   body->dump_with_types(stream, n+2);
}

void BreakStmt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "BreakStmt\n";
}


void ContinueStmt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "ContinueStmt\n";
}

void ReturnStmt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "ReturnStmt\n";
   stream << pad(n+2) << "(return value)\n";
   value->dump_with_types(stream, n+2);
}

void Assign_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Assign\n";
   stream << pad(n+2) << "(left value)\n";
   dump_Symbol(stream, n+2, lvalue);
   stream << pad(n+2) << "(right value)\n";
   value->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Add_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "+\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Minus_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "-\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Multi_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "*\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}
void Divide_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "/\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}
void Mod_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "%\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}
void Neg_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "-\n";
   stream << pad(n+2) << "(OP)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}
void Lt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "<\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}
void Le_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "<=\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Equ_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "==\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Neq_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "!=\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Ge_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << ">=\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Gt_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << ">\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void And_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "&&\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Or_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "||\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Xor_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "^\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Not_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "!\n";
   stream << pad(n+2) << "(OP)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Bitand_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "&\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}
void Bitor_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "|\n";
   stream << pad(n+2) << "(OP left)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(OP right)\n";
   e2->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}

void Bitnot_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "~\n";
   stream << pad(n+2) << "(OP)\n";
   e1->dump_with_types(stream, n+2);
   stream << pad(n+2) << "(type)\n";
}

void Object_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Object\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, var);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}


void Call_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Call\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, name);
   stream << pad(n+2) << "(actual parameters)\n";
   stream << pad(n+2) << "(\n";
   for(int i = actuals->first(); actuals->more(i); i = actuals->next(i))
     actuals->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Actual_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Actual\n";
   stream << pad(n+2) << "(expr)\n";
   expr->dump_with_types(stream,n+2);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Const_int_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Const_int\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, value);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Const_string_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Const_string\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, value);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Const_float_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Const_float\n";
   stream << pad(n+2) << "(name)\n";
   dump_Symbol(stream, n+2, value);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void Const_bool_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "Const_bool\n";
   stream << pad(n+2) << "(name)\n";
   dump_Boolean(stream, n+2, value);
   stream << pad(n+2) << "(type)\n";
   dump_type(stream,n);
}

void No_expr_class::dump_with_types(ostream& stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "No_expr\n";
}