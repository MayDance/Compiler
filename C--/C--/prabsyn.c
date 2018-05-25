//
//  prabsyn.c
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//

// prabsyn.c - Print Abstract Syntax data structures. Most functions handle an instance of an abstract syntax rule.
#include <stdio.h>
#include "util.h"
#include "symbol.h" /* symbol table data structures */
#include "absyn.h"  /* abstract syntax data structures */
#include "prabsyn.h" /* function prototype */

// local function prototypes 
static void pr_var(FILE *out, A_var v, int d);
static void pr_dec(FILE *out, A_dec v, int d);
static void pr_ty(FILE *out, A_ty v, int d);
static void pr_varList(FILE *out, A_varList v, int d);
static void pr_expList(FILE *out, A_expList v, int d);
static void pr_decList(FILE *out, A_decList v, int d);
static void pr_def(FILE *out, A_def v, int d);
static void pr_defList(FILE *out, A_defList v, int d);
static void pr_stmt(FILE *out, A_stmt v, int d);
static void pr_stmtList(FILE *out, A_stmtList v, int d);
static void pr_stru(FILE *out, A_stru v, int d);
static void pr_paramDec(FILE *out, A_paramDec v, int d);


static void indent(FILE *out, int d) {
    int i;
    for (i = 0; i <= d; i++)
        fprintf(out, " ");
}

static char str_oper[][12] = {"AND", "OR", "PLUS", "MINUS", "TIMES", "DIVIDE"};
static char str_type[][12] = {"int", "float", "char"};

static void pr_oper(FILE *out, A_oper d) {
    fprintf(out, "%s", str_oper[d]);
}
static void pr_type(FILE *out, A_type t) {
    fprintf(out, "%s", str_oper[t]);
}

// Print A_var types. Indent d spaces.
void pr_exp(FILE *out, A_exp v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_funExp:
            fprintf(out, "funcExp(%s,\n", S_name(v->u.funcc.idd));
            pr_expList(out, v->u.funcc.args, d+1); fprintf(out, ")");
            break;
        case A_struExp:
            fprintf(out, "struExp(%s,\n", S_name(v->u.struu.stru));
            pr_exp(out, v->u.struu.exp, d+1); fprintf(out, ")");
            break;
        case A_simpleExp:
            fprintf(out, "simpleExp(%s)", S_name(v->u.simple.idd));
        case A_intExp:
            fprintf(out, "intExp(%d)", v->u.i);
            break;
        case A_floatExp:
            fprintf(out, "floatExp(%f)", v->u.f);
            break;
        case A_charExp:
            fprintf(out, "charExp(%c)", v->u.c);
            break;
        case A_opExp:
            fprintf(out, "opExp(\n");
            indent(out, d+1); pr_oper(out, v->u.opp.oper); fprintf(out, ",\n");
            pr_exp(out, v->u.opp.left, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.opp.right, d+1); fprintf(out, ")");
            break;
        case A_assignExp:
            fprintf(out, "assignExp(\n");
            pr_exp(out, v->u.assignn.left, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.assignn.right, d+1); fprintf(out, ")");
            break;
        case A_notExp:
            fprintf(out, "notExp(\n"); pr_exp(out, v->u.nott.exp, d+1);
            fprintf(out, "%s", ")");
            break;
        default:
            assert(0);
    }
}

static void pr_dec(FILE *out, A_dec v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_funcDec:
            fprintf(out, "functionDec(%s\n", S_name(v->u.funcc.func));
            pr_varList(out, v->u.funcc.body, d+1); fprintf(out, ")");
            break;
        case A_varDec:
            fprintf(out, "varDec(%s,\n", S_name(v->u.varr.var));
            pr_exp(out, v->u.varr.init, d+1); fprintf(out, ",\n");
            indent(out, d+1); fprintf(out, "%s", v->u.varr.escape ? "TRUE)" : "FALSE)");
            break;
        case A_arrayDec:
            fprintf(out, "arrayDec(%s\n", S_name(v->u.arrayy.arr));
            indent(out, v->u.arrayy.size); fprintf(out, "%s", v->u.arrayy.escape ? "TRUE)" : "FALSE)");
            break;
        default:
            assert(0);
    }
}

