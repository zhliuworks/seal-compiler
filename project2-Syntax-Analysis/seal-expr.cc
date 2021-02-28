#include "tree.h"
#include "seal-decl.h"
#include "seal-expr.h"
#include "seal-stmt.h"


Expr Assign_class::copy_Expr()
{
   return new Assign_class(copy_Symbol(lvalue), value->copy_Expr());
}


void Assign_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_assign\n";
   dump_Symbol(stream, n+2, lvalue);
   value->dump(stream, n+2);
}

Expr Add_class::copy_Expr()
{
   return new Add_class(e1->copy_Expr(), e2->copy_Expr());
}


void Add_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_add\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Minus_class::copy_Expr()
{
   return new Minus_class(e1->copy_Expr(), e2->copy_Expr());
}


void Minus_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_minus\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Multi_class::copy_Expr()
{
   return new Multi_class(e1->copy_Expr(), e2->copy_Expr());
}


void Multi_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_multi\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Divide_class::copy_Expr()
{
   return new Divide_class(e1->copy_Expr(), e2->copy_Expr());
}


void Divide_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_divide\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Mod_class::copy_Expr()
{
   return new Mod_class(e1->copy_Expr(), e2->copy_Expr());
}


void Mod_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_mod\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}


Expr Neg_class::copy_Expr()
{
   return new Neg_class(e1->copy_Expr());
}


void Neg_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_neg\n";
   e1->dump(stream, n+2);
}

Expr Lt_class::copy_Expr()
{
   return new Lt_class(e1->copy_Expr(), e2->copy_Expr());
}


void Lt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_lt\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Le_class::copy_Expr()
{
   return new Le_class(e1->copy_Expr(), e2->copy_Expr());
}


void Le_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_le\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Equ_class::copy_Expr()
{
   return new Equ_class(e1->copy_Expr(), e2->copy_Expr());
}


void Equ_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_equ\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Neq_class::copy_Expr()
{
   return new Neq_class(e1->copy_Expr(), e2->copy_Expr());
}


void Neq_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_neq\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Ge_class::copy_Expr()
{
   return new Ge_class(e1->copy_Expr(), e2->copy_Expr());
}


void Ge_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_ge\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr Gt_class::copy_Expr()
{
   return new Gt_class(e1->copy_Expr(), e2->copy_Expr());
}


void Gt_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_gt\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}

Expr And_class::copy_Expr()
{
   return new And_class(e1->copy_Expr(), e2->copy_Expr());
}


void And_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_and\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}


Expr Or_class::copy_Expr()
{
   return new Or_class(e1->copy_Expr(), e2->copy_Expr());
}


void Or_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_or\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}


Expr Xor_class::copy_Expr()
{
   return new Xor_class(e1->copy_Expr(), e2->copy_Expr());
}


void Xor_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_xor\n";
   e1->dump(stream, n+2);
   e2->dump(stream, n+2);
}


Expr Not_class::copy_Expr()
{
   return new Not_class(e1->copy_Expr());
}


void Not_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_not\n";
   e1->dump(stream, n+2);
}


Expr Bitnot_class::copy_Expr()
{
   return new Bitnot_class(e1->copy_Expr());
}


void Bitnot_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_bitnot\n";
   e1->dump(stream, n+2);
}

Expr Bitand_class::copy_Expr()
{
   return new Bitand_class(e1->copy_Expr(), e2->copy_Expr());
}


void Bitand_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_bitand\n";
   e1->dump(stream, n+2);
}

Expr Bitor_class::copy_Expr()
{
   return new Bitor_class(e1->copy_Expr(), e2->copy_Expr());
}


void Bitor_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_bitor\n";
   e1->dump(stream, n+2);
}

Object Object_class::copy_Object()
{
   return new Object_class(copy_Symbol(var));
}

void Object_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_object\n";
   dump_Symbol(stream, n+2, var);
}


Expr Call_class::copy_Expr()
{
   return new Call_class(copy_Symbol(name), actuals->copy_list());
}

void Call_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_call\n";
   dump_Symbol(stream, n+2, name);
   actuals->dump(stream, n+2);
}

Expr Actual_class::copy_Expr()
{
   return new Actual_class(expr->copy_Expr());
}

