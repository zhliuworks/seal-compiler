
//**************************************************************
//
// Code generator SKELETON
//
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <stack>
#include <vector>

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

static char *CALL_REGS[] = {RDI, RSI, RDX, RCX, R8, R9};
static char *CALL_XMM[] = {XMM0, XMM1, XMM2, XMM3};

void cgen_helper(Decls decls, ostream& s);
void code(Decls decls, ostream& s);

struct TempVar { // include Int/Bool/Float constants, varName == varType
  Symbol varName;
  Symbol varType;
  int varAddr;
};

std::stack<TempVar> tempVarStack;
std::map<Symbol, Symbol> globalVarMap;
std::stack<int> breakPosStack, continuePosStack;
std::map<Symbol, Symbol> callTypeMap;

int posIdx = 0;


//////////////////////////////////////////////////////////////////
//
//
//    Helper Functions
//  
//
//////////////////////////////////////////////////////////////////

TempVar searchObj(Expr value) {
  Symbol objvar = static_cast<Object>(value)->getVar();
  std::stack<TempVar> temp = tempVarStack;
  while(temp.top().varName != objvar) {
    temp.pop();
  }
  return temp.top();
}

int calcExprAddr(Expr e) {
  if(e->is_object()) {
    return searchObj(e).varAddr;
  } else {
    return -8 * tempVarStack.size();
  } 
}

Symbol calcExprType(Expr e) {
  if(e->is_object()) {
    return searchObj(e).varType;
  } else {
    return tempVarStack.top().varType;
  } 
}

void getFloatBytes(char *floatBytes, Symbol value) {
  double floatVal = atof(value->get_string());
  unsigned char *p = (unsigned char *)&floatVal;
  int byte;
  
  for(int i=0; i<8; i++) {
    byte = (int)*(p + i);
    if(byte / 16 < 10) {
      floatBytes[16 - 2 * i] = byte / 16 + '0';
    } else {
      floatBytes[16 - 2 * i] = byte / 16 - 10 + 'a';
    }

    if(byte % 16 < 10) {
      floatBytes[17 - 2 * i] = byte % 16 + '0';
    } else {
      floatBytes[17 - 2 * i] = byte % 16 - 10 + 'a';
    }
  }
  
  floatBytes[0] = '0';
  floatBytes[1] = 'x';
  floatBytes[18] = '\0';
}

int getStrIdx(Symbol value) {
  for(int i=stringtable.first(); stringtable.more(i); i=stringtable.next(i)) {
    if(strcmp(stringtable.lookup(i)->get_string(), value->get_string()) == 0) {
      return i;
    }
  }
  return -1;
}

