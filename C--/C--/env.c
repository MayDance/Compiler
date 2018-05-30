//
//
//by LBQ
//Reference: Textbook Page 74-82
#include "absyn.h"
#include "types.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "translate.h"
#include "env.h"
#include <stdlib.h>



S_table E_base_tenv(void)   //basic type
{
	S_table Ty_ty_BaseTable = S_empty();
	S_enter(Ty_ty_BaseTable, S_Symbol("int"), Ty_Int());
	S_enter(Ty_ty_BaseTable, S_Symbol("float"), Ty_Float());
	S_enter(Ty_ty_BaseTable, S_Symbol("char"), Ty_Char());

	return Ty_ty_BaseTable;
}

E_enventry E_VarEntry(Tr_access access, Ty_ty ty)  // value variable
{
	E_enventry env = checked_malloc(sizeof(*env));

	env->kind = E_varEntry;
	env->u.var.access = access;
	env->u.var.ty = ty;
	return env;
}

E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals, Ty_ty result) //value function
{
	E_enventry env = checked_malloc(sizeof(*env));

	env->kind = E_funEntry;
	env->u.fun.formals = formals;
	env->u.fun.level = level;
	env->u.fun.label = label;
	env->u.fun.result  = result;

	return env;
}
