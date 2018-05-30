//
//  errormsg.c
//  C--
//
//  Created by Starblo Hong on 19/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//

//errormsg.c - functions used in all phases of the compiler to give error messages about the C-- program.
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "errormsg.h"

bool anyErrors= FALSE;

static string fileName = "";

static int lineNum = 1;

int EM_tokPos=0;

extern FILE *yyin;

typedef struct intList {int i; struct intList *rest;} *IntList;

static IntList intList(int i, IntList rest) {
    IntList l= checked_malloc(sizeof *l);
    l->i=i; l->rest=rest;
    return l;
}

static IntList linePos=NULL;

void EM_newline(void) {
    lineNum++;
    linePos = intList(EM_tokPos, linePos);
}

void EM_error(int pos, char *message,...) {
    va_list ap;
    IntList lines = linePos;
    int num=lineNum;
    
    printf("erroraccur\n");
    anyErrors=TRUE;
    while (lines && lines->i >= pos) {
        lines=lines->rest;
        num--;
    }
    if (fileName)
        fprintf(stderr,"%s:",fileName);
    if (lines)
        fprintf(stderr,"%d.%d: ", num, pos-lines->i);
    va_start(ap,message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr,"\n");
    printf("erroraccured\n");
}
 
void EM_reset(string fname) {
    anyErrors=FALSE;
    fileName=fname;
    lineNum=1;
    linePos=intList(0,NULL);
    yyin = fopen(fname,"r");
    if (!yyin) {
        EM_error(0,"cannot open");
        exit(1);
    }
}