bool isGlobalObj(Expr value) {
  if(!value->is_object()) {
    return false;
  }

  Symbol objvar = static_cast<Object>(value)->getVar();
  std::stack<TempVar> temp = tempVarStack;
  while(temp.top().varName != objvar) {
    temp.pop();
    if(temp.empty()) {
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}


//*********************************************************
//
// Define method for code generation
//
//
//*********************************************************

void Program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  cgen_helper(decls,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_mov(const char *source, const char *dest, ostream& s)
{
  s << MOV << source << COMMA << dest << endl;
}

static void emit_rmmov(const char *source_reg, int offset, const char *base_reg, ostream& s)
{
  s << MOV << source_reg << COMMA << offset << "(" << base_reg << ")"
      << endl;
}

static void emit_mrmov(const char *base_reg, int offset, const char *dest_reg, ostream& s)
{
  s << MOV << offset << "(" << base_reg << ")" << COMMA << dest_reg  
      << endl;
}

static void emit_irmov(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_irmovl(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOVL << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_immov(const char *immidiate, int offset, const char *base_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << "(" << offset << ")" << base_reg  
      << endl;
}

static void emit_add(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADD << source_reg << COMMA << dest_reg << endl;
}

static void emit_sub(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUB << source_reg << COMMA << dest_reg << endl;
}

static void emit_mul(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MUL << source_reg << COMMA << dest_reg << endl;
}

static void emit_div(const char *dest_reg, ostream& s)
{
  s << DIV << dest_reg << endl;
}

static void emit_cqto(ostream &s)
{
  s << CQTO << endl;
}

static void emit_neg(const char *dest_reg, ostream& s)
{
  s << NEG << dest_reg << endl;
}

static void emit_and(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << AND << source_reg << COMMA << dest_reg << endl;
}

static void emit_or(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << OR << source_reg << COMMA << dest_reg << endl;
}

static void emit_xor(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XOR << source_reg << COMMA << dest_reg << endl;
}

static void emit_not(const char *dest_reg, ostream& s)
{
  s << NOT << " " << dest_reg << endl;
}

static void emit_movsd(const char *source, const char *dest, ostream& s)
{
  s << MOVSD << source << COMMA << dest << endl;
}

static void emit_movaps(const char *source, const char *dest, ostream& s)
{
  s << MOVAPS << source << COMMA << dest << endl;
}

static void emit_addsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADDSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_subsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUBSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_mulsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MULSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_divsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << DIVSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_cmp(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << CMP << source_reg << COMMA << dest_reg << endl;
}

static void emit_test(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << TEST << source_reg << COMMA << dest_reg << endl;
}

static void emit_ucompisd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << UCOMPISD << source_reg << COMMA << dest_reg << endl;
}

static void emit_xorpd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XORPD << source_reg << COMMA << dest_reg << endl;
}
static void emit_jmp(const char *dest, ostream& s)
{
  s << JMP << " " << dest << endl;
}

static void emit_jl(const char *dest, ostream& s)
{
  s << JL << " " << dest << endl;
}

static void emit_jle(const char *dest, ostream& s)
{
  s << JLE << " " << dest << endl;
}

static void emit_je(const char *dest, ostream& s)
{
  s << JE << " " << dest << endl;
}

static void emit_jne(const char *dest, ostream& s)
{
  s << JNE << " " << dest << endl;
}

static void emit_jg(const char *dest, ostream& s)
{
  s << JG << " " << dest << endl;
}

static void emit_jge(const char *dest, ostream& s)
{
  s << JGE << " " << dest << endl;
}

static void emit_jb(const char *dest, ostream& s)
{
  s << JB << " " << dest << endl;
}

static void emit_jbe(const char *dest, ostream& s)
{
  s << JBE << " " << dest << endl;
}

static void emit_ja(const char *dest, ostream& s)
{
  s << JA << " " << dest << endl;
}

static void emit_jae(const char *dest, ostream& s)
{
  s << JAE << " " << dest << endl;
}

static void emit_jp(const char *dest, ostream& s)
{
  s << JP << " " << dest << endl;
}

static void emit_jz(const char *dest, ostream& s)
{
  s << JZ << " " << dest << endl;
}

static void emit_jnz(const char *dest, ostream& s)
{
  s << JNZ << " " << dest << endl;
}

static void emit_call(const char *dest, ostream& s)
{
  s << CALL << " " << dest << endl;
}

static void emit_ret(ostream& s)
{
  s << RET << endl;
}

static void emit_push(const char *reg, ostream& s)
{
  s << PUSH << " " << reg << endl;
}

static void emit_pop(const char *reg, ostream& s)
{
  s << POP << " " << reg << endl;
}

static void emit_leave(ostream& s)
{
  s << LEAVE << endl;
}

static void emit_position(const char *p, ostream& s)
{
  s << p << ":" << endl;
}

static void emit_float_to_int(const char *float_mmx, const char *int_reg, ostream& s)
{
  s << CVTTSD2SIQ << float_mmx << COMMA << int_reg << endl;
}

static void emit_int_to_float(const char *int_reg, const char *float_mmx, ostream& s)
{
  s << CVTSI2SDQ << int_reg << COMMA << float_mmx << endl;
}
///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Seal has four kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// If you like, you can add any ***Entry::code_def() and ***Entry::code_ref()
// functions to help.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << "$" << STRINGCONST_PREFIX << index;
}

//
// Emit code for a constant String.
//

void StringEntry::code_def(ostream& s)
{
  s << STRINGCONST_PREFIX << index << ":" << endl;
  s  << STRINGTAG ; emit_string_constant(s,str);                                                // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s);
}

// the following 2 functions are useless, please DO NOT care about them
void FloatEntry::code_ref(ostream &s)
{
  s << FLOATTAG << index;
}

void IntEntry::code_def(ostream &s)
{
  s << GLOBAL;
}

//***************************************************
//
//  Emit global var and functions.
//
//***************************************************

static void emit_global_int(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  INTTAG << 0 << endl;
}

static void emit_global_float(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl <<
  FLOATTAG << 0 << endl <<
  FLOATTAG << 0 << endl;
}

static void emit_global_bool(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  BOOLTAG << 0 << endl;
}

void code_global_data(Decls decls, ostream &str)
{
  bool hasData = false;
  Decl curr_decl;
  Symbol name;
  Symbol type;

  for(int i=decls->first(); decls->more(i); i=decls->next(i)) {
    curr_decl = decls->nth(i);
    if(!curr_decl->isCallDecl()) {
      if(!hasData) {
        str << DATA << endl;
        hasData = true;
      } 
      name = curr_decl->getName();
      type = curr_decl->getType();  
      globalVarMap[name] = type;             
      if(type == Int) {
        emit_global_int(name, str);
      } else if(type == Float) {
        emit_global_float(name, str);
      } else if(type == Bool) {
        emit_global_bool(name, str);
      }
    }
  }
}

void code_calls(Decls decls, ostream &str) {
  str << SECTION << RODATA << endl;
  stringtable.code_string_table(str);
  str << TEXT << endl;

  Decl curr_decl;
  CallDecl curr_call;
  for(int i=decls->first(); decls->more(i); i=decls->next(i)) {
    curr_decl = decls->nth(i);
    if(curr_decl->isCallDecl()) {
      curr_call = static_cast<CallDecl>(curr_decl);
      curr_call->code(str);
    }
  }
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************



//********************************************************
//
// Cgen helper helps to initialize and call code() function.
// You can do any initializing operations here
//
//********************************************************

void cgen_helper(Decls decls, ostream& s)
{
  code(decls, s);
}


void code(Decls decls, ostream& s)
{
  if (cgen_debug) cout << "Coding global data" << endl;
  code_global_data(decls, s);

  if (cgen_debug) cout << "Coding calls" << endl;
  code_calls(decls, s);
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `seal-decl.h', `seal-expr.h' and `seal-stmt.h'
//   Sample code for constant integers, strings, and booleans are provided.
//   
//*****************************************************************

void CallDecl_class::code(ostream &s) {
  s << GLOBAL << name << endl;
  s << SYMBOL_TYPE << name << COMMA << FUNCTION << endl;
  s << name << ":" << endl;

  emit_push(RBP, s);
  emit_mov(RSP, RBP, s);
  emit_push(RBX, s);
  emit_push(R10, s);
  emit_push(R11, s);
  emit_push(R12, s);
  emit_push(R13, s);
  emit_push(R14, s);
  emit_push(R15, s); 

  // detect func "main"
  if(strcmp(getName()->get_string(), "main") != 0) {
    TempVar tv;
    tv.varName = Void;
    tv.varType = Void;
    tv.varAddr = -8 * (tempVarStack.size() + 1);  
    for(int i=0; i<7; i++) {
      tempVarStack.push(tv);   
    }
  }

  callTypeMap[name] = returnType;

  Symbol paraType;
  Symbol paraName;

  int freg = 0;
  int ufreg = 0;

  for(int i=paras->first(); paras->more(i); i=paras->next(i)) {
    emit_sub("$8", RSP, s);
    paraName = paras->nth(i)->getName();
    paraType = paras->nth(i)->getType();

    TempVar tv;
    tv.varName = paraName;
    tv.varType = paraType;
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);
    
    
    if(paraType == Float) {
      freg += 1;
      switch(freg) {
        case 1: emit_rmmov(XMM0, tv.varAddr, RBP, s); break;
        case 2: emit_rmmov(XMM1, tv.varAddr, RBP, s); break;
        case 3: emit_rmmov(XMM2, tv.varAddr, RBP, s); break;
        case 4: emit_rmmov(XMM3, tv.varAddr, RBP, s); break;
        case 5: emit_rmmov(XMM4, tv.varAddr, RBP, s); break;
        case 6: emit_rmmov(XMM5, tv.varAddr, RBP, s); break;
      }
    } else {
      ufreg += 1;
      switch(ufreg) {
        case 1: emit_rmmov(RDI, tv.varAddr, RBP, s); break;
        case 2: emit_rmmov(RSI, tv.varAddr, RBP, s); break;
        case 3: emit_rmmov(RDX, tv.varAddr, RBP, s); break;
        case 4: emit_rmmov(RCX, tv.varAddr, RBP, s); break;
        case 5: emit_rmmov(R8, tv.varAddr, RBP, s); break;
        case 6: emit_rmmov(R9, tv.varAddr, RBP, s); break;
      }
    }
  }
 
  body->code(s);

  for(int i=paras->first(); paras->more(i); i=paras->next(i)) {
    TempVar tv;
    do {
      tv = tempVarStack.top();
      tempVarStack.pop();
    } while(tv.varName == tv.varType); // for constants
  }

  s << SIZE << name << COMMA << ".-" << name << endl;
}


void StmtBlock_class::code(ostream &s){
  for(int i=vars->first(); vars->more(i); i=vars->next(i)) {
    emit_sub("$8", RSP, s);
    TempVar tv;
    tv.varName = vars->nth(i)->getName();
    tv.varType = vars->nth(i)->getType();
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);    
  }

  for(int i=stmts->first(); stmts->more(i); i=stmts->next(i)) {
    stmts->nth(i)->code(s);
  }

  for(int i=vars->first(); vars->more(i); i=vars->next(i)) {
    TempVar tv;
    do {
      tv = tempVarStack.top();
      tempVarStack.pop();
    } while(tv.varName == tv.varType); // for constants
  }  
}


void IfStmt_class::code(ostream &s) {
  int reIdx = posIdx;
  posIdx += 2;
  condition->code(s);
  emit_mrmov(RBP, tempVarStack.top().varAddr, RAX, s);
  emit_test(RAX, RAX, s);
  emit_jz((".POS" + std::to_string(reIdx)).c_str(), s);
  thenexpr->code(s);
  emit_jmp((".POS" + std::to_string(reIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(reIdx)).c_str(), s);
  elseexpr->code(s);
  emit_position((".POS" + std::to_string(reIdx + 1)).c_str(), s);
}


void WhileStmt_class::code(ostream &s) {
  int reIdx = posIdx;
  breakPosStack.push(reIdx + 1);
  continuePosStack.push(reIdx);
  emit_position((".POS" + std::to_string(reIdx)).c_str(), s);
  posIdx += 2;
  condition->code(s);
  emit_mrmov(RBP, tempVarStack.top().varAddr, RAX, s);
  emit_test(RAX, RAX, s);
  emit_jz((".POS" + std::to_string(reIdx + 1)).c_str(), s);
  body->code(s);
  emit_jmp((".POS" + std::to_string(reIdx)).c_str(), s);
  emit_position((".POS" + std::to_string(reIdx + 1)).c_str(), s);
  breakPosStack.pop();
  continuePosStack.pop();
}


void ForStmt_class::code(ostream &s) {
  int reIdx = posIdx;
  breakPosStack.push(reIdx + 2);
  continuePosStack.push(reIdx + 1);

  if(!initexpr->is_empty_Expr()) {
    initexpr->code(s); 
  }

  posIdx += 3;

  if(!condition->is_empty_Expr()) {
    emit_position((".POS" + std::to_string(reIdx)).c_str(), s);  
    condition->code(s);
    emit_mrmov(RBP, tempVarStack.top().varAddr, RAX, s);
    emit_test(RAX, RAX, s); 
    emit_jz((".POS" + std::to_string(reIdx + 2)).c_str(), s);
  }

  body->code(s);
  emit_position((".POS" + std::to_string(reIdx + 1)).c_str(), s);

  if(!loopact->is_empty_Expr()) {
    loopact->code(s);  
  }

  if(!condition->is_empty_Expr()) {
    emit_jmp((".POS" + std::to_string(reIdx)).c_str(), s);
  }
 
  emit_position((".POS" + std::to_string(reIdx + 2)).c_str(), s);
  breakPosStack.pop();
  continuePosStack.pop();
}


void ReturnStmt_class::code(ostream &s) {
  if(!value->is_empty_Expr()) {
    value->code(s);

    if(value->is_object()) { // return data stored in corresponding object
      TempVar tv = searchObj(value);
      if(tv.varType == Int || tv.varType == Bool || tv.varType == String) {
        emit_mrmov(RBP, searchObj(value).varAddr, RAX, s);
      } else if(tv.varType == Float) {
        s << MOVAPS << searchObj(value).varAddr << "(" << RBP << ")" << COMMA << XMM0 << endl;
      }
    } else { // return data stored in address of stack top
      Symbol valueType = tempVarStack.top().varType;
      if(valueType == Int || valueType == Bool || valueType == String) {
        emit_mrmov(RBP, tempVarStack.top().varAddr, RAX, s); 
      } else if(valueType == Float) {
        s << MOVAPS << tempVarStack.top().varAddr << "(" << RBP << ")" << COMMA << XMM0 << endl;
      }
    }
  }

  emit_pop(R15, s);
  emit_pop(R14, s);
  emit_pop(R13, s);
  emit_pop(R12, s);
  emit_pop(R11, s);
  emit_pop(R10, s);
  emit_pop(RBX, s);
  
  s << LEAVE << endl << RET << endl;
}


void ContinueStmt_class::code(ostream &s) {
  emit_jmp((".POS" + std::to_string(continuePosStack.top())).c_str(), s);
}


void BreakStmt_class::code(ostream &s) {
  emit_jmp((".POS" + std::to_string(breakPosStack.top())).c_str(), s);
}


void Call_class::code(ostream &s) {
  Symbol call_name = getName();
  Actuals actual_list = getActuals();
  Actual curr_act;
  std::vector<Symbol> actType;
  std::vector<int> actAddr;
  
  for(int i=actual_list->first(); actual_list->more(i); i=actual_list->next(i)) {
    curr_act = actual_list->nth(i);
    curr_act->code(s);
    actType.push_back(calcExprType(curr_act->getExpr()));
    actAddr.push_back(calcExprAddr(curr_act->getExpr()));
  }

  int freg = 0;
  int ufreg = 0;
  
  for(unsigned int i=0; i<actType.size(); i++) {
    if(actType.at(i) == Float) {
      freg += 1;
      switch(freg) {
        case 1: s << MOVSD << actAddr.at(i) << "(" << RBP << ")" << COMMA << XMM0 << endl; break;
        case 2: s << MOVSD << actAddr.at(i) << "(" << RBP << ")" << COMMA << XMM1 << endl; break;
        case 3: s << MOVSD << actAddr.at(i) << "(" << RBP << ")" << COMMA << XMM2 << endl; break;
        case 4: s << MOVSD << actAddr.at(i) << "(" << RBP << ")" << COMMA << XMM3 << endl; break;
        case 5: s << MOVSD << actAddr.at(i) << "(" << RBP << ")" << COMMA << XMM4 << endl; break;
        case 6: s << MOVSD << actAddr.at(i) << "(" << RBP << ")" << COMMA << XMM5 << endl; break;
      }
    } else {
      ufreg += 1;
      switch(ufreg) {
        case 1: emit_mrmov(RBP, actAddr.at(i), RDI, s); break;
        case 2: emit_mrmov(RBP, actAddr.at(i), RSI, s); break;
        case 3: emit_mrmov(RBP, actAddr.at(i), RDX, s); break;
        case 4: emit_mrmov(RBP, actAddr.at(i), RCX, s); break;
        case 5: emit_mrmov(RBP, actAddr.at(i), R8, s); break;
        case 6: emit_mrmov(RBP, actAddr.at(i), R9, s); break;
      }
    }     
  }

  if(strcmp(call_name->get_string(), "printf") == 0) {
    emit_sub("$8", RSP, s);
    emit_irmovl(std::to_string(freg).c_str(), EAX, s);
    emit_call("printf", s);
    return;
  }

  emit_call(call_name->get_string(), s);

  emit_sub("$8", RSP, s);

  TempVar tv;
  tv.varName = callTypeMap[call_name]; 
  tv.varType = callTypeMap[call_name]; 
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);  
  if(tv.varType == Float) {
    s << MOVSD << XMM0 << COMMA << tempVarStack.top().varAddr << "(" << RBP << ")" << endl;
  } else {
    emit_rmmov(RAX, tempVarStack.top().varAddr, RBP, s);
  } 
}


void Actual_class::code(ostream &s) {
  expr->code(s);
}


void Assign_class::code(ostream &s) {
  value->code(s);
  if(value->is_object()) {
    emit_mrmov(RBP, searchObj(value).varAddr, RAX, s);
  } else {
    emit_mrmov(RBP, tempVarStack.top().varAddr, RAX, s);
  }

  std::stack<TempVar> temp = tempVarStack;
  while(temp.top().varName != lvalue) {
    temp.pop();
    if(temp.empty()) {
      s << MOV << RAX << COMMA << lvalue << "(" << RIP << ")" << endl;
      return;
    }
  }
  emit_rmmov(RAX, temp.top().varAddr, RBP, s);    
}


void Add_class::code(ostream &s) { // ? tried detecting global variables but failed, only tried `Add_class'
  bool isGlob1 = isGlobalObj(e1);
  bool isGlob2 = isGlobalObj(e2);
  int offset1, offset2;
  Symbol exprType1, exprType2;

  if(isGlob1) {
    exprType1 = globalVarMap[static_cast<Object>(e1)->getVar()];
  } else {
    e1->code(s);
    offset1 = calcExprAddr(e1);
    exprType1 = calcExprType(e1);
  }

  if(isGlob2) {
    exprType2 = globalVarMap[static_cast<Object>(e2)->getVar()];
  } else {
    e2->code(s);
    offset2 = calcExprAddr(e2);
    exprType2 = calcExprType(e2);
  } 

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    if(isGlob1) {
      s << MOV << static_cast<Object>(e1)->getVar() << "(" << RIP << ")" << COMMA << RBX << endl;
    } else {
      emit_mrmov(RBP, offset1, RBX, s);
    }

    if(isGlob2) {
      s << MOV << static_cast<Object>(e2)->getVar() << "(" << RIP << ")" << COMMA << R10 << endl;
    } else {
      emit_mrmov(RBP, offset2, R10, s);
    }
       
    emit_add(RBX, R10, s);

    TempVar tv;
    tv.varName = Int; // intermediate results are seen as constants
    tv.varType = Int; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    emit_rmmov(R10, tv.varAddr, RBP, s);
    return;
  } 

  if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RBX, s);
    emit_int_to_float(RBX, XMM4, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_addsd(XMM4, XMM5, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM5 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RBX, s);
    emit_int_to_float(RBX, XMM5, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    emit_addsd(XMM4, XMM5, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM5 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Float) {
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_addsd(XMM4, XMM5, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM5 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }
}


void Minus_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);
  
  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RBX, s);
    emit_mrmov(RBP, offset2, R10, s);
    emit_sub(R10, RBX, s);

    TempVar tv;
    tv.varName = Int;
    tv.varType = Int;
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    emit_rmmov(RBX, tv.varAddr, RBP, s); 
    return;
  }

  if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RBX, s);
    emit_int_to_float(RBX, XMM4, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_subsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RBX, s);
    emit_int_to_float(RBX, XMM5, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    emit_subsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Float) {
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_subsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }  
}


