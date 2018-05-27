//
//  parse.h
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
#ifndef parse_h
#define parse_h
// function prototype from parse.c 

#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "prabsyn.h"

extern int yyparse(void);
extern A_defList absyn_root;

A_defList parse(string fname);

#endif
 
