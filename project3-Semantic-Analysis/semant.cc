#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

static ostream& error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;

std::vector<Decl> callDecl_list;
std::vector<int> callDecl_idx;
std::stack<bool> loopLevel;
std::stack<bool> blockLevel;
bool hasReturn;
bool hasCheckFormal;
CallDecl curr_call;
Variables param_list;

///////////////////////////////////////////////
// helper func
///////////////////////////////////////////////


static ostream& semant_error() {
    semant_errors++;
    return error_stream;
}

static ostream& semant_error(tree_node *t) {
    error_stream << t->get_line_number() << ": ";
    return semant_error();
}

static ostream& internal_error(int lineno) {
    error_stream << "FATAL:" << lineno << ": ";
    return error_stream;
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

static Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;

bool isValidCallName(Symbol type) {
    return type != (Symbol)print;
}

bool isValidTypeName(Symbol type) {
    return type != Void;
}

//
// Initializing the predefined symbols.
//

static void initialize_constants(void) {
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // Main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}

/*
    TODO :
    you should fill the following function defines, so that semant() can realize a semantic 
    analysis in a recursive way. 
    Of course, you can add any other functions to help.
*/


static bool sameType(Symbol name1, Symbol name2) {
    return strcmp(name1->get_string(), name2->get_string()) == 0;
}


static void install_calls(Decls decls) {
    bool existed;

    for(int i=decls->first(); decls->more(i); i=decls->next(i)) {
        if(decls->nth(i)->isCallDecl()) {
            existed = false;

            for(int j=0; j<callDecl_list.size(); j++) {
                if(strcmp(callDecl_list[j]->getName()->get_string(), decls->nth(i)->getName()->get_string()) == 0) {
                    existed = true;
                    break;             
                }
            }

            if(existed) {
                // ERROR: function multiply defined
                semant_error(decls->nth(i)) << "Function " << decls->nth(i)->getName()->get_string() << " was previously defined.\n";
            } else {
                callDecl_list.push_back(decls->nth(i));
                callDecl_idx.push_back(i);
            }       
        }
    }
}


static void install_globalVars(Decls decls) {
    for(int i=decls->first(); decls->more(i); i=decls->next(i)) {
        if(!decls->nth(i)->isCallDecl()) {
            Symbol gvar_name = decls->nth(i)->getName();
            Symbol gvar_type = decls->nth(i)->getType();

            if(objectEnv.probe(gvar_name)) {
                // ERROR: global variable multiply defined
                semant_error(decls->nth(i)) << "var " << gvar_name->get_string() << " was previously defined.\n";
            } else {
                if(sameType(gvar_type, Void)) {
                    // ERROR: Void type global variable
                    semant_error(decls->nth(i)) << "var " << gvar_name->get_string() << " cannot be of type Void. Void can just be used as return type.\n";
                } else {
                    objectEnv.addid(gvar_name, new Symbol(gvar_type));
                }
            }
        }
    }
}


static void check_calls(Decls decls) {
    bool hasMain = false;

    for(int i=0; i<callDecl_list.size(); i++) {
        curr_call = static_cast<CallDecl>(decls->nth(callDecl_idx[i]));

        // detect func "main"
        if(strcmp(curr_call->getName()->get_string(), "main") == 0) {
            hasMain = true;
            
            if(strcmp(curr_call->getType()->get_string(), "Void") != 0) {
                // ERROR: main function return non-Void            
                semant_error(curr_call) << "Main function should have return type Void.\n";
            }

            if(curr_call->getVariables()->len() != 0) {
                // ERROR: main function has parameters         
                semant_error(curr_call) << "Main function should not have any parameters.\n";
            }
        }

        // detect func "printf"
        if(strcmp(curr_call->getName()->get_string(), "printf") == 0) {
            // ERROR: printf function redefined
            semant_error(curr_call) << "Function printf cannot be redefination.\n";
            semant_error(curr_call) << "Function printf cannot have a name as printf.\n";
        }

        param_list = curr_call-> getVariables();
        for(int i=param_list->first(); param_list->more(i); i=param_list->next(i)) {
            if(sameType(param_list->nth(i)->getType(), Void)) {
                // ERROR: Void type parameter of function
                semant_error(curr_call) << "Function " << curr_call->getName()->get_string() << " \'s parameter has an invalid type Void.\n";
            }
        }

        hasReturn = false;
        hasCheckFormal = false;
        curr_call->getBody()->check(curr_call->getType());
        if(!hasReturn) {
            // ERROR: no return function
            semant_error(curr_call) << "Function " << curr_call->getName()->get_string() << " must have an overall return statement.\n";
        }
    }

    if(!hasMain) {
        // ERROR: no main function
        semant_error() << "Main function is not defined.\n";
    }

}


static void check_main() {

}


void VariableDecl_class::check() {
    if(sameType(getType(), Void)) {
        // ERROR: Void type variable
        semant_error(this) << "var " << getName()->get_string() << " cannot be of type Void. Void can just be used as return type.\n";
    } else if(objectEnv.probe(getName())){
        // ERROR: variable multiply defined
        semant_error(this) << "var " << getName()->get_string() << " was previously defined.\n";
    } else {
        objectEnv.addid(getName(), new Symbol(getType()));
    }   
}


void CallDecl_class::check() {

}


void StmtBlock_class::check(Symbol type) {
    objectEnv.enterscope();
    blockLevel.push(1);

    if(!hasCheckFormal) {
        hasCheckFormal = true;
        for(int i=param_list->first(); param_list->more(i); i=param_list->next(i)) {
            if(objectEnv.probe(param_list->nth(i)->getName())) {
                // ERROR: formal parameter multiply defined
                semant_error(curr_call) << "Function " << curr_call->getName()->get_string() << " \'s parameter has a duplicate name " << param_list->nth(i)->getName()->get_string() << ".\n";
            } else {
                objectEnv.addid(param_list->nth(i)->getName(), new Symbol(param_list->nth(i)->getType()));
            }
        }
    }

    for(int i=vars->first(); vars->more(i); i=vars->next(i)) {
        vars->nth(i)->check();
    }

    for(int i=stmts->first(); stmts->more(i); i=stmts->next(i)) {
        stmts->nth(i)->check(type);
    }

    blockLevel.pop();
    objectEnv.exitscope();
}


void IfStmt_class::check(Symbol type) {
    Symbol if_condition_type = getCondition()->checkType();
    if(!sameType(if_condition_type, Bool)) {
        // ERROR: non-Bool if-condition
        semant_error(this) << "Condition must be a Bool, got " << if_condition_type->get_string() << ".\n";
    }
    getThen()->check(type);
    getElse()->check(type);
}


void WhileStmt_class::check(Symbol type) {
    Symbol while_condition_type = getCondition()->checkType();
    if(!sameType(while_condition_type, Bool)) {
        // ERROR: non-Bool while-condition
        semant_error(this) << "Condition must be a Bool, got " << while_condition_type->get_string() << ".\n";
    }   

    loopLevel.push(1);
    getBody()->check(type);
    loopLevel.pop();
}


void ForStmt_class::check(Symbol type) {
    if(!getInit()->is_empty_Expr()) {
        getInit()->check(type);
    }

    if(!getCondition()->is_empty_Expr()) {
        Symbol for_condition_type = getCondition()->checkType();
        if(!sameType(for_condition_type, Bool)) {
            // ERROR: non-Bool for-condition
            semant_error(this) << "Condition must be a Bool, got " << for_condition_type->get_string() << ".\n";
        }
    }

    if(!getLoop()->is_empty_Expr()) {
        getLoop()->check(type);
    }

    loopLevel.push(1);
    getBody()->check(type);
    loopLevel.pop();
}


void ReturnStmt_class::check(Symbol type) {
    if(blockLevel.size() == 1) {
        hasReturn = true;
    }
    
    if(getValue()->is_empty_Expr()) {
        if(!sameType(type, Void)) {
            // ERROR: unmatched return type (return;)
            semant_error(this) << "Returns Void , but need " << type->get_string() << ".\n";
        }
    } else {
        Symbol return_type = getValue()->checkType();
        if(!sameType(type, return_type)) {
            // ERROR: unmatched return type (return ...;)
            semant_error(this) << "Returns " << return_type->get_string() << " , but need " << type->get_string() << ".\n";
        }
    }
}


void ContinueStmt_class::check(Symbol type) {
    if(loopLevel.empty()) {
        // ERROR: continue not in loop
        semant_error(this) << "continue must be used in a loop sentence.\n";
    }
}


void BreakStmt_class::check(Symbol type) {
    if(loopLevel.empty()) {
        // ERROR: break not in loop
        semant_error(this) << "break must be used in a loop sentence.\n";
    }
}


Symbol Call_class::checkType(){
    Actuals actual_list;

    // printf()
    if(strcmp(getName()->get_string(), "printf") == 0) { 
        actual_list = getActuals();
        if(actual_list->len() == 0) {
            // ERROR: printf() no argument        
            semant_error(this) << "printf() must have at least one parameter of type String.\n";
            setType(Void);
            return type;
        } else {
            if(!sameType(actual_list->nth(0)->checkType(), String)) {
                // ERROR: printf() 1st argument not String
                semant_error(this) << "printf()'s first parameter must be of type String.\n";    
                setType(Void);
                return type;      
            }

            for(int i=actual_list->first(); actual_list->more(i); i=actual_list->next(i)) {
                actual_list->nth(i)->checkType();
            }
        }
        setType(Void);
        return type;
    }

    // other functions
    CallDecl curr_func;
    for(int i=0; i<callDecl_list.size(); i++) {
        curr_func = static_cast<CallDecl>(callDecl_list[i]);
        if(strcmp(curr_func->getName()->get_string(), getName()->get_string()) == 0) {
            actual_list = getActuals();
            if(curr_func->getVariables()->len() != actual_list->len()) {
                // ERROR: wrong number of arguments
                semant_error(this) << "Function " << getName()->get_string() << " called with wrong number of arguments.\n";
                setType(curr_func->getType());
                return type;
            }
            for(int j=actual_list->first(); actual_list->more(j); j=actual_list->next(j)) {
                Symbol actual_type = actual_list->nth(j)->checkType();
                if(!sameType(curr_func->getVariables()->nth(j)->getType(), actual_type)) {
                    // ERROR: unmatched type of argument
                    semant_error(this) << "Function " << getName()->get_string() << ", the " << j + 1 << " parameter should be " << curr_func->getVariables()->nth(j)->getType()->get_string() << " but provided a " << actual_type->get_string() << ".\n";
                    setType(curr_func->getType());
                    return type;
                }
            }
            setType(curr_func->getType());
            return type;
        }
    }

    // ERROR: undefined function
    semant_error(this) << "Function " << getName()->get_string() << " has not been defined.\n";
    setType(Void);
    return type;
}


Symbol Actual_class::checkType(){
    setType(expr->checkType());
    return type;
}


Symbol Assign_class::checkType(){
    Symbol rtype = value->checkType();

    if(!objectEnv.lookup(lvalue)) {
        // ERROR: assign to undeclared variable
        semant_error(this) << "Left value " << lvalue << " has not been defined.\n";
        type = rtype;
        return type;
    }

    Symbol ltype = *objectEnv.lookup(lvalue);

    if(!sameType(ltype, rtype)) {
        // ERROR: unmatched ltype and rtype
        semant_error(this) << "Right value must have type " << ltype << " , got " << rtype << ".\n";
        type = ltype;
        return type;        
    }

    type = rtype;
    return type;
}


Symbol Add_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '+'
        semant_error(this) << "Cannot add a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    if(sameType(type1, Int) && sameType(type2, Int)) {
        setType(Int);
    } else {
        setType(Float);
    }
    return type;
}