void Multi_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);
  
  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RBX, s);
    emit_mrmov(RBP, offset2, R10, s);
    emit_mul(R10, RBX, s);

    TempVar tv;
    tv.varName = Int;
    tv.varType = Int;
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    emit_rmmov(RBX, tv.varAddr, RBP, s);
    return;
  }

  if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RBX, s);
    emit_int_to_float(RBX, XMM4, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_mulsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RBX, s);
    emit_int_to_float(RBX, XMM5, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    emit_mulsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Float) {
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_mulsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }   
}


void Divide_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);
  
  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_cqto(s);
    emit_mrmov(RBP, offset2, RBX, s);
    emit_div(RBX, s);

    TempVar tv;
    tv.varName = Int;
    tv.varType = Int;
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    emit_rmmov(RAX, tv.varAddr, RBP, s); 
    return;
  }

   if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RBX, s);
    emit_int_to_float(RBX, XMM4, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_divsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RBX, s);
    emit_int_to_float(RBX, XMM5, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    emit_divsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }

  if(exprType1 == Float && exprType2 == Float) {
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM4 << endl;
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM5 << endl;
    emit_divsd(XMM5, XMM4, s);

    TempVar tv;
    tv.varName = Float; 
    tv.varType = Float; 
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    s << MOVSD << XMM4 << COMMA << tv.varAddr << "(" << RBP << ")" << endl;
    return;
  }    
}


