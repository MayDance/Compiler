//
//  parse.c
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
 
// parse.c - Parse source file.
// sematic analysis
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

//extern anyErrors;

// parse source file fname; return abstract syntax data structure
A_defList parse(string fname) {
    EM_reset(fname);
    if (yyparse() == 0) // parsing worked
        return absyn_root;
    else return NULL;
}

int main(int argc, char **argv) {
    A_defList absyn_root;
    FILE *out = stdout;
    
    if (argc==2) {
        fprintf(out, "\n++++++++++++Lexical and Semantic Analysis Begin!++++++++++++\n");
        absyn_root = parse(argv[1]);
        if (!absyn_root) {
            fprintf(out, "\n++++++++++++Semantic Analysis Failed!++++++++++++\n");
            return 1;
        }
        pr_defList(out, absyn_root, 0); // print absyn data structure
        //fragList = SEM_transProg(absyn_root);
        fprintf(out, "\n++++++++++++Semantic Analysis Successed!++++++++++++\n");
    }
    return 0;
}
