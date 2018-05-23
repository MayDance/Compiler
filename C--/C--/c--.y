%{
#include <stdio.h>
#include "util.h"
#include "symbol.h" 
#include "errormsg.h"
#include "absyn.h"

int yylex(void); /* function prototype */

A_exp absyn_root;

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
}
%}

%union {
    int pos;
    int ival;
    float fval;
    char cval;
    string sval;
    S_symbol sym;
    A_var var;
    A_exp expr;
    A_dec dec;
    A_ty ty;
    A_decList decList;
    A_expList exprList;
    A_field field;
    A_fieldList fieldList;
    A_fundec fundec;
    A_fundecList fundecList;
    A_namety namety;
    A_nametyList nametyList;
    A_efield efield;
    A_efieldList efieldList;
}

// declare tokens
%token <sval> ID
%token <ival> INT
%token <fval> FLOAT
%token <cval> CHAR
%token 
ID INT FOLAT COMMA SEMICOLON PLUS MINUS TIMES DIVIDE ASSIGN NOT REL AND OR DOT TYPE
LP RP LB RB LC RC IF ELSE WHILE FOR BREAK CONTINUE STRUCT RETURN VOID TYPEDEF

%right ASSIGN
%left OR 
%left AND
%left REL
%left PLUS MINUS 
%left TIMES DIVIDE
%right NOT
%left DOT LP RP LB RB LC RC


// declare non-terminals
%type <var> Var
%type <expr> Expr
%type <exprList> Exprlist
%type <dec> Dec Funcdec Vardec Arraydec
%type <decList> Declist Program
%type <ty> ty Stru
%type <field>
%type <fieldList>
%type <fundec>
%type <fundecList>
%type <namety>
%type <nametyList>
%type <efield>
%type <efieldList>
%type <sym> Opttag
%%

// high-level definetions
Program:    Declist
            ;

Declist:     Dec Declist
            |
            ;

Exprdef:  Specifier Exprlist SEMICOLON
        |Specifier SEMICOLON
        |Specifier Funcdec Compst
        |error SEMICOLON
        ;

Exprlist:    Vardec
            |Vardec COMMA Exprlist
            ;

// specifiers
Specifier:   ty
            |Stru
            ;

Stru:    STRUCT Opttag LC decList RC SEMICOLON
        |STRUCT ID
        ;

Opttag:  ID
        |
        ;

//declarators
Vardec:  ID
        |Vardec LB INT RB
        ;

Funcdec: ID LP Varlist RP
        |exprList
        ;



// statements
Compst:  LC Declist Stmtlist RC
        ;

Stmtlist:    Stmt Stmtlist
            |
            ;

Stmt:    Expr SEMICOLON
        |Compst
        |RETURN expr SEMICOLON
        |IF LP expr RP Stmt
        |IF LP expr RP Stmt ELSE Stmt
        |WHILE LP expr RP Stmt
        |BREAK SEMICOLON
        |CONTINUE SEMICOLON
        ;

// local definations


// expressions
Expr:    Expr ASSIGN Expr
        |Expr AND Expr
        |Expr OR Expr
        |Expr REL Expr
        |Expr PLUS Expr
        |Expr MINUS Expr
        |Expr TIMES Expr
        |Expr DIVIDE Expr
        |LP Expr RP
        |MINUS Expr
        |NOT Expr
        |ID LP Args RP
        |Expr LB Expr RB
        |Expr DOT ID
        |ID
        |INT
        |FLOAT

Args:    expr COMMA Args
        |expr

%%