void Mod_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  
  e2->code(s);
  int offset2 = calcExprAddr(e2);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset1, RAX, s);
  emit_cqto(s);
  emit_mrmov(RBP, offset2, RBX, s);
  emit_div(RBX, s);

  TempVar tv;
  tv.varName = Int;
  tv.varType = Int;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RDX, tv.varAddr, RBP, s); 
}


void Neg_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  emit_sub("$8", RSP, s);  

  if(exprType1 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_neg(RAX, s);

    TempVar tv;
    tv.varName = Int;
    tv.varType = Int;
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    emit_rmmov(RAX, tv.varAddr, RBP, s); 
    return;
  }

  if(exprType1 == Float) {
    // emit_sub("$8", RSP, s);  
    emit_irmov("0x8000000000000000", RAX, s);
    emit_mrmov(RBP, offset1, RDX, s);
    emit_xor(RAX, RDX, s);

    TempVar tv;
    tv.varName = Float;
    tv.varType = Float;
    tv.varAddr = -8 * (tempVarStack.size() + 1);
    tempVarStack.push(tv);   
    emit_rmmov(RDX, tv.varAddr, RBP, s); 
    return;
  }
}


void Lt_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_mrmov(RBP, offset2, RDX, s);
    emit_cmp(RDX, RAX, s);

    emit_jl((".POS" + std::to_string(posIdx)).c_str(), s);
  } 
  
  else if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);

    emit_jb((".POS" + std::to_string(posIdx)).c_str(), s);    
  }

  else if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);

    emit_jb((".POS" + std::to_string(posIdx)).c_str(), s);      
  }

  else {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);

    emit_jb((".POS" + std::to_string(posIdx)).c_str(), s); 
  }

  emit_irmov("0", RAX, s);
  emit_jmp((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(posIdx)).c_str(), s);
  emit_irmov("1", RAX, s);
  emit_position((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  posIdx += 2;

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s);
}


