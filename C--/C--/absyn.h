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
typedef struct A_ty_ *A_ty; // type

typedef struct A_varList_ *A_varList;
typedef struct A_expList_ *A_expList;
typedef struct A_decList_ *A_decList;
typedef struct A_def_ *A_def;   // definition
typedef struct A_defList_ *A_defList;
typedef struct A_stmt_ *A_stmt; //statement
typedef struct A_stmtList_ *A_stmtList;
typedef struct A_stru_ *A_stru;
typedef struct A_paramDec_ *A_paramDec;

typedef enum {A_andOP, A_orOP, A_plusOp, A_minusOp, A_timesOp, A_divideOp} A_oper;
typedef enum {INT, FLOAT, CHAR} A_type;


// expression
struct A_exp_ {
    enum {A_funExp, A_struExp, A_simpleExp, A_intExp, A_floatExp, A_charExp, A_opExp, A_assignExp, A_notExp} kind;
    A_pos pos;
    union {
        struct {
            S_symbol idd;
            A_expList args;
        } funcc;
        struct {
            A_exp exp;
            S_symbol stru;
        } struu;
        struct {
            S_symbol idd;
        } simple;
        int i;
        float f;
        char c;
        struct {
            A_oper oper;
            A_exp left, right;
        } opp;
        struct {
            A_exp left, right;
        } assignn;
        struct {
            A_exp exp;
        } nott;
    } u;
};

// declaraction
struct A_dec_ {
    enum {A_funcDec, A_varDec, A_arrayDec} kind;
    A_pos pos;
    union {
        struct {
            S_symbol func;
            A_varList body;
        } funcc;
        // escape may change after the initial declaration
        struct {
            S_symbol var;
            A_exp init;
            bool escape;
        } varr;
        struct {
            S_symbol arr;
            int size;
            bool escape;
        } arrayy;
    } u;
};

// types
struct A_ty_ {
    enum {A_simpleTy, A_struTy} kind;
    A_pos pos;
    union {
        struct {
            A_type type;
        } varr;
        struct {
            S_symbol tag;
            A_defList deflist;
        } structt;
    } u;
};

// Linked lists and nodes of lists
struct A_varList_ {
    A_var head;
    A_varList tail;
};
struct A_expList_ {
    A_exp head;
    A_expList tail;
};
struct A_decList_ {
    A_dec head;
    A_decList tail;
};

struct A_def_ {
    enum {A_globalDef, A_structDef, A_funcDef, A_loacalDef} kind;
    A_pos pos;
    union {
        struct {
            A_ty type;
            A_decList declist;
        } globall;
        struct {
            A_ty type;
        } structt;
        struct {
            A_ty type;
            A_dec funcdec;
            A_stmt compst;
        } funcc;
        struct {
            S_symbol dec;
            A_decList declist;
        } loacll;
    } u;
};

struct A_defList_ {
    A_def head;
    A_defList tail;
};

struct A_stmt_ {
    enum {A_expStmt, A_retnStmt, A_ifStmt, A_whileStmt, A_breakStmt, A_continueStmt, A_compStmt} kind;
    A_pos pos;
    union {
        struct {
            A_exp exp;
        } expp;
        struct {
            A_exp exp;
        }retn;
        struct {
            A_exp condition;
            A_stmt iff, elsee;
        }iff;
        struct {
            A_exp condition;
            A_stmt stmt;
        }whilee;
        // break
        // continue
        struct {
            A_decList declist;
            A_stmtList stmtlist;
        }compst;
    } u;
};
struct A_stmtList_ {
    A_stmt head;
    A_stmtList tail;
};

struct A_paramDec_ {
    A_pos pos;
    A_ty type;
    A_dec dec;
};

// Function Prototypes
A_exp A_FuncExp(A_pos pos, S_symbol idd, A_expList args);
A_exp A_StruExp(A_pos pos, A_exp exp, S_symbol stru);
A_exp A_SimpleExp(A_pos pos, S_symbol idd);
A_exp A_IntExp(A_pos pos, int i);
A_exp A_FloatExp(A_pos pos, float f);
A_exp A_CharExp(A_pos pos, char c);
A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right);
A_exp A_AssignExp(A_pos pos, A_exp left, A_exp right);
A_exp A_NotExp(A_pos pos, A_exp exp);

A_dec A_FuncDec(A_pos pos, S_symbol func, A_varList body);
A_dec A_VarDec(A_pos pos, S_symbol var, A_exp init);
A_dec A_ArrayDec(A_pos pos, S_symbol arr, int size);

A_ty A_SimpleTy(A_pos pos, A_type type);
A_ty A_StruTy(A_pos pos, S_symbol tag, A_defList deflist);

A_varList A_VarList(A_var head, A_varList tail);
A_expList A_ExpList(A_exp head, A_expList tail);
A_decList A_DecList(A_dec head, A_decList tail);

A_def A_GlobalDef(A_pos pos, A_ty type, A_decList declist);
A_def A_StructDef(A_pos pos, A_ty type);
A_def A_FuncDef(A_pos pos, A_ty type, A_dec funcdec, A_stmt compst);
A_def A_LocalDef(A_pos pos, S_symbol dec, A_decList declist);

A_defList A_DefList(A_def head, A_defList tail);

A_stmt ExpStmt(A_pos pos, A_exp exp);
A_stmt A_RetnStmt(A_pos pos, A_exp exp);
A_stmt A_IfStmt(A_pos pos, A_exp condition, A_stmt iff, A_stmt elsee);
A_stmt A_WhileStmt(A_pos pos, A_exp condition, A_stmt stmt);
A_stmt A_BreakStmt(A_pos pos);
A_stmt A_ContinueStmt(A_pos pos);
A_stmt A_CompStmt(A_pos pos, A_decList declist, A_stmtList stmtlist);

A_stmtList A_StmtList(A_stmt head, A_stmtList tail);

A_paramDec A_ParamDec(A_pos pos, A_ty type, A_dec dec);
