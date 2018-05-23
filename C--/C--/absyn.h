//
//  absyn.h
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//

// All types and functions declared in this header file begin with "A_" Linked list types end with "..list"
// Type Definitions
typedef int A_pos;

typedef struct A_var_ *A_var;   // variable
typedef struct A_exp_ *A_exp;   // expression
typedef struct A_dec_ *A_dec;   // declaraction
typedef struct A_ty_ *A_ty;

typedef struct A_decList_ *A_decList;
typedef struct A_expList_ *A_expList;
typedef struct A_field_ *A_field;
typedef struct A_fieldList_ *A_fieldList;
typedef struct A_fundec_ *A_fundec;
typedef struct A_fundecList_ *A_fundecList;
typedef struct A_namety_ *A_namety;
typedef struct A_nametyList_ *A_nametyList;
typedef struct A_efield_ *A_efield;
typedef struct A_efieldList_ *A_efieldList;

typedef enum {A_plusOp, A_minusOp, A_timesOp, A_divideOp, A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp} A_oper;

// variable
struct A_var_ {
    enum {A_simpleVar, A_fieldVar, A_arrayVar, A_ptrVar} kind;
    A_pos pos;
    union {
        S_symbol simple;
        struct {
            A_var var;
            S_symbol sym;
        } field;
        struct {
            A_var var;
            A_exp exp;
        } array;
        struct {
            A_var var;
        } ptr;
    } u;
};

// expression
struct A_exp_ {
    enum {A_varExp, A_nilExp, A_intExp, A_floatExp, A_charExp, A_callExp, A_opExp, A_assignExp, A_ifExp, A_whileExp, A_forExp, A_breakExp, A_continueExp, A_returnExp, A_seqExp, A_structExp} kind;
    A_pos pos;
    union {
        A_var var;
        // nil; - needs only the pos
        int intt;
        float floatt;
        char charr;
        struct {
            S_symbol func;
            A_expList args;
        } call;
        struct {
            A_oper oper;
            A_exp left;
            A_exp right;
        } op;
        struct {
            A_var var;
            A_exp exp;
        } assign;
        struct {
            A_exp test, then, elsee;
        } iff; //elsee is optional
        struct {
            A_exp test, body;
        } whilee;
        struct {
            S_symbol var;
            A_exp lo, hi, body;
            bool escape;
        } forr;
        // breakk; - need only the pos
        // continue; - need only the pos
        A_exp rtn;  //return
        A_expList seq;  // seq exp
        struct {
            S_symbol stru;
            A_expList vars;
        } structt;
    } u;
};

// declaraction
struct A_dec_ {
    enum {A_functionDec, A_varDec, A_arrayDec} kind;
    A_pos pos;
    union {
//        A_fundecList function;
        struct {
            S_symbol func;
            A_expList params;
            S_symbol result;
            A_expList body;
        } funcc;
        // escape may change after the initial declaration
        struct {
            S_symbol var;
            S_symbol typ;
//            A_exp init;
            bool escape;
        } var;
        struct {
            S_symbol arr;
            S_symbol type;
            int size;
            bool escape;
        } arrayy;
    } u;
};

// types
struct A_ty_ {
//    enum {A_nameTy, A_recordTy, A_arrayTy} kind;
    A_pos pos;
    S_symbol id;
    S_symbol type;
};

// Linked lists and nodes of lists
struct A_field_ {
    S_symbol name, typ;
    A_pos pos;
    bool escape;
};
struct A_fieldList_ {
    A_field head;
    A_fieldList tail;
};
struct A_expList_ {
    A_exp head;
    A_expList tail;
};
struct A_fundec_ {
    A_pos pos;
    S_symbol name;
    A_fieldList params;
    S_symbol result;
    A_exp body;
};
struct A_fundecList_ {
    A_fundec head;
    A_fundecList tail;
};
struct A_decList_ {
    A_dec head;
    A_decList tail;
};
struct A_namety_ {
    S_symbol name;
    A_ty ty;
};
struct A_nametyList_ {
    A_namety head;
    A_nametyList tail;
};
struct A_efield_ {
    S_symbol name;
    A_exp exp;
};
struct A_efieldList_ {
    A_efield head;
    A_efieldList tail;
};


// Function Prototypes
A_var A_SimpleVar(A_pos pos, S_symbol sym);
A_var A_FieldVar(A_pos pos, A_var var, S_symbol sym);
A_var A_ArrayVar(A_pos pos, A_var var, A_exp exp);
A_var A_PtrVar(A_pos pos, A_var var);

A_exp A_VarExp(A_pos pos, A_var var);
A_exp A_NilExp(A_pos pos);
A_exp A_IntExp(A_pos pos, int i);
A_exp A_FloatExp(A_pos pos, float f);
A_exp A_CharExp(A_pos pos, char c);
A_exp A_CallExp(A_pos pos, S_symbol func, A_expList args);
A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right);
A_exp A_AssignExp(A_pos pos, A_var var, A_exp exp);
A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee);
A_exp A_WhileExp(A_pos pos, A_exp test, A_exp body);
A_exp A_ForExp(A_pos pos, S_symbol var, A_exp lo, A_exp hi, A_exp body);
A_exp A_BreakExp(A_pos pos);
A_exp A_ContinueExp(A_pos pos);
A_exp A_ReturnExp(A_pos pos, A_exp rtn);
A_exp A_SeqExp(A_pos pos, A_expList seq);
A_exp A_StructExp(A_pos pos, S_symbol stru, A_expList vars);

A_dec A_FunctionDec(A_pos pos, S_symbol func, A_expList params, S_symbol result, A_expList body);
A_dec A_VarDec(A_pos pos, S_symbol var, S_symbol typ, A_exp init);
A_dec A_ArrayDec(A_pos pos, S_symbol arr, S_symbol type, int size);

A_ty A_Ty(A_pos pos, S_symbol id, S_symbol type);

A_field A_Field(A_pos pos, S_symbol name, S_symbol typ);
A_fieldList A_FieldList(A_field head, A_fieldList tail);
A_expList A_ExpList(A_exp head, A_expList tail);
A_fundec A_Fundec(A_pos pos, S_symbol name, A_fieldList params, S_symbol result, A_exp body);
A_fundecList A_FundecList(A_fundec head, A_fundecList tail);
A_decList A_DecList(A_dec head, A_decList tail);
A_namety A_Namety(S_symbol name, A_ty ty);
A_nametyList A_NametyList(A_namety head, A_nametyList tail);
A_efield A_Efield(S_symbol name, A_exp exp);
A_efieldList A_EfieldList(A_efield head, A_efieldList tail);

