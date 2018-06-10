/*
 Author:  Jing Yao
 Reference: Chpt.5.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "absyn.h"
#include "types.h"
#include "temp.h"
#include "tree.h"
#include "translate.h"
#include "env.h"
#include "errormsg.h"
#include "semant.h"
#include "symbol.h"

/*-----------------------------------------------------------------------------------------------------*/
/*constructor for expTy*/
/*Reference: Textbook Page 82*/
S_table globalVenv;
S_table funcFenv;
S_table typeTenv;
varty varTy(A_var v, A_ty t)
{
    varty p = checked_malloc(sizeof(*p));
    p->v = v;
    p->t = t;
    return p;
}
functy funcTy(A_dec f, A_ty t)
{
    functy p = checked_malloc(sizeof(*p));
    p->f = f;
    p->t = t;
    return p;
}
/*-----------------------------------------------------------------------------------------------------*/
static void startCheck(A_defList v);
static void checkVar(S_table venv, A_var v);
static A_ty checkExp(S_table venv, A_exp v);
static void checkDec(S_table venv, A_dec v, A_ty t);
static void checkDecVar(S_table venv, A_var v, A_ty t);
static void checkTy(S_table venv, A_ty v);
static void checkFuncDec(S_table venv, A_dec v);
static void checkExpList(S_table venv, A_expList v);
static void checkDecList(S_table venv, A_decList v, A_ty t);
static void checkDefList(S_table venv, A_defList v);
static void checkDef(S_table venv, A_def v);
static void checkStmt(S_table venv, A_stmt v);
static void checkStmtList(S_table venv, A_stmtList v);
static void checkCompStmt(S_table venv, A_compStmt v);
static void checkParamList(S_table venv, A_paramList v);


/*-----------------------------------------------------------------------------------------------------*/
/*Functions Within the module*/

/*return real type in Ty_ty in case of Ty_name*/
/*Reference Textbook Page 83*/
int equalType(A_ty t1, A_ty t2)
{
    if(t1->kind != t2->kind)
        return 0;
    switch(t1->kind){
        case A_simpleTy:
            if (t1->u.simplee.type == A_CHAR && t2->u.simplee.type != A_CHAR)
                return 0;
            else if (t2->u.simplee.type == A_CHAR && t1->u.simplee.type != A_CHAR)
                return 0;
            break;
        case A_struTy:
            if(t1->u.structt.tag != t2->u.structt.tag)
                return 0;
            break;
        default:
            return 0;
            break;
    }
    return 1;
}


static void startCheck(A_defList v){
    globalVenv = E_base_venv();
    typeTenv = E_base_tenv();
    funcFenv = S_empty();
    checkDefList(globalVenv, v);
}


static void checkVar(S_table venv, A_var v)
{
    switch (v->kind)
    {
        case A_simpleVar:
            varty simpleVar = S_look(venv, v->u.simplee);
            /*Not found*/
            if (!simpleVar)
            {
                simpleVar = S_look(globalVenv, v->u.simplee);
                if(!simpleVar)
                    EM_error(v->pos, "Undefined variable: \'%s\'.", S_name(v->u.simplee));
                break;
            }
            
            break;
        case A_arrayVar:
            varty arrayVar = S_look(venv, v->u.arrayy.simple);
            if(!arrayVar)
            {
                arrayVar = S_look(globalVenv, v->u.simplee);
                if (!arrayVar)
                    EM_error(v->pos, "Undefined variable: \'%s\'.", S_name(v->u.arrayy.simple));
            }
            else{
                if(arrayVar->v->u.arrayy.size <= v->u.arrayy.size)
                    EM_error(v->pos, "Segment error: \'%s\'.", S_name(v->u.arrayy.simple));
                else{
                    
                }
            }
            
            break;
    }
}

static void checkDecVar(S_table venv, A_var v, A_ty t)
{
    switch (v->kind)
    {
        case A_simpleVar:
            
            varty simpleVar = S_look(venv, v->u.simplee);
            /*Not found*/
            if (simpleVar)
            {
                EM_error(v->pos, "redefined variable: \'%s\'.", S_name(v->u.simplee));
                break;
            }
            else
            {
                S_enter(venv, v->u.simplee, varTy(v, t));
                //
            }
            break;
        case A_arrayVar:
            varty arrayVar = S_look(venv, v->u.arrayy.simple);
            if (arrayVar)
            {
                EM_error(v->pos, "redefined variable: \'%s\'.", S_name(v->u.arrayy.simple));
            }
            else
            {
                S_enter(venv, v->u.arrayy.simple->u.simplee, varTy(v, t));
                //
            }
            
            break;
    }
}

