
#ifndef _H_seal_decl
#define _H_seal_decl

#include "tree.h"
#include "seal-tree.handcode.h"



class Decl_class : public tree_node {
public:
    tree_node *copy()		 { return copy_Decl(); }
    virtual Decl copy_Decl() = 0;
    virtual void dump_with_types(ostream&,int) = 0; 
    virtual void dump(ostream&,int) = 0;
};


class Variable_class : public tree_node {
protected:
   Symbol name;
   Symbol type;
public:
   Variable_class(Symbol a1, Symbol a2) {
      name = a1;
      type = a2;
   }
   tree_node *copy()		 { return copy_Variable(); }
   Variable copy_Variable();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int);

};

class VariableDecl_class : public Decl_class {
protected:
   Variable variable;
public:
   VariableDecl_class(Variable a1) {
      variable = a1;
   }
   Decl copy_Decl();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int);   

};

class CallDecl_class : public Decl_class {
protected:
    Symbol   name; 
    Variables paras;
    Symbol   returnType;
    StmtBlock body;
    
public:
   CallDecl_class(Symbol a1, Variables a2, Symbol a3, StmtBlock a4) {
      name = a1;
      paras = a2;
      returnType = a3;
      body = a4;
   }
   Decl copy_Decl();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int);  
};

typedef class Decl_class *Decl;
typedef class Variable_class *Variable;
typedef class VariableDecl_class *VariableDecl;
typedef class CallDecl_class *CallDecl;


typedef list_node<Decl> Decls_class;
typedef Decls_class *Decls;
typedef list_node<VariableDecl> VariableDecls_class;
typedef VariableDecls_class *VariableDecls;
typedef list_node<Variable> Variables_class;
typedef Variables_class *Variables;


Decls nil_Decls();
Decls single_Decls(Decl);
Decls append_Decls(Decls,Decls);
VariableDecls nil_VariableDecls();
VariableDecls single_VariableDecls(VariableDecl);
VariableDecls append_VariableDecls(VariableDecls,VariableDecls);
Variables nil_Variables();
Variables single_Variables(Variable);
Variables append_Variables(Variables,Variables);

VariableDecl variableDecl(Variable);
Variable variable(Symbol,Symbol);
CallDecl callDecl(Symbol, Variables, Symbol, StmtBlock);

#endif