void Actual_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_actual\n";
   expr->dump(stream, n+2);
}


Expr Const_int_class::copy_Expr()
{
   return new Const_int_class(copy_Symbol(value));
}

void Const_int_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_const_int\n";
   dump_Symbol(stream, n+2, value);
}

Expr Const_string_class::copy_Expr()
{
   return new Const_string_class(copy_Symbol(value));
}

void Const_string_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_const_string\n";
   dump_Symbol(stream, n+2, value);
}


Expr Const_float_class::copy_Expr()
{
   return new Const_float_class(copy_Symbol(value));
}

void Const_float_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_const_float\n";
   dump_Symbol(stream, n+2, value);
}


Expr Const_bool_class::copy_Expr()
{
   return new Const_bool_class(copy_Boolean(value));
}

void Const_bool_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_const_bool\n";
   dump_Boolean(stream, n+2, value);
}

Expr No_expr_class::copy_Expr()
{
   return new No_expr_class();
}


void No_expr_class::dump(ostream& stream, int n)
{
   stream << pad(n) << "_no_expr\n";
}


// interfaces used by Bison




Exprs nil_Exprs()
{
   return new nil_node<Expr>();
}
Exprs single_Exprs(Expr e)
{
   return new single_list_node<Expr>(e);
}
Exprs append_Exprs(Exprs p1, Exprs p2)
{
   return new append_node<Expr>(p1, p2);
}


Actuals nil_Actuals()
{
   return new nil_node<Actual>();
}
Actuals single_Actuals(Actual e)
{
   return new single_list_node<Actual>(e);
}
Actuals append_Actuals(Actuals p1, Actuals p2)
{
   return new append_node<Actual>(p1, p2);
}


Expr assign(Symbol a1, Expr a2)
{
  return new Assign_class(a1, a2);
}

Expr add(Expr a1,  Expr a2)
{
	return new Add_class(a1, a2);
}

Expr minus(Expr a1,  Expr a2)
{
	return new Minus_class(a1, a2);
}

Expr divide(Expr a1, Expr a2)
{
  return new Divide_class(a1, a2);
}

Expr mod(Expr a1, Expr a2)
{
  return new Mod_class(a1, a2);
}

Expr multi(Expr a1, Expr a2)
{
  return new Multi_class(a1, a2);
}

Expr neg(Expr a1)
{
  return new Neg_class(a1);
}

Expr lt(Expr a1, Expr a2)
{
  return new Lt_class(a1, a2);
}

Expr le(Expr a1, Expr a2)
{
  return new Le_class(a1, a2);
}

Expr equ(Expr a1, Expr a2)
{
  return new Equ_class(a1, a2);
}

Expr neq(Expr a1, Expr a2)
{
  return new Neq_class(a1, a2);
}

Expr ge(Expr a1, Expr a2)
{
  return new Ge_class(a1, a2);
}

Expr gt(Expr a1, Expr a2)
{
  return new Gt_class(a1, a2);
}

Expr and_(Expr a1, Expr a2)
{
  return new And_class(a1, a2);
}

Expr or_(Expr a1, Expr a2)
{
  return new Or_class(a1, a2);
}

Expr xor_(Expr a1, Expr a2)
{
  return new Xor_class(a1, a2);
}

Expr not_(Expr a1)
{
  return new Not_class(a1);
}

Expr bitand_(Expr a1, Expr a2)
{
  return new Bitand_class(a1, a2);
}

Expr bitor_(Expr a1, Expr a2)
{
  return new Bitor_class(a1, a2);
}

Expr bitnot(Expr a1)
{
  return new Bitnot_class(a1);
}
Expr object(Symbol a1)
{
  return new Object_class(a1);
}

Call call(Symbol a1, Actuals a2)
{
  return new Call_class(a1, a2);
}

Actual actual(Expr a1)
{
  return new Actual_class(a1);
}

Expr const_int(Symbol a1)
{
	return new Const_int_class(a1);
}

Expr const_bool(Boolean a1)
{
	return new Const_bool_class(a1);
}

Expr const_string(Symbol a1)
{
	return new Const_string_class(a1);
}

Expr const_float(Symbol a1)
{
	return new Const_float_class(a1);
}

Expr no_expr()
{
	return new No_expr_class();
}
