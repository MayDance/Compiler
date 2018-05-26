//
//  parse.c
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//

// parse.c - Parse source file.
#include <stdio.h>
#include "parse.h"

extern int yyparse(void);
extern A_exp absyn_root;

/* parse source file fname; return abstract syntax data structure */
A_exp parse(string fname) {
    EM_reset(fname);
    if (yyparse() == 0) /* parsing worked */
        return absyn_root;
    else return NULL;
}

