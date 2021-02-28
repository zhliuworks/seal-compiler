
#ifndef _H_seal_expr
#define _H_seal_expr

#include "tree.h"
#include "seal-tree.handcode.h"
#include "seal-stmt.h"
#include "seal-decl.h"

typedef class Expr_class *Expr;
typedef class Actual_class *Actual;
typedef class Object_class *Object;
typedef class Call_class *Call;


class Expr_class : public Stmt_class {
public:     
   Symbol type;                      
   Stmt copy_Stmt() { return copy_Expr(); }   
   Symbol getType() { return type; }           
   Expr setType(Symbol s) { type = s; return this; }           
   Expr_class() { type = (Symbol) NULL; }
   Expr_class(Symbol a1) {
        type = a1;
   }
   void check(Symbol s) {checkType();}
   void dump_type(ostream&, int);
   
   virtual void dump_with_types(ostream&,int) = 0; 
	virtual void dump(ostream&,int) = 0;
   virtual Expr copy_Expr() = 0;
   virtual Symbol checkType() = 0;
   virtual bool is_empty_Expr() = 0;
   virtual void code(ostream&) = 0;
   virtual bool is_object() = 0; // add is_object
};

class Call_class : public Expr_class {
protected:
   Symbol name;
   Actuals actuals;
public:
   Call_class(Symbol a1, Actuals a2)  {
        name = a1;
        actuals = a2;
   }
   Symbol getName(){return name;}
   Actuals getActuals(){return actuals;}
   Expr copy_Expr();
   void dump_with_types(ostream&,int); 
	void dump(ostream&,int);
   void dump_type(ostream& , int );
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};


class Actual_class : public Expr_class {
protected:
    Expr expr;
public:
   Actual_class(Expr a1)  {
        expr = a1;
   }
   Expr copy_Expr();
   void dump_with_types(ostream&,int); 
	void dump(ostream&,int);
   void dump_type(ostream& , int );
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   Expr getExpr() { return expr; } // added
   bool is_object() {return false;} // add is_object
};

// define constructor - expr
class Assign_class : public Expr_class {
protected:
   Symbol lvalue;
   Expr value;
public:
   Assign_class(Symbol a1, Expr a2)  {
      lvalue = a1;
      value = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object   
};

// define constructor - add
class Add_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Add_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - minus
class Minus_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Minus_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - multi
class Multi_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Multi_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int);
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - divide
class Divide_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Divide_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - mod
class Mod_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Mod_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - -
class Neg_class : public Expr_class {
protected:
   Expr e1;
public:
   Neg_class(Expr a1) {
      e1 = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - <
class Lt_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Lt_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - <=
class Le_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Le_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - ==
class Equ_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Equ_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - !=
class Neq_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Neq_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - >=
class Ge_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Ge_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - >
class Gt_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Gt_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - and &&
class And_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   And_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - or ||
class Or_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Or_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - xor ^
class Xor_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Xor_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - not !
class Not_class : public Expr_class {
protected:
   Expr e1;
public:
   Not_class(Expr a1) {
      e1 = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructor - bitnot ~
class Bitnot_class : public Expr_class {
protected:
   Expr e1;
public:
   Bitnot_class(Expr a1) {
      e1 = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

class Bitand_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Bitand_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

class Bitor_class : public Expr_class {
protected:
   Expr e1;
   Expr e2;
public:
   Bitor_class(Expr a1, Expr a2) {
      e1 = a1;
      e2 = a2;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructconst_int - const_int
class Const_int_class : public Expr_class {
protected:
   Symbol value;
public:
   Const_int_class(Symbol a1) {
      value = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructconst_string - const_string
class Const_string_class : public Expr_class {
protected:
   Symbol value;
public:
   Const_string_class(Symbol a1) {
      value = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructconst_float - const_float
class Const_float_class : public Expr_class {
protected:
   Symbol value;
public:
   Const_float_class(Symbol a1) {
      value = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

// define constructconst_bool - const_bool
class Const_bool_class : public Expr_class {
protected:
   Boolean value;
public:
   Const_bool_class(Boolean a1) {
      value = a1;
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};

class Object_class : public Expr_class {
protected:
   Symbol var;
public:
   Object_class(Symbol a1) {
      var = a1;
   }
   Expr copy_Expr(){return copy_Object();};
   Object copy_Object();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return false;}
   Symbol checkType();
   void code(ostream&);
   Symbol getVar() { return var; } // added
   bool is_object() {return true;} // add is_object
};

// define constructor - no_expr
class No_expr_class : public Expr_class {
protected:
public:
   No_expr_class() {
   }
   Expr copy_Expr();
   void dump(ostream& stream, int n);
   void dump_with_types(ostream&,int); 
   bool is_empty_Expr(){ return true;}
   Symbol checkType();
   void code(ostream&);
   bool is_object() {return false;} // add is_object
};



typedef list_node<Expr> Exprs_class;
typedef Exprs_class *Exprs;
typedef list_node<Actual> Actuals_class;
typedef Actuals_class *Actuals;


Exprs nil_Exprs();
Exprs single_Exprs(Expr);
Exprs append_Exprs(Exprs,Exprs);


LValues nil_LValues();
LValues single_LValues(LValue);
LValues append_LValues(LValues,LValues);


Calls nil_Calls();
Calls single_Calls(Call);
Calls append_Calls(Calls,Calls);


Actuals nil_Actuals();
Actuals single_Actuals(Actual);
Actuals append_Actuals(Actuals,Actuals);


Constants nil_Constants();
Constants single_Constants(Constant);
Constants append_Constants(Constants,Constants);


Expr assign(Symbol, Expr);
Expr add( Expr,  Expr);
Expr minus( Expr, Expr);
Expr divide(Expr, Expr);
Expr mod(Expr, Expr);
Expr multi(Expr, Expr);
Expr neg(Expr);
Expr lt(Expr, Expr);
Expr le(Expr, Expr);
Expr equ(Expr, Expr);
Expr neq(Expr, Expr);
Expr ge(Expr, Expr);
Expr gt(Expr, Expr);
Expr and_(Expr, Expr);  // Bool && Bool
Expr or_(Expr, Expr);   // Bool || Bool
Expr xor_(Expr, Expr);  // Bool ^ Bool, Int ^ Int
Expr not_(Expr);        // !Bool
Expr bitand_(Expr, Expr);  // Int & int
Expr bitor_(Expr, Expr);   // Int | Int
Expr bitnot(Expr);         // ~Int
Expr object(Symbol);
Call call(Symbol, Actuals);
Actuals actuals(Exprs);
Actual actual(Expr);
Expr const_int(Symbol);
Expr const_bool(Boolean);
Expr const_string(Symbol);
Expr const_float(Symbol);
Expr no_expr();

#endif
