//
//  absyn.c
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//

// absyn.c - Abstract Syntax Functions. Most functions create an instance of an abstract syntax rule.
#include "util.h"
#include "symbol.h" /* symbol table data structures */
#include "absyn.h"  /* abstract syntax data structures */

A_var A_SimpleVar(A_pos pos, S_symbol sym) {
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_simpleVar;
    p->pos=pos;
    p->u.simple=sym;
    return p;
}
A_var A_FieldVar(A_pos pos, A_var var, S_symbol sym) {
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_fieldVar;
    p->pos=pos;
    p->u.field.var=var;
    p->u.field.sym=sym;
    return p;
}
A_var A_ArrayVar(A_pos pos, A_var var, A_exp exp) {
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_arrayVar;
    p->pos=pos;
    p->u.array.var=var;
    p->u.array.exp=exp;
    return p;
}
A_var A_PtrVar(A_pos pos, A_var var) {
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_ptrVar;
    p->pos=pos;
    p->u.ptr.var=var;
    return p;
}

A_exp A_VarExp(A_pos pos, A_var var) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_varExp;
    p->pos=pos;
    p->u.var=var;
    return p;
}
A_exp A_NilExp(A_pos pos) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_nilExp;
    p->pos=pos;
    return p;
}
A_exp A_IntExp(A_pos pos, int i) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_intExp;
    p->pos=pos;
    p->u.intt=i;
    return p;
}
A_exp A_FloatExp(A_pos pos, float f) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_floatExp;
    p->pos=pos;
    p->u.floatt=f;
    return p;
}
A_exp A_CharExp(A_pos pos, char c) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_charExp;
    p->pos=pos;
    p->u.charr=c;
    return p;
}
A_exp A_CallExp(A_pos pos, S_symbol func, A_expList args) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_callExp;
    p->pos=pos;
    p->u.call.func=func;
    p->u.call.args=args;
    return p;
}
A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_opExp;
    p->pos=pos;
    p->u.op.oper=oper;
    p->u.op.left=left;
    p->u.op.right=right;
    return p;
}
A_exp A_AssignExp(A_pos pos, A_var var, A_exp exp)
{A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_assignExp;
    p->pos=pos;
    p->u.assign.var=var;
    p->u.assign.exp=exp;
    return p;
}
A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_ifExp;
    p->pos=pos;
    p->u.iff.test=test;
    p->u.iff.then=then;
    p->u.iff.elsee=elsee;
    return p;
}
A_exp A_WhileExp(A_pos pos, A_exp test, A_exp body) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_whileExp;
    p->pos=pos;
    p->u.whilee.test=test;
    p->u.whilee.body=body;
    return p;
}
A_exp A_ForExp(A_pos pos, S_symbol var, A_exp lo, A_exp hi, A_exp body) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_forExp;
    p->pos=pos;
    p->u.forr.var=var;
    p->u.forr.lo=lo;
    p->u.forr.hi=hi;
    p->u.forr.body=body;
    p->u.forr.escape=TRUE;
    return p;
}
A_exp A_BreakExp(A_pos pos) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_breakExp;
    p->pos=pos;
    return p;
}
A_exp A_ContinueExp(A_pos pos) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_continueExp;
    p->pos=pos;
    return p;
}
A_exp A_SeqExp(A_pos pos, A_expList seq) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_seqExp;
    p->pos=pos;
    p->u.seq=seq;
    return p;
}
A_exp A_StructExp(A_pos pos, S_symbol stru, A_expList vars) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_structExp;
    p->pos=pos;
    p->u.structt.stru=stru;
    p->u.structt.vars=vars;
    return p;
}

A_dec A_FunctionDec(A_pos pos, S_symbol func, A_expList params, S_symbol result, A_expList body) {
    A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_functionDec;
    p->pos=pos;
    p->u.funcc.func=func;
    p->u.funcc.params=params;
    p->u.funcc.result=result;
    p->u.funcc.body=body;
    return p;
}
A_dec A_VarDec(A_pos pos, S_symbol var, S_symbol typ, A_exp init)
{A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_varDec;
    p->pos=pos;
    p->u.var.var=var;
    p->u.var.typ=typ;
//    p->u.var.init=init;
    p->u.var.escape=TRUE;
    return p;
}
A_dec A_ArrayDec(A_pos pos, S_symbol arr, S_symbol type, int size)
{A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_arrayDec;
    p->pos=pos;
    p->u.arrayy.arr=arr;
    p->u.arrayy.type=type;
    p->u.arrayy.size=size;
    p->u.arrayy.escape=TRUE;
    return p;
}

A_ty A_Ty(A_pos pos, S_symbol id, S_symbol type) {
    A_ty p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->id=id;
    p->type=type;
    return p;
}

A_field A_Field(A_pos pos, S_symbol name, S_symbol typ) {
    A_field p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->name=name;
    p->typ=typ;
    p->escape=TRUE;
    return p;
}
A_fieldList A_FieldList(A_field head, A_fieldList tail) {
    A_fieldList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}
A_expList A_ExpList(A_exp head, A_expList tail) {
    A_expList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}
A_fundec A_Fundec(A_pos pos, S_symbol name, A_fieldList params, S_symbol result, A_exp body) {
    A_fundec p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->name=name;
    p->params=params;
    p->result=result;
    p->body=body;
    return p;
}
A_fundecList A_FundecList(A_fundec head, A_fundecList tail) {
    A_fundecList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}
A_decList A_DecList(A_dec head, A_decList tail) {
    A_decList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}
A_namety A_Namety(S_symbol name, A_ty ty) {
    A_namety p = checked_malloc(sizeof(*p));
    p->name=name;
    p->ty=ty;
    return p;
}
A_nametyList A_NametyList(A_namety head, A_nametyList tail) {
    A_nametyList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}
A_efield A_Efield(S_symbol name, A_exp exp) {
    A_efield p = checked_malloc(sizeof(*p));
    p->name=name;
    p->exp=exp;
    return p;
}
A_efieldList A_EfieldList(A_efield head, A_efieldList tail) {
    A_efieldList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

