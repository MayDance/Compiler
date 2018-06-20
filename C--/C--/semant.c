/*
 Author:  Jing Yao
 Reference: Chpt.5.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "absyn.h"
#include "errormsg.h"
#include "semant.h"
#include "symbol.h"

/*-----------------------------------------------------------------------------------------------------*/
/*constructor for expTy*/
/*Reference: Textbook Page 82*/
varEnv globalVenv = NULL;
funcEnv funcFenv = NULL;
tyEnv typeTenv = NULL;
int numtype = 3; //1-int, 2-float, 3 char
/*-----------------------------------------------------------------------------------------------------*/
static int checkExp(varEnv venv, A_exp v);
static void checkDec(varEnv venv, A_dec v, int t);
static void checkDecVar(varEnv venv, A_var v, int t);
static int checkTy(varEnv venv, A_ty v);
static void checkFuncDec(varEnv venv, A_dec v, int t);
static void checkExpList(varEnv venv, A_expList v);
static void checkDecList(varEnv venv, A_decList v, int t);
static void checkDefList(varEnv venv, A_defList v);
static void checkDef(varEnv venv, A_def v);
static void checkStmt(varEnv venv, A_stmt v, int t);
static void checkStmtList(varEnv venv, A_stmtList v, int t);
static void checkParamDec(varEnv venv, A_paramDec v);
static void checkParamList(varEnv venv, A_paramList v);
void addStruDec(tyEnv pstr, A_defList v);

/*-----------------------------------------------------------------------------------------------------*/
/*Functions Within the module*/

/*return real type in Ty_ty in case of Ty_name*/
/*Reference Textbook Page 83*/

varEnv insertVar(varEnv pvh, string varname, int varty, int size)
{
    varEnv p = checked_malloc(sizeof(*p));
    p->varname = varname;
    p->vartype = varty;
    p->size = size;
    p->next = pvh;
    pvh = p;
    return pvh;
}
varEnv findVar(varEnv pvh, string varname)
{
    varEnv tmp = pvh;
    for(; tmp; tmp = tmp->next){
        if(strcmp(varname, tmp->varname) == 0)
            return tmp;
    }
    return NULL;
}

funcEnv insertFunc(funcEnv pfh, string funcname, int functy)
{
    funcEnv p = checked_malloc(sizeof(*p));
    p->funcname = funcname;
    p->functype = functy;
    p->head = NULL;
    p->next = pfh;
    pfh = p;
    return pfh;
}

void insertFuncVar(funcEnv pft, string varname, int varty)
{
    varEnv p = checked_malloc(sizeof(*p));
    p->varname = varname;
    p->vartype = varty;
    p->size = 0;
    p->next = pft->head;
    pft->head = p;
}

funcEnv findFunc(funcEnv pfh, string funcname)
{
    funcEnv tmp = pfh;
    for (; tmp; tmp = tmp->next)
    {
        if (strcmp(funcname, tmp->funcname) == 0)
            return tmp;
    }
    return NULL;
}

tyEnv insertTy(tyEnv pth, string tyname)
{
    tyEnv p = checked_malloc(sizeof(*p));
    p->tyname = tyname;
    p->tyid = ++numtype;
    p->head = NULL;
    p->next = pth;
    pth = p;
    return p;
}

void insertTyVar(tyEnv ptt, string varname, int varty, int size)
{
    varEnv p = checked_malloc(sizeof(*p));
    p->varname = varname;
    p->vartype = varty;
    p->size = 0;
    p->next = ptt->head;
    ptt->head = p;
}

tyEnv findTy(tyEnv pth, int tyid)
{
    tyEnv tmp = pth;
    for (; tmp; tmp = tmp->next)
    {
        if (tyid == tmp->tyid)
            return tmp;
    }
    return NULL;
}
tyEnv findTyname(tyEnv pth, string tyname)
{
    tyEnv tmp = pth;
    for (; tmp; tmp = tmp->next)
    {
        if (strcmp(tmp->tyname, tyname)==0)
            return tmp;
    }
    return NULL;
}