void Le_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_mrmov(RBP, offset2, RDX, s);
    emit_cmp(RDX, RAX, s);

    emit_jle((".POS" + std::to_string(posIdx)).c_str(), s);
  }

  else if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);

    emit_jbe((".POS" + std::to_string(posIdx)).c_str(), s);    
  }

  else if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);

    emit_jbe((".POS" + std::to_string(posIdx)).c_str(), s);      
  }

  else {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);

    emit_jbe((".POS" + std::to_string(posIdx)).c_str(), s); 
  }

  emit_irmov("0", RAX, s);
  emit_jmp((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(posIdx)).c_str(), s);
  emit_irmov("1", RAX, s);
  emit_position((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  posIdx += 2;

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s); 
}


void Equ_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_mrmov(RBP, offset2, RDX, s);
    emit_cmp(RDX, RAX, s);
  } 
  
  else if(exprType1 == Bool && exprType2 == Bool) {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);
  }

  else if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);  
  }

  else if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);    
  }

  else {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);
  }

  emit_je((".POS" + std::to_string(posIdx)).c_str(), s);

  emit_irmov("0", RAX, s);
  emit_jmp((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(posIdx)).c_str(), s);
  emit_irmov("1", RAX, s);
  emit_position((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  posIdx += 2;

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Neq_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_mrmov(RBP, offset2, RDX, s);
    emit_cmp(RDX, RAX, s);
  }
  
  else if(exprType1 == Bool && exprType2 == Bool) {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);
  }

  else if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);  
  }

  else if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);    
  }

  else {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);
  }

  emit_jne((".POS" + std::to_string(posIdx)).c_str(), s);

  emit_irmov("0", RAX, s);
  emit_jmp((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(posIdx)).c_str(), s);
  emit_irmov("1", RAX, s);
  emit_position((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  posIdx += 2;

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Ge_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_mrmov(RBP, offset2, RDX, s);
    emit_cmp(RDX, RAX, s);

    emit_jge((".POS" + std::to_string(posIdx)).c_str(), s);
  }

  else if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);

    emit_jae((".POS" + std::to_string(posIdx)).c_str(), s);    
  }

  else if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);

    emit_jae((".POS" + std::to_string(posIdx)).c_str(), s);      
  }

  else {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);

    emit_jae((".POS" + std::to_string(posIdx)).c_str(), s); 
  }  

  emit_irmov("0", RAX, s);
  emit_jmp((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(posIdx)).c_str(), s);
  emit_irmov("1", RAX, s);
  emit_position((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  posIdx += 2;

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Gt_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  if(exprType1 == Int && exprType2 == Int) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_mrmov(RBP, offset2, RDX, s);
    emit_cmp(RDX, RAX, s);

    emit_jg((".POS" + std::to_string(posIdx)).c_str(), s);
  }

  else if(exprType1 == Int && exprType2 == Float) {
    emit_mrmov(RBP, offset1, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset2 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM1, XMM0, s);

    emit_ja((".POS" + std::to_string(posIdx)).c_str(), s);    
  }

  else if(exprType1 == Float && exprType2 == Int) {
    emit_mrmov(RBP, offset2, RAX, s);
    emit_int_to_float(RAX, XMM0, s);
    s << MOVSD << offset1 << "(" << RBP << ")" << COMMA << XMM1 << endl;
    emit_ucompisd(XMM0, XMM1, s);

    emit_ja((".POS" + std::to_string(posIdx)).c_str(), s);      
  }

  else {
    emit_mrmov(RBP, offset1, XMM1, s);
    emit_mrmov(RBP, offset2, XMM0, s);
    emit_ucompisd(XMM0, XMM1, s);

    emit_ja((".POS" + std::to_string(posIdx)).c_str(), s); 
  }  

  emit_irmov("0", RAX, s);
  emit_jmp((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  emit_position((".POS" + std::to_string(posIdx)).c_str(), s);
  emit_irmov("1", RAX, s);
  emit_position((".POS" + std::to_string(posIdx + 1)).c_str(), s);
  posIdx += 2;

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s); 
}


void And_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset1, RAX, s);
  emit_mrmov(RBP, offset2, RDX, s);
  emit_and(RAX, RDX, s);

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RDX, tv.varAddr, RBP, s); 
}