Symbol Minus_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '-'
        semant_error(this) << "Cannot minus a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    if(sameType(type1, Int) && sameType(type2, Int)) {
        setType(Int);
    } else {
        setType(Float);
    }
    return type; 
}


Symbol Multi_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '*'
        semant_error(this) << "Cannot multi a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    if(sameType(type1, Int) && sameType(type2, Int)) {
        setType(Int);
    } else {
        setType(Float);
    }
    return type;
}


Symbol Divide_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '/'
        semant_error(this) << "Cannot div a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    if(sameType(type1, Int) && sameType(type2, Int)) {
        setType(Int);
    } else {
        setType(Float);
    }
    return type;
}


Symbol Mod_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if(!sameType(type1, Int) || !sameType(type2, Int)) {
        // ERROR: Non-Int Operand of '%'
        semant_error(this) << "Cannot mod a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Int);
    return type;
}


Symbol Neg_class::checkType(){
    Symbol type1 = e1->checkType();

    if(!sameType(type1, Int) && !sameType(type1, Float)) {
        // ERROR: Non-Int/Float Operand of '-'(neg)
        semant_error(this) << "A " << type1->get_string() << " doesn\'t have a negative.\n";
        setType(Void);
        return type;
    }

    if(sameType(type1, Int)) {
        setType(Int);
    } else {
        setType(Float);
    }
    return type;
}


