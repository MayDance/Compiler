//
// semantic.h
// C--
//

#ifndef SEMANT_H
#define SEMANT_H

struct defty {
    Tr_def def;
    Ty_ty ty;
};

F_fragList SEM_transProg(A_def def);

#endif
