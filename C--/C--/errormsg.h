//
//  errormsg.h
//  C--
//
//  Created by Starblo Hong on 19/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
#ifndef errormsg_h
#define errormsg_h

#include "util.h"
#include "errormsg.h"

extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int, string,...);
void EM_impossible(string,...);
void EM_reset(string filename);

#endif
