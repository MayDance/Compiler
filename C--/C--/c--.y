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
    A_exp exp;
    A_dec dec;
    A_ty ty;

    A_varList varList;
    A_expList expList;
    A_decList decList;
    A_def def;
    A_defList defList;
    A_stmt stmt;
    A_stmtList stmtList;
    A_stru stru;
    A_paramDec paramDec;
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
%type <sym> Opttag ID
%type <var> Var
%type <varList> Varlist
%type <exp> Exp
%type <expList> Explist
%type <dec> Dec Funcdec Vardec Arraydec
%type <decList> Declist Program
%type <ty> Ty Varty Stru
%type <def> Def
%type <defList> Deflist
%type <stmt> Stmt
%type <stmtList> Stmtlist Compst
%type <stru> Stru
%type <paramDec> Paramdec
%%
{$$=A_VarExp(EM_tokPos,A_SimpleVar(EM_tokPos,S_Symbol($1)));}
// high-level definetions
Program:    Deflist     {absyn_root=$1;}
            ;

Deflist:     Def Deflist        {$$=A_decList(EM_tokPos, $1, $2);}
            |                   {$$=NULL;}
            ;

Def:     Specifier Declist SEMICOLON    {$$=A_globalDef(EM_tokPos, $1, $2);}
        |Specifier SEMICOLON            {$$=A_struDef(EM_tokPos, $1);}
        |Specifier Funcdec Compst       {$$=A_funcDef(EM_tokPos, $1, $2, $3);}
        |error SEMICOLON
        ;

Declist:         Dec                    {$$=decList(EM_tokPos, $1, NULL);}
                |Dec COMMA Declist      {$$=decList(EM_tokPos, $1, $3);}
                ;

// specifiers
Specifier:   ty         {$$=$1;}
            |Stru       {$$=$1;}
            ;

Stru:    STRUCT Opttag LC Deflist RC    {$$=struTy(EM_tokPos, $2, $4);}
        |STRUCT ID                      {$$=struTy(EM_tokPos, $2, NULL);}
        ;

Opttag:  ID     {$$=$1;}
        |       {$$=NULL;}
        ;

//declarators
Vardec:  ID                     {$$=varDec(EM_tokPos, $1);}
        |Vardec LB INT RB       {$$=arrayDec(EM_tokPos, $1, $3);}
        ;

Funcdec:         ID LP Varlist RP       {$$=funcDec(EM_tokPos, $1, $3);}
                |ID LP RP               {$$=funcDec(EM_tokPos, $1, NULL);}
                ;

Varlist:         Paramdec COMMA Varlist {$$=varList(EM_tokPos, $1, $3);}
                |Paramdec               {$$=varList(EM_tokPos, $1, NULL);}
                ;

Paramdec:        Specifier Vardec       {$$=paramDec(EM_tokPos, $1, $2);}
                ;

// statements
Compst:  LC Declist Stmtlist RC {$$=compStmt(EM_tokPos, $1, $2);}
        ;

Stmtlist:    Stmt Stmtlist      {$$=stmtList(EM_tokPos, $1, $2);}
            |                   {$$=NULL;}
            ;

Stmt:    Exp SEMICOLON                  {$$=stmt(EM_tokPos, $1);}
        |Compst                         {$$=stmt(EM_tokPos, $1);}
        |RETURN Exp SEMICOLON           {$$=stmt(EM_tokPos, $2);}
        |IF LP Exp RP Stmt              {$$=stmt(EM_tokPos, $3, $5, NULL);}
        |IF LP Exp RP Stmt ELSE Stmt    {$$=stmt(EM_tokPos, $3, $5, $7);}
        |WHILE LP Exp RP Stmt           {$$=stmt(EM_tokPos, $3, $5);}
        |BREAK SEMICOLON                {$$=stmt(EM_tokPos);}
        |CONTINUE SEMICOLON             {$$=stmt(EM_tokPos);}
        ;

// local definations
Deflist:         Def Deflist    {$$=defList(EM_tokPos, $1, $2);}
                |               {$$=NULL;}
                ;

Def:     Specifier Declist SEMICOLON    {$$=def(EM_tokPos, $1, $2);}
        ;

Declist:         Dec                    {$$=decList(EM_tokPos, $1, NULL);}
                |Dec COMMA Declist      {$$=decList(EM_tokPos, $1, $2);}
                ;

Dec:     Vardec                 {$$=$1;}
        |Vardec ASSIGN Exp      {$$=varDec(EM_tokPos, $1, $2);}

// expressions
Exp:     Exp ASSIGN Exp {$$=A_AssignExp(EM_tokPos, $1, $3);}
        |Exp AND Exp    {$$=A_OpExp(EM_tokPos, $1, $3);}
        |Exp OR Exp     {$$=A_OpExp(EM_tokPos, $1, $3);}
        |Exp REL Exp    {$$=A_OpExp(EM_tokPos, REL, $1, $3);}
        |Exp PLUS Exp   {$$=A_OpExp(EM_tokPos, A_plusOp, $1, $3);}
        |Exp MINUS Exp  {$$=A_OpExp(EM_tokPos, A_minusOp, $1, $3);}
        |Exp TIMES Exp  {$$=A_OpExp(EM_tokPos, A_timesOp, $1, $3);}
        |Exp DIVIDE Exp {$$=A_OpExp(EM_tokPos, A_divideOp, $1, $3);}
        |LP Exp RP      {$$=$2;}
        |MINUS Exp      {$$=A_OPExp(EM_tokPos, A_minusOp, 0, $2);}
        |NOT Exp        {$$=A_OpExp(EM_tokPos, $2);}
        |ID LP Args RP  {$$=A_FuncExp(EM_tokPos, $1, $3);}
        |Exp LB Exp RB  {$$=A_ArrayExp(EM_tokPos, $1, $3);}
        |Exp DOT ID     {$$=A_StruExp(EM_tokPos, $1, $3);}
        |ID             {$$=A_SimpleExp(EM_tokPos, $1);}
        |INT            {$$=A_IntExp(EM_tokPos, $1);}
        |FLOAT          {$$=A_FloatExp(EM_tokPos, $1);}

Args:    Exp COMMA Args {$$=A_Args(EM_tokPos, $1, $2);}
        |Exp            {$$=$1;}

%%
