//
//  absyn.c
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
 
// absyn.c - Abstract Syntax Functions. Most functions create an instance of an abstract syntax rule.

#include "absyn.h"  // abstract syntax data structures

A_var A_SimpleVar(A_pos pos, S_symbol idd) {
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_simpleVar;
    p->pos=pos;
    p->u.simplee=idd;
    return p;
}
A_var A_ArrayVar(A_pos pos, A_var var, int size) {
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_arrayVar;
    p->pos=pos;
    p->u.arrayy.simple=var;
    p->u.arrayy.size=size;
    return p;
}

A_exp A_FuncExp(A_pos pos, S_symbol idd, A_expList args) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_funcExp;
    p->pos=pos;
    p->u.funcc.idd=idd;
    p->u.funcc.args=args;
    return p;
}
A_exp A_StruExp(A_pos pos, A_exp exp, S_symbol stru) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_struExp;
    p->pos=pos;
    p->u.struu.exp=exp;
    p->u.struu.stru=stru;
    return p;
}
A_exp A_SimpleExp(A_pos pos, S_symbol idd) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_simpleExp;
    p->pos=pos;
    p->u.simple.idd=idd;
    return p;
}
A_exp A_IntExp(A_pos pos, int i) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_intExp;
    p->pos=pos;
    p->u.i=i;
    return p;
}
A_exp A_FloatExp(A_pos pos, float f) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_floatExp;
    p->pos=pos;
    p->u.f=f;
    return p;
}
A_exp A_CharExp(A_pos pos, char c) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_intExp;
    p->pos=pos;
    p->u.c=c;
    return p;
}
A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_opExp;
    p->pos=pos;
    p->u.opp.oper=oper;
    p->u.opp.left=left;
    p->u.opp.right=right;
    return p;
}
A_exp A_AssignExp(A_pos pos, A_exp left, A_exp right) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_assignExp;
    p->pos=pos;
    p->u.assignn.left=left;
    p->u.assignn.right=right;
    return p;
}
A_exp A_NotExp(A_pos pos, A_exp exp) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_notExp;
    p->pos=pos;
    p->u.nott.exp=exp;
    return p;
}
A_exp A_ArrayExp(A_pos pos, A_exp exp1, A_exp exp2) {
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_arrayExp;
    p->pos=pos;
    p->u.arrayy.exp1=exp1;
    p->u.arrayy.exp2=exp2;
    return p;
}

A_dec A_FuncDec(A_pos pos, S_symbol func, A_paramList body) {
    A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_funcDec;
    p->pos=pos;
    p->u.funcc.func=func;
    p->u.funcc.body=body;
    return p;
}
A_dec A_VarDec(A_pos pos, A_var var, A_exp init) {
    A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_varDec;
    p->pos=pos;
    p->u.varr.var=var;
    p->u.varr.init=init;
    p->u.varr.escape=TRUE;
    return p;
}

A_ty A_SimpleTy(A_pos pos, A_type type) {
    A_ty p = checked_malloc(sizeof(*p));
    p->kind=A_simpleTy;
    p->pos=pos;
    p->u.simplee.type=type;
    return p;
}
A_ty A_StruTy(A_pos pos, S_symbol tag, A_defList deflist) {
    A_ty p = checked_malloc(sizeof(*p));
    p->kind=A_struTy;
    p->u.structt.tag=tag;
    p->u.structt.deflist=deflist;
    return p;
}

A_expList A_ExpList(A_exp head, A_expList tail) {
    A_expList p = checked_malloc(sizeof(*p));
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
A_def A_GlobalDef(A_pos pos, A_ty type, A_decList declist) {
    A_def p = checked_malloc(sizeof(*p));
    p->kind=A_globalDef;
    p->pos=pos;
    p->u.globall.type=type;
    p->u.globall.declist=declist;
    return p;
}
A_def A_StructDef(A_pos pos, A_ty type) {
    A_def p = checked_malloc(sizeof(*p));
    p->kind=A_structDef;
    p->pos=pos;
    p->u.structt.type=type;
    return p;
}
A_def A_FuncDef(A_pos pos, A_ty type, A_dec funcdec, A_compStmt compst) {
    A_def p = checked_malloc(sizeof(*p));
    p->kind=A_funcDec;
    p->pos=pos;
    p->u.funcc.type=type;
    p->u.funcc.funcdec=funcdec;
    p->u.funcc.compst=compst;
    return p;
}
A_def A_LocalDef(A_pos pos, A_ty dec, A_decList declist) {
    A_def p = checked_malloc(sizeof(*p));
    p->kind=A_localDef;
    p->pos=pos;
    p->u.loacll.dec=dec;
    p->u.loacll.declist=declist;
    return p;
}
A_defList A_DefList(A_def head, A_defList tail) {
    A_defList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_stmt A_ExpStmt(A_pos pos, A_exp exp) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_expStmt;
    p->pos=pos;
    p->u.expp.exp=exp;
    return p;
}
A_stmt A_RetnStmt(A_pos pos, A_exp exp) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_retnStmt;
    p->pos=pos;
    p->u.retn.exp=exp;
    return p;
}
A_stmt A_IfStmt(A_pos pos, A_exp condition, A_stmt iff, A_stmt elsee) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_ifStmt;
    p->pos=pos;
    p->u.iff.condition=condition;
    p->u.iff.iff=iff;
    p->u.iff.elsee=elsee;
    return p;
}
A_stmt A_WhileStmt(A_pos pos, A_exp condition, A_stmt stmt) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_whileStmt;
    p->pos=pos;
    p->u.whilee.condition=condition;
    p->u.whilee.stmt=stmt;
    return p;
}
A_stmt A_BreakStmt(A_pos pos) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_breakStmt;
    p->pos=pos;
    return p;
}
A_stmt A_ContinueStmt(A_pos pos) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_continueStmt;
    p->pos=pos;
    return p;
}
A_stmt A_ComppStmt(A_pos pos, A_compStmt comp) {
    A_stmt p = checked_malloc(sizeof(*p));
    p->kind=A_comppStmt;
    p->pos=pos;
    p->u.compp.comp=comp;
    return p;
}

A_stmtList A_StmtList(A_stmt head, A_stmtList tail) {
    A_stmtList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_compStmt A_CompStmt(A_pos pos, A_decList declist, A_stmtList stmtlist) {
    A_compStmt p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->declist=declist;
    p->stmtlist=stmtlist;
    return p;
}

A_paramDec A_ParamDec(A_pos pos, A_ty type, A_var var) {
    A_paramDec p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->type=type;
    p->var=var;
    return p;
}

A_paramList A_ParamList(A_paramDec head, A_paramList tail) {
    A_paramList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}
