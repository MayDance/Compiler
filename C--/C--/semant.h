/*Author: Jing Yao*/
#ifndef SEMANT_H
#define SEMANT_H
#include <string.h>
typedef struct varEnv_ *varEnv; // variable
typedef struct funcEnv_ *funcEnv;
typedef struct tyEnv_ *tyEnv;
struct tyEnv_
{
    string tyname;
    int tyid;
    varEnv head;
    tyEnv next;
};
struct varEnv_
{
    string varname;
    int vartype;
    int size;
    varEnv next;
};
struct funcEnv_
{
    string funcname;
    int functype;
    varEnv head;
    funcEnv next;
};

void startCheck(A_defList v);
#endif