static A_ty checkExp(S_table venv, A_exp v)
{
    switch (v->kind)
    {
        case A_funcExp:
            A_def funcDef = S_look(funcFenv, v->u.funcc.idd);
            if(!funcDef){
                EM_error(v->pos, "undefined function: \'%s\'.", S_name(v->u.funcc.idd));
            }
            return funcDef->u.funcc.type;
            //checkExpList(venv, v->u.funcc.args);
            break;
        case A_struExp:
            if(v->u.struu.exp->kind != A_simpleExp){
                EM_error(v->pos, "unexpected expresion type before DOT.");
            }
            else{
                varty struVar = S_look(venv, v->u.struu.exp->u.simple.idd);
                if(!struVar){
                    EM_error(v->pos, "undefined variable: \'%s\'.", S_name(v->u.struu.exp->u.simple.idd));
                }
            }
            break;
        case A_simpleExp:
            varty simpleexp = S_look(venv, v->u.simple.idd);
            if(!simpleexp)
            {
                EM_error(v->pos, "undefined variable: \'%s\'.", S_name(v->u.simple.idd));
            }
            else{
                return simpleexp->t;
            }
            //fprintf(out, "simpleExp(%s)\n", S_name(v->u.simple.idd));
            break;
        case A_intExp:
            return A_SimpleTy(v->pos, A_INT);
            break;
        case A_floatExp:
            return A_SimpleTy(v->pos, A_FLOAT);
            break;
        case A_charExp:
            return A_SimpleTy(v->pos, A_CHAR);
            break;
        case A_opExp:
            A_ty ty1 = checkExp(venv, v->u.opp.left);
            A_ty ty2 = checkExp(venv, v->u.opp.right);
            switch (v->u.opp.oper)
        {
            case A_plusOp:
            case A_minusOp:
            case A_timesOp:
            case A_divideOp:
            {
                if (ty1->kind != A_simpleTy || ty1->u.simplee.type == A_CHAR)
                    EM_error(v->u.opp.left->pos, "Left hand side is not int or float type!");
                else if (ty2->kind != A_simpleTy || ty2->u.simplee.type == A_CHAR)
                    EM_error(v->u.opp.right->pos, "Right hand side is not int or float type!");
                else
                    return ty1;
                break;
            }
                /*case A_eqOp:
                 case A_neqOp:
                 {
                 switch (leftExp.ty->kind)
                 {
                 case Ty_int:
                 case Ty_string:
                 {
                 if (!equal_ty(rightExp.ty, leftExp.ty))
                 EM_error(e->u.op.right->pos, "Left and right hand side types do not match.");
                 else
                 OpExp = Tr_stringCmpExp(e->u.op.oper, leftExp.exp, rightExp.exp);
                 break;
                 }
                 case Ty_array:
                 {
                 if (!equal_ty(rightExp.ty, leftExp.ty))
                 EM_error(e->u.op.right->pos, "Left and right hand side types do not match.");
                 else
                 OpExp = Tr_comOpExp(e->u.op.oper, leftExp.exp, rightExp.exp);
                 break;
                 }
                 case Ty_record:
                 {
                 if (!equal_ty(rightExp.ty, leftExp.ty))
                 EM_error(e->u.op.right->pos, "Left and right hand side types do not match.");
                 else if (equal_ty(rightExp.ty, leftExp.ty) || rightExp.ty->kind == Ty_nil)
                 OpExp = Tr_comOpExp(e->u.op.oper, leftExp.exp, rightExp.exp);
                 break;
                 }
                 default:
                 EM_error(e->u.op.right->pos, "Unexpected type of left hand side.");
                 }
                 break;
                 }
                 case A_ltOp:
                 case A_leOp:
                 case A_gtOp:
                 case A_geOp:
                 {
                 switch (leftExp.ty->kind)
                 {
                 case Ty_int:
                 {
                 if (!rightExp.ty->kind == Ty_int)
                 EM_error(e->u.op.right->pos, "Right hand side type is expected to be int.");
                 else
                 OpExp = Tr_comOpExp(e->u.op.oper, leftExp.exp, rightExp.exp);
                 break;
                 }
                 case Ty_string:
                 {
                 if (!rightExp.ty->kind == Ty_string)
                 EM_error(e->u.op.right->pos, "Right hand side type is expected to be string.");
                 else
                 OpExp = Tr_stringCmpExp(e->u.op.oper, leftExp.exp, rightExp.exp);
                 break;
                 }
                 default:
                 EM_error(e->u.op.right->pos, "Unexpected type of left hand side.");
                 }
                 break;
                 }*/
            default:
                break;
        }
            break;
        case A_assignExp:
            if (v->u.assignn.left->kind != A_simpleExp && v->u.assignn.left->kind != A_struExp && v->u.assignn.left->kind != A_arrayExp){
                EM_error(v->u.assignn.left->pos, "Unexpected type in the left hand of =");
            }
            else{
                A_ty ty1 = checkExp(venv, v->u.assignn.left);
                A_ty ty2 = checkExp(venv, v->u.assignn.right);
                if (!equalType(ty1, ty2))
                {
                    EM_error(v->u.assignn.right->pos, "Unexpected type in the right hand of =");
                }
                else
                {
                    return ty1;
                }
            }
            break;
        case A_notExp:
            return Checkexp(venv, v->u.nott.exp);
            break;
        case A_arrayExp:
            
            return A_SimpleTy(v->pos, A_INT);
            
    }
    
    static void checkDec(S_table venv, A_dec v, A_ty t)
    {
        switch (v->kind)
        {
            case A_simpleDec:
                checkDecVar(venv, v->u.simpp.var, t);
                if (v->u.simpp.init != NULL)
                {
                    checkExp(venv, v->u.simpp.init);
                }
                break;
            case A_funcDec:
                
                //S_enter(funcFenv, v->u.funcc.func, funTy());
                //fprintf(out, "funcDec(%s\n", S_name(v->u.funcc.func));
                //S_table newVenv = E_base_venv();
                //checkParamList(newVenv, v->u.funcc.body);
                break;
            default:
                assert(0);
        }
        
    }
    
    static void checkTy(S_table venv, A_ty v)
    {
        switch (v->kind)
        {
            case A_simpleTy:
                break;
            case A_struTy:
                A_ty typet = S_look(typeTenv, v->u.structt.tag);
                if(!typet)
                {
                    EM_error(v->pos, "undefined type: \'%s\'.", S_name(v->u.structt.tag));
                }
                //checkDefList(venv, v->u.structt.deflist);
                break;
            default:
                assert(0);
        }
    }
    
    static void checkExpList(S_table venv, A_expList v)
    {
        if (v)
        {
            checkExp(venv, v->head);
            if (v->tail)
                checkExpList(venv, v->tail);
        }
    }
    
    static void checkDecList(S_table venv, A_decList v, A_ty t)
    {
        if (v)
        {
            checkDec(venv, v->head, t);
            checkDecList(venv, v->tail, t);
        }
    }
    
    //tr_defList ir_root;
    //addrootESEQ
    static void checkDefList(S_table venv, A_decList v)
    {
        if (v){
            checkDef(venv, v->head);
            checkDefList(venv, v->tail);
            
        }
    }
    
    static void checkDef(S_table venv, A_def v)
    {
        switch (v->kind)
        {
            case A_globalDef:
                checkTy(globalVenv, v->u.globall.type);
                checkDecList(globalVenv, v->u.globall.declist, v->u.globall.type);
                break;
            case A_structDef:
                checkDefList(venv, v->u.structt.type->u.structt.deflist);
                //添加到tenv
                S_enter(typeTenv, v->u.structt.type->u.structt.tag, v->u.structt.type);
                break;
            case A_funcDef:
                checkTy(venv, v->u.funcc.type);
                S_table newVenv = E_base_venv();
                checkFuncDec(newVenv, v->u.funcc.funcdec, v->u.funcc.type);
                //checkFuncDec(venv, tenv, v->u.funcc.funcdec);
                //这里的venv需要考虑
                checkStmt(newVenv, v->u.funcc.compst);
                break;
            case A_localDef:
                checkTy(venv, v->u.loacll.dec);
                checkDecList(venv, v->u.loacll.declist, v->u.loacll.dec);
                break;
            default:
                assert(0);
        }
    }
    
    static void checkFuncDec(S_table venv, A_dec v, A_ty t)
    {
        S_enter(funcFenv, v->u.funcc.func, funcTy(v, t));
        checkParamList(venv, v->u.funcc.body);
    }
    
    static void checkStmt(S_table venv, A_stmt v)
    {
        switch (v->kind)
        {
            case A_expStmt:
                checkExp(venv, v->u.expp.exp);
                break;
            case A_retnStmt:
                checkExp(venv, v->u.retn.exp);
                break;
            case A_ifStmt:
                checkExp(venv, v->u.iff.condition);
                checkStmt(venv, v->u.iff.iff);
                if (v->u.iff.elsee)
                {
                    checkStmt(venv, v->u.iff.elsee);
                }
                break;
            case A_whileStmt:
                checkExp(venv, v->u.whilee.condition);
                checkStmt(venv, v->u.whilee.stmt);
                break;
            case A_comppStmt:
                if (v->u.compp.deflist)
                {
                    checkDefList(venv, v->u.compp.deflist);
                }
                if (v->u.compp.stmtlist)
                {
                    checkStmtList(venv, v->u.compp.stmtlist);
                }
                break;
        }
    }
    static void checkStmtList(S_table venv, A_stmtList v)
    {
        if(v)
        {
            checkStmt(venv, v->head);
            checkStmtList(venv, v->tail);
        }
    }
    
    static void checkParamDec(S_table venv, A_paramDec v)
    {
        checkTy(venv, v->type);
        checkDecVar(venv, v->var, v->type);
    }
    
    static void checkParamList(S_table venv, A_paramList v)
    {
        if (v)
        {
            checkParamDec(venv, v->head);
            checkParamList(venv, v->tail);
        }
    }