void Or_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset1, RAX, s);
  emit_mrmov(RBP, offset2, RDX, s);
  emit_or(RAX, RDX, s);

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RDX, tv.varAddr, RBP, s);  
}


void Xor_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);
  Symbol exprType1 = calcExprType(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);
  Symbol exprType2 = calcExprType(e2);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset1, RAX, s);
  emit_mrmov(RBP, offset2, RDX, s);
  emit_xor(RAX, RDX, s);

  TempVar tv;
  tv.varName = exprType1;
  tv.varType = exprType1;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RDX, tv.varAddr, RBP, s);  
}


void Not_class::code(ostream &s) {
  e1->code(s);
  int offset = calcExprAddr(e1);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset, RAX, s);
  emit_irmov("0x0000000000000001", RDX, s);
  emit_xor(RDX, RAX, s);

  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Bitnot_class::code(ostream &s) {
  e1->code(s);
  int offset = calcExprAddr(e1);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset, RAX, s);
  emit_not(RAX, s);

  TempVar tv;
  tv.varName = Int;
  tv.varType = Int;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Bitand_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset1, RAX, s);
  emit_mrmov(RBP, offset2, RDX, s);
  emit_and(RAX, RDX, s);

  TempVar tv;
  tv.varName = Int;
  tv.varType = Int;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RDX, tv.varAddr, RBP, s); 
}


