//
//  parse.c
//  C--
//
//  Created by Starblo Hong on 20/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
 
// parse.c - Parse source file.
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"

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
        absyn_root = parse(argv[1]);
        if (!absyn_root) {
            fprintf(out, "testcase is empty!");
            return 1;
        }
        pr_defList(out, absyn_root, 0); // print absyn data structure
        fprintf(out, "\n");
    }
    return 0;
}
