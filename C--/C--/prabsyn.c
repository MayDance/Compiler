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

// Print A_var types. Indent d spaces.
static void pr_var(FILE *out, A_var v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_simpleVar:
            fprintf(out, "simpleVar(%s)", S_name(v->u.simple));
            break;
        case A_fieldVar:
            fprintf(out, "%s\n", "fieldVar(");
            pr_var(out, v->u.field.var, d+1); fprintf(out, "%s\n", ",");
            indent(out, d+1); fprintf(out, "%s)", S_name(v->u.field.sym));
            break;
        case A_arrayVar:
            fprintf(out, "%s\n", "arrayVar(");
            pr_var(out, v->u.array.var, d+1); fprintf(out, "%s\n", ",");
            pr_exp(out, v->u.array.exp, d+1); fprintf(out, "%s", ")");
            break;
        case A_ptrVar:
            fprintf(out, "%s\n", "ptrVar(");
            pr_var(out, v->u.ptr.var, d+1); fprintf(out, "%s", ")");
            break;
        default:
            assert(0);
    }
}


static char str_oper[][12] = {
    "PLUS", "MINUS", "TIMES", "DIVIDE",
    "EQUAL", "NOTEQUAL", "LESSTHAN", "LESSEQ", "GREAT", "GREATEQ"};

static void pr_oper(FILE *out, A_oper d) {
    fprintf(out, "%s", str_oper[d]);
}

// Print A_var types. Indent d spaces.
void pr_exp(FILE *out, A_exp v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_varExp:
            fprintf(out, "varExp(\n"); pr_var(out, v->u.var, d+1);
            fprintf(out, "%s", ")");
            break;
        case A_nilExp:
            fprintf(out, "nilExp()");
            break;
        case A_intExp:
            fprintf(out, "intExp(%d)", v->u.intt);
            break;
        case A_floatExp:
            fprintf(out, "floatExp(%d)", v->u.intt);
            break;
        case A_callExp:
            fprintf(out, "callExp(%s,\n", S_name(v->u.call.func));
            pr_expList(out, v->u.call.args, d+1); fprintf(out, ")");
            break;
        case A_opExp:
            fprintf(out, "opExp(\n");
            indent(out, d+1); pr_oper(out, v->u.op.oper); fprintf(out, ",\n");
            pr_exp(out, v->u.op.left, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.op.right, d+1); fprintf(out, ")");
            break;
        case A_assignExp:
            fprintf(out, "assignExp(\n");
            pr_var(out, v->u.assign.var, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.assign.exp, d+1); fprintf(out, ")");
            break;
        case A_ifExp:
            fprintf(out, "iffExp(\n");
            pr_exp(out, v->u.iff.test, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.iff.then, d+1);
            if (v->u.iff.elsee) { /* else is optional */
                fprintf(out, ",\n");
                pr_exp(out, v->u.iff.elsee, d+1);
            }
            fprintf(out, ")");
            break;
        case A_whileExp:
            fprintf(out, "whileExp(\n");
            pr_exp(out, v->u.whilee.test, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.whilee.body, d+1); fprintf(out, ")\n");
            break;
        case A_forExp:
            fprintf(out, "forExp(%s,\n", S_name(v->u.forr.var));
            pr_exp(out, v->u.forr.lo, d+1); fprintf(out, ",\n");
            pr_exp(out, v->u.forr.hi, d+1); fprintf(out, "%s\n", ",");
            pr_exp(out, v->u.forr.body, d+1); fprintf(out, ",\n");
            indent(out, d+1); fprintf(out, "%s", v->u.forr.escape ? "TRUE)" : "FALSE)");
            break;
        case A_breakExp:
            fprintf(out, "breakExp()");
            break;
        case A_continueExp:
            fprintf(out, "continueExp()");
            break;
        case A_returnExp:
            fprintf(out, "returnExp(\n");
            pr_exp(out, v->u.rtn, d+1); fprintf(out, ")");
            break;
        case A_seqExp:
            fprintf(out, "seqExp(\n");
            pr_expList(out, v->u.seq, d+1); fprintf(out, ")");
            break;
        case A_structExp:
            fprintf(out, "structExp(%s,\n", S_name(v->u.structt.stru));
            pr_expList(out, v->u.structt.vars, d+1); fprintf(out, ")");
            break;
        default:
            assert(0);
    }
}

static void pr_dec(FILE *out, A_dec v, int d) {
    indent(out, d);
    switch (v->kind) {
        case A_functionDec:
            fprintf(out, "functionDec(%s\n", S_name(v->u.funcc.func));
            pr_expList(out, v->u.funcc.params, d+1); fprintf(out, ",\n");
            pr_expList(out, v->u.funcc.body, d+1); fprintf(out, ")");
            break;
        case A_varDec:
            fprintf(out, "varDec(%s,\n", S_name(v->u.var.var));
            if (v->u.var.typ) {
                indent(out, d+1); fprintf(out, "%s,\n", S_name(v->u.var.typ));
            }
            pr_exp(out, v->u.var.init, d+1); fprintf(out, ",\n");
            indent(out, d+1); fprintf(out, "%s", v->u.var.escape ? "TRUE)" : "FALSE)");
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
    fprintf(out, "aTy(%s)", S_name(v->type));
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
            fprintf(out, "globalDef(%s\n", S_name(v->u.globall.glob));
            pr_ty(out, v->u.globall.type, d+1); fprintf(out, ",\n");
            pr_varList(out, v->u.globall.varlist, d+1); fprintf(out, ")");
            break;
        case A_structDef:
            fprintf(out, "structDef(%s\n", S_name(v->u.structt.struc));
            pr_ty(out, v->u.structt.type, d+1); fprintf(out, ",\n");
            pr_stru(out, v->u.structt.stru, d+1); fprintf(out, ")");
            break;
        case A_functionDef:
            fprintf(out, "functionDef(%s\n", S_name(v->u.funcc.func));
            pr_ty(out, v->u.funcc.type, d+1); fprintf(out, ",\n");
            pr_var(out, v->u.funcc.returnval, d+1); fprintf(out, ")");
            pr_paramDec(out, v->u.funcc.param, d+1); fprintf(out, ")");
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
    fprintf(out, "stmt(%s)", S_name(v->stmt));
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

static void pr_stru(FILE *out, A_stru v, int d) {
    indent(out, d);
    fprintf(out, "stmt(%s,\n", S_name(v->struc));
    pr_ty(out, v->type, d+1); fprintf(out, ",\n");
    pr_decList(out, v->declist, d+1); fprintf(out, ",\n");
    fprintf(out, ")");
}

static void pr_paramDec(FILE *out, A_paramDec v, int d) {
    indent(out, d);
    fprintf(out, "paramDec(%s,\n", S_name(v->param));
    pr_ty(out, v->type, d+1); fprintf(out, ",\n");
    pr_var(out, v->var, d+1); fprintf(out, ",\n");
    fprintf(out, ")");
}