int findStruVar(tyEnv ptt, string struname)
{
    varEnv tmp = ptt->head;
    for(;tmp;tmp = tmp->next){
        if(strcmp(tmp->varname, struname)==0)
            return tmp->vartype;
    }
    return 0;
}

int equalType(int ty1, int ty2)
{
    if(ty1 == ty2)
        return 1;
    else if(ty1==1&&ty2==2)
        return 1;
    else if(ty1==2&&ty2==1)
        return 1;
    else
        return 0;
}

void startCheck(A_defList v)
{
    checkDefList(globalVenv, v);
}

static void checkDecVar(varEnv venv, A_var v, int t)
{
	varEnv simpleVar;
	varEnv arrayVar;
    switch (v->kind)
    {
        case A_simpleVar:
            simpleVar = findVar(venv, S_name(v->u.simplee));
            /*Found*/
            if (simpleVar)
            {
                EM_error(v->pos, "redefined variable: \'%s\'.", S_name(v->u.simplee));
            }
            else
            {
                insertVar(venv, S_name(v->u.simplee), t, 0);
            }
            break;
        case A_arrayVar:
            arrayVar = findVar(venv, S_name(v->u.arrayy.simple->u.simplee));
            if (arrayVar)
            {
                EM_error(v->pos, "redefined variable: \'%s\'.", S_name(v->u.arrayy.simple->u.simplee));
            }
            else
            {
                insertVar(venv, S_name(v->u.arrayy.simple->u.simplee), t, v->u.arrayy.size);
            }
            
            break;
    }
}