Symbol Lt_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '<'
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol Le_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '<='
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol Equ_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float) && !sameType(type1, Bool)) || (!sameType(type2, Int) && !sameType(type2, Float) && !sameType(type2, Bool))){
        // ERROR: Non-Int/Float/Bool Operand of '=='
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    if((sameType(type1, Int) && sameType(type2, Bool)) || (sameType(type1, Bool) && sameType(type2, Int))) {
        // ERROR: cannot compare Int==Bool
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;        
    }

    if((sameType(type1, Float) && sameType(type2, Bool)) || (sameType(type1, Bool) && sameType(type2, Float))) {
        // ERROR: cannot compare Float==Bool
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;        
    }

    setType(Bool);
    return type;
}


Symbol Neq_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float) && !sameType(type1, Bool)) || (!sameType(type2, Int) && !sameType(type2, Float) && !sameType(type2, Bool))){
        // ERROR: Non-Int/Float/Bool Operand of '!='
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    if((sameType(type1, Int) && sameType(type2, Bool)) || (sameType(type1, Bool) && sameType(type2, Int))) {
        // ERROR: cannot compare Int==Bool
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;        
    }

    if((sameType(type1, Float) && sameType(type2, Bool)) || (sameType(type1, Bool) && sameType(type2, Float))) {
        // ERROR: cannot compare Float==Bool
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;        
    }

    setType(Bool);
    return type;
}