static void pr_ty(FILE *out, A_ty v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_simpleTy:
            fprintf(out, "simpleTy(\n");
            pr_type(out, v->u.varr.type);  fprintf(out, ")");
            break;
        case A_struTy:
            fprintf(out, "struTy(%s\n", S_name(v->u.structt.tag));
            pr_defList(out, v->u.structt.deflist, d+1); fprintf(out, ")");
            break;
        default:
            assert(0);
    }
}

static void pr_varList(FILE *out, A_varList v, int d) {
    indent(out, d);
    if (v) {
        fprintf(out, "varList(\n");
        pr_var(out, v->head, d+1); fprintf(out, ",\n");
        pr_varList(out, v->tail, d+1); fprintf(out, ")");
    }
    else fprintf(out, "fieldList()");
}

static void pr_expList(FILE *out, A_expList v, int d) {
    indent(out, d);
    if (v) {
        fprintf(out, "expList(\n");
        pr_exp(out, v->head, d+1); fprintf(out, ",\n");
        pr_expList(out, v->tail, d+1);
        fprintf(out, ")");
    }
    else fprintf(out, "expList()");
    
}

static void pr_decList(FILE *out, A_decList v, int d) {
    indent(out, d);
    if (v) {
        fprintf(out, "decList(\n");
        pr_dec(out, v->head, d+1); fprintf(out, ",\n");
        pr_decList(out, v->tail, d+1);
        fprintf(out, ")");
    }
    else fprintf(out, "decList()");
    
}

static void pr_def(FILE *out, A_def v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_globalDef:
            fprintf(out, "globalDef(\n");
            pr_ty(out, v->u.globall.type, d+1); fprintf(out, ",\n");
            pr_decList(out, v->u.globall.declist, d+1); fprintf(out, ")");
            break;
        case A_structDef:
            fprintf(out, "structDef(\n");
            pr_ty(out, v->u.structt.type, d+1); fprintf(out, ",\n");
            break;
        case A_funcDef:
            fprintf(out, "funcDef(\n");
            pr_ty(out, v->u.funcc.type, d+1); fprintf(out, ",\n");
            pr_dec(out, v->u.funcc.funcdec, d+1); fprintf(out, ",\n");
            pr_stmt(out, v->u.funcc.compst, d+1); fprintf(out, ")");
            break;
        default:
            assert(0);
    }
}

static void pr_defList(FILE *out, A_defList v, int d) {
    indent(out, d);
    if (v) {
        fprintf(out, "defList(\n");
        pr_def(out, v->head, d+1); fprintf(out, ",\n");
        pr_defList(out, v->tail, d+1);
        fprintf(out, ")");
    }
    else fprintf(out, "decList()");
}

static void pr_stmt(FILE *out, A_stmt v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_expStmt:
            fprintf(out, "expStmt(\n");
            pr_exp(out, v->u.expp.exp, d+1); fprintf(out, ",\n");
            break;
        case A_retnStmt:
            fprintf(out, "retnStmt(\n");
            pr_exp(out, v->u.retn.exp, d+1); fprintf(out, ",\n");
            break;
        case A_ifStmt:
            fprintf(out, "ifStmt(\n");
            pr_exp(out, v->u.iff.condition, d+1); fprintf(out, ",\n");
            pr_stmt(out, v->u.iff.iff, d+1); fprintf(out, ",\n");
            pr_stmt(out, v->u.iff.elsee, d+1); fprintf(out, ")");
            break;
        case A_breakStmt:
            fprintf(out, "breakStmt()");
        case A_continueStmt:
            fprintf(out, "continueStmt()");
        case A_compStmt:
            fprintf(out, "ifStmt(\n");
            pr_decList(out, v->u.compst.declist, d+1); fprintf(out, ",\n");
            pr_stmtList(out, v->u.compst.stmtlist, d+1); fprintf(out, ",\n");
            break;
        default:
            assert(0);
    }
}

static void pr_stmtList(FILE *out, A_stmtList v, int d) {
    indent(out, d);
    if (v) {
        fprintf(out, "stmtList(\n");
        pr_stmt(out, v->head, d+1); fprintf(out, ",\n");
        pr_stmtList(out, v->tail, d+1);
        fprintf(out, ")");
    }
    else fprintf(out, "decList()");
}

static void pr_paramDec(FILE *out, A_paramDec v, int d) {
    indent(out, d);
    fprintf(out, "paramDec(,\n");
    pr_ty(out, v->type, d+1); fprintf(out, ",\n");
    pr_dec(out, v->dec, d+1); fprintf(out, ")");
}
