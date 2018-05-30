//
//
//by LBQ
//Reference: Textbook Page 74-82
#include <stdio.h>
#include "symbol.h"
#include "errormsg.h"
#include "types.h"

static struct Ty_ty_ tyint = { Ty_int };
static struct Ty_ty_ tyint = { Ty_int };
Ty_ty Ty_Int(void) { return &tyint; }
Ty_ty Ty_Float(void) { return &tyFloat; }
Ty_ty Ty_Char(void) { return &tyChar; }


Ty_ty Ty_Struc(Ty_fieldList fields)
{
	Ty_ty p = checked_malloc(sizeof(*p));
	p->kind = Ty_struc;
	p->u.struc = fields;
	return p;
}

Ty_ty Ty_Array(Ty_ty ty)
{
	Ty_ty p = checked_malloc(sizeof(*p));
	p->kind = Ty_array;
	p->u.array = ty;
	return p;
}

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail)
{
	Ty_tyList p = checked_malloc(sizeof(*p));
	p->head = head;
	p->tail = tail;
	return p;
}

Ty_field Ty_Field(S_symbol name, Ty_ty ty)
{
	Ty_field p = checked_malloc(sizeof(*p));
	p->name = name;
	p->ty = ty;
	return p;
}

Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail)
{
	Ty_fieldList p = checked_malloc(sizeof(*p));
	p->head = head;
	p->tail = tail;
	return p;
}