Symbol Ge_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '>='
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol Gt_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if((!sameType(type1, Int) && !sameType(type1, Float)) || (!sameType(type2, Int) && !sameType(type2, Float))){
        // ERROR: Non-Int/Float Operand of '>'
        semant_error(this) << "Cannot compare a " << type1->get_string() << " and a " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol And_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if(!sameType(type1, Bool) || !sameType(type2, Bool)) {
        // ERROR: Non-Bool Operand of '&&'
        semant_error(this) << "Cannot use && between " << type1->get_string() << " and " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol Or_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if(!sameType(type1, Bool) || !sameType(type2, Bool)) {
        // ERROR: Non-Bool Operand of '||'
        semant_error(this) << "Cannot use || between " << type1->get_string() << " and " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol Xor_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if(!sameType(type1, Int) || !sameType(type2, Int)) {
        // ERROR: Non-Int Operand of '^'
        semant_error(this) << "Cannot use ^ between " << type1->get_string() << " and " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Int);
    return type;
}


Symbol Not_class::checkType(){
    Symbol type1 = e1->checkType();

    if(!sameType(type1, Bool)) {
        // ERROR: Non-Bool Operand of '!'
        semant_error(this) << "Cannot use ! upon " << type1->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Bool);
    return type;
}


Symbol Bitand_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if(!sameType(type1, Int) || !sameType(type2, Int)) {
        // ERROR: Non-Int Operand of '&'
        semant_error(this) << "Cannot use & between " << type1->get_string() << " and " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Int);
    return type;
}


Symbol Bitor_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();

    if(!sameType(type1, Int) || !sameType(type2, Int)) {
        // ERROR: Non-Int Operand of '|'
        semant_error(this) << "Cannot use | between " << type1->get_string() << " and " << type2->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Int);
    return type;
}


Symbol Bitnot_class::checkType(){
    Symbol type1 = e1->checkType();

    if(!sameType(type1, Int)) {
        // ERROR: Non-Int Operand of '~'
        semant_error(this) << "Cannot use unary op ~ upon " << type1->get_string() << ".\n";
        setType(Void);
        return type;
    }

    setType(Int);
    return type;
}


Symbol Const_int_class::checkType(){
    setType(Int);
    return type;
}


Symbol Const_string_class::checkType(){
    setType(String);
    return type;
}


Symbol Const_float_class::checkType(){
    setType(Float);
    return type;
}


Symbol Const_bool_class::checkType(){
    setType(Bool);
    return type;
}


Symbol Object_class::checkType(){
    if(!objectEnv.lookup(var)) {
        // ERROR: undefined object
        semant_error(this) << "object " << var->get_string() << " has not been defined.\n";
        return Void;
    }
    Symbol object_type = *objectEnv.lookup(var);
    setType(object_type);
    return object_type;
}


Symbol No_expr_class::checkType(){
    setType(Void);
    return getType();
}


void Program_class::semant() {
    initialize_constants();
    install_calls(decls);
    // check_main();
    objectEnv.enterscope();
    install_globalVars(decls);
    check_calls(decls);
    objectEnv.exitscope();
    
    if (semant_errors > 0) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}