static int checkExp(varEnv venv, A_exp v)
{
	funcEnv funcDef;
	varEnv simpleexp;
	int ty1, ty2;
    switch (v->kind)
    {
        case A_funcExp:
            funcDef = findFunc(funcFenv, S_name(v->u.funcc.idd));
            if(!funcDef){
                EM_error(v->pos, "undefined function: \'%s\'.", S_name(v->u.funcc.idd));
            }
            else{

            }
            return funcDef->functype;
            //checkExpList(venv, v->u.funcc.args);
            break;
        case A_struExp:
            if(v->u.struu.exp->kind != A_simpleExp){
                EM_error(v->pos, "unexpected expresion type before DOT.");
            }
            else{
                varEnv struVar = findVar(venv, S_name(v->u.struu.exp->u.simple.idd));
                if(!struVar){
                    EM_error(v->pos, "undefined variable: \'%s\'.", S_name(v->u.struu.exp->u.simple.idd));
                }
                else{
                    tyEnv struty = findTy(typeTenv, struVar->vartype);
                    int tyid = findStruVar(struty, S_name(v->u.struu.stru));
                    if(!tyid){
                        EM_error(v->pos, "undefined variable: \'%s\' in \'%s\'.", S_name(v->u.struu.stru), struty->tyname);
                    }
                    else{
                        return tyid;
                    }
                }
            }
            break;
        case A_simpleExp:
            simpleexp = findVar(venv, S_name(v->u.simple.idd));
            if(!simpleexp)
            {
				simpleexp = findVar(globalVenv, S_name(v->u.simple.idd));
				if (!simpleexp)
					EM_error(v->pos, "undefined variable: \'%s\'.", S_name(v->u.simple.idd));
				else
					return simpleexp->vartype;
            }
            else{
                return simpleexp->vartype;
            }
            //fprintf(out, "simpleExp(%s)\n", S_name(v->u.simple.idd));
            break;
        case A_intExp:
            return 1;
            break;
        case A_floatExp:
            return 2;
            break;
        case A_charExp:
            return 3;
            break;
        case A_opExp:
            ty1 = checkExp(venv, v->u.opp.left);
            ty2 = checkExp(venv, v->u.opp.right);
            switch (v->u.opp.oper)
            {
            case A_plusOp:
            case A_minusOp:
            case A_timesOp:
            case A_divideOp:
            {
                if (ty1!=1 && ty1!=2)
                    EM_error(v->u.opp.left->pos, "Left hand side is not int or float type!");
                else if (ty2!=1&&ty2!=2)
                    EM_error(v->u.opp.right->pos, "Right hand side is not int or float type!");
                else
                    return ty1;
                break;
            }
            case A_andOP:
            case A_orOP:
            {
                return 1;
                break;
            }
            default:
                break;
            }
            break;
        case A_relExp:
        {
            ty1 = checkExp(venv, v->u.opp.left);
            ty2 = checkExp(venv, v->u.opp.right);
            switch (v->u.rell.rel){
                case A_eeRel:
                case A_neRel:
                    if (ty1 != 1 && ty1 != 2 && ty1 != 3)
                        EM_error(v->pos, "Unexpected left hand of == or !=");
                    else if (ty2 != 1 && ty2 != 2 && ty2 != 3)
                        EM_error(v->pos, "Unexpected right hand of == or !=");
                    
                    else
                        return 1;
                    break;
                default:
                    if (ty1 != 1 && ty1 != 2)
                        EM_error(v->pos, "Left hand side is not int or float type!");
                    else if (ty2 != 1 && ty2 != 2)
                        EM_error(v->pos, "Right hand side is not int or float type!");
                    else
                        return 1;
                    break;
            }
            break;
        }
        case A_assignExp:
            if (v->u.assignn.left->kind != A_simpleExp && v->u.assignn.left->kind != A_struExp && v->u.assignn.left->kind != A_arrayExp){
                EM_error(v->u.assignn.left->pos, "Unexpected type in the left hand of =");
            }
            else{
                ty1 = checkExp(venv, v->u.assignn.left);
                ty2 = checkExp(venv, v->u.assignn.right);
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
            return checkExp(venv, v->u.nott.exp);
            break;
        case A_arrayExp:
            ty1 = checkExp(venv, v->u.arrayy.exp1);
            ty2 = checkExp(venv, v->u.arrayy.exp2);
            if(ty2!=1)
                EM_error(v->pos, "The index is not int type!");   
            else
            {
                return ty1;
            }
            break;
        default:
            EM_error(v->pos, "Unknown expression!");
            break;
            
    }
    return 1;//错误的统一用int类型
}

static void checkDec(varEnv venv, A_dec v, int t)
{
    switch (v->kind)
    {
    case A_simpleDec:
        checkDecVar(venv, v->u.simpp.var, t);
        if (v->u.simpp.init != NULL)
        {
            int ty = checkExp(venv, v->u.simpp.init);
            if (ty != t)
            {
                EM_error(v->pos, "Unexpected type in the right hand of =");
            }
        }
        break;
    case A_funcDec:

        //S_enter(funcFenv, v->u.funcc.func, funTy());
        //fprintf(out, "funcDec(%s\n", S_name(v->u.funcc.func));
        //S_table newVenv = E_base_venv();
        //checkParamList(newVenv, v->u.funcc.body);
        break;
    default:
        EM_error(v->pos, "Unknown declaration");
        break;
    }
}

static int checkTy(varEnv venv, A_ty v)
{
	tyEnv typet;
    switch (v->kind)
    {
    case A_simpleTy:
        switch(v->u.simplee.type)
        {
        case A_INT:
            return 1;
            break;
        case A_FLOAT:
            return 2;
            break;
		case A_CHAR:
            return 3;
            break;
        }
        break;
    case A_struTy:
        typet = findTyname(typeTenv, S_name(v->u.structt.tag));
        if (!typet)
        {
            EM_error(v->pos, "undefined type: \'%s\'.", S_name(v->u.structt.tag));
        }
        else
            return typet->tyid;
        //checkDefList(venv, v->u.structt.deflist);
        break;
    default:
        EM_error(v->pos, "unknown type");
        break;
    }
    return 1;
}

static void checkExpList(varEnv venv, A_expList v)
{
    if (v)
    {
        checkExp(venv, v->head);
        if (v->tail)
            checkExpList(venv, v->tail);
    }
}

static void checkDecList(varEnv venv, A_decList v, int t)
{
    if (v)
    {
        checkDec(venv, v->head, t);
        checkDecList(venv, v->tail, t);
    }
}

static void checkDefList(varEnv venv, A_defList v)
{
    if (v)
    {
        checkDef(venv, v->head);
        checkDefList(venv, v->tail);
    }
}

void addStruDec(tyEnv pstr, A_defList v)
{
	A_defList tmp = v;
	A_def tmp1;
	A_decList dectmp;
	A_dec dectmp1;
	int ty;
	while (tmp) {
		tmp1 = tmp->head;
		if (tmp1->kind != A_localDef)
			EM_error(tmp1->pos, "Illegal definition!");
		else {	
			ty = checkTy(globalVenv, tmp1->u.loacll.dec);
			dectmp = tmp1->u.loacll.declist;
			while (dectmp) {
				dectmp1 = dectmp->head;
				if(dectmp1->kind == A_funcDec)
					EM_error(dectmp1->pos, "Illegal declaration!");
				else {
					if(dectmp1->u.simpp.init != NULL)
						EM_error(dectmp1->pos, "Illegal initialization!");
					else {
						checkDecVar(pstr->head, dectmp1->u.simpp.var, ty);
					}
				}
				dectmp = dectmp->tail;
			}
		}
		tmp = tmp->tail;
	}

}

static void checkDef(varEnv venv, A_def v)
{
	int tydec;
	tyEnv newty;
	varEnv newVenv = NULL;
    switch (v->kind)
    {
    case A_globalDef:
        tydec = checkTy(globalVenv, v->u.globall.type);
        checkDecList(globalVenv, v->u.globall.declist, tydec);
        break;
    case A_structDef:
        newty = insertTy(typeTenv, S_name(v->u.structt.type->u.structt.tag));
		addStruDec(newty, v->u.structt.type->u.structt.deflist);
        break;
    case A_funcDef:
        tydec = checkTy(venv, v->u.funcc.type);	
        checkFuncDec(newVenv, v->u.funcc.funcdec, tydec);
        checkStmt(newVenv, v->u.funcc.compst, tydec);
        break;
    case A_localDef:
        tydec = checkTy(venv, v->u.loacll.dec);
        checkDecList(venv, v->u.loacll.declist, tydec);
        break;
    default:
        assert(0);
    }
}

static void checkFuncDec(varEnv venv, A_dec v, int t)
{
	insertFunc(funcFenv, S_name(v->u.funcc.func), t);
    checkParamList(venv, v->u.funcc.body);
}

static void checkStmt(varEnv venv, A_stmt v, int t)
{
	int tystmt;
	switch (v->kind)
	{
	case A_expStmt:
		tystmt = checkExp(venv, v->u.expp.exp);
		break;
	case A_retnStmt:
		tystmt = checkExp(venv, v->u.retn.exp);
		if (t != tystmt) {
			EM_error(v->pos, "Unexpected return type!");
		}
		break;
	case A_ifStmt:
		tystmt = checkExp(venv, v->u.iff.condition);
		checkStmt(venv, v->u.iff.iff, t);
		if (v->u.iff.elsee)
		{
			checkStmt(venv, v->u.iff.elsee, t);
		}
		break;
	case A_whileStmt:
		tystmt = checkExp(venv, v->u.whilee.condition);
		checkStmt(venv, v->u.whilee.stmt, t);
		break;
	case A_comppStmt:
		if (v->u.compp.deflist)
		{
			checkDefList(venv, v->u.compp.deflist);
		}
		if (v->u.compp.stmtlist)
		{
			checkStmtList(venv, v->u.compp.stmtlist, t);
		}
		break;
	case A_breakStmt:
	case A_continueStmt:
		break;
	}
}

static void checkStmtList(varEnv venv, A_stmtList v, int t)
{
    if (v)
    {
        checkStmt(venv, v->head, t);
        checkStmtList(venv, v->tail, t);
    }
}

static void checkParamDec(varEnv venv, A_paramDec v)
{
    int ty1 = checkTy(venv, v->type);
    checkDecVar(venv, v->var, ty1);
}

static void checkParamList(varEnv venv, A_paramList v)
{
    if (v)
    {
        checkParamDec(venv, v->head);
        checkParamList(venv, v->tail);
    }
}