void Bitor_class::code(ostream &s) {
  e1->code(s);
  int offset1 = calcExprAddr(e1);

  e2->code(s);
  int offset2 = calcExprAddr(e2);

  emit_sub("$8", RSP, s);

  emit_mrmov(RBP, offset1, RAX, s);
  emit_mrmov(RBP, offset2, RDX, s);
  emit_or(RAX, RDX, s);

  TempVar tv;
  tv.varName = Int;
  tv.varType = Int;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   
  emit_rmmov(RDX, tv.varAddr, RBP, s);  
}


void Const_int_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  TempVar tv;
  tv.varName = Int;
  tv.varType = Int;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   

  emit_irmov(value->get_string(), RAX, s);
  emit_rmmov(RAX, tv.varAddr, RBP, s); 
}


void Const_string_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  TempVar tv;
  tv.varName = String;
  tv.varType = String;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   

  emit_irmov((".LC" + std::to_string(getStrIdx(value))).c_str(), RAX, s);
  emit_rmmov(RAX, tv.varAddr, RBP, s); 
}


void Const_float_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  TempVar tv;
  tv.varName = Float;
  tv.varType = Float;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   

  char floatBytes[19];
  getFloatBytes(floatBytes, value);
  emit_irmov(floatBytes, RAX, s);
  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Const_bool_class::code(ostream &s) {
  emit_sub("$8", RSP, s);
  TempVar tv;
  tv.varName = Bool;
  tv.varType = Bool;
  tv.varAddr = -8 * (tempVarStack.size() + 1);
  tempVarStack.push(tv);   

  if(value) {
    emit_irmov("1", RAX, s);
  } else {
    emit_irmov("0", RAX, s);
  }

  emit_rmmov(RAX, tv.varAddr, RBP, s);  
}


void Object_class::code(ostream &s) {
 
}


void No_expr_class::code(ostream &s) {

}