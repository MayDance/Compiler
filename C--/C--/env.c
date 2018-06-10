
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
    
    return Ty_ty_BaseTable;
}

S_table E_base_venv(void)
{
    S_table E_enventry_BaseTable = S_empty();
    return E_enventry_BaseTable;
}
