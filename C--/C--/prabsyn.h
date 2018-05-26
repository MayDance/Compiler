//
//  prabsyn.h
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
#ifndef prabsyn_h
#define prabsyn_h

// function prototype from prabsyn.c
#include "util.h"
#include "symbol.h" // symbol table data structures
#include "absyn.h"  // abstract syntax data structures
void pr_exp(FILE *out, A_exp v, int d);

#endif
