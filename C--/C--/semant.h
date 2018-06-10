/*Author: Jing Yao*/
#ifndef SEMANT_H
#define SEMANT_H
typedef struct varty_ *varty; // variable
typedef struct functy_ *functy;
typedef struct expty_ *expty;
struct expty_
{
    A_exp exp;
    Ty_ty ty;
};
struct varty_
{
    A_ty t;
    A_var v;
};
struct functy_
{
    A_ty t;
    A_dec f;
};

static void startCheck(A_defList v);
#endif
