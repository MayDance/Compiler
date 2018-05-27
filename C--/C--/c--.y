%{
#include <stdio.h>
#include "symbol.h" 
#include "errormsg.h"
#include "absyn.h"

extern int yylex(void); // function prototype 

A_defList absyn_root;

void yyerror(char *s) {
        EM_error(EM_tokPos, "%s", s);
}

%}

%union {
    int pos;
    int ival;
    string sval;
    float fval;
    char cval;
    S_symbol sym;

    A_var var;
    A_exp exp;
    A_dec dec;
    A_ty ty;

    A_expList expList;
    A_decList decList;
    A_def def;
    A_defList defList;
    A_stmt stmt;
    A_stmtList stmtList;
    A_compStmt compStmt;
    A_paramDec paramDec;
    A_paramList paramList;
}

// declare tokens
%token <sval> ID
%token <ival> INT
%token <fval> FLOAT
%token <cval> CHAR
%token 
COMMA SEMICOLON PLUS MINUS TIMES DIVIDE ASSIGN NOT REL AND OR DOT TYPE
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
%type <sym> Opttag Id
%type <var> Var
%type <exp> Exp
%type <expList> Args
%type <dec> Dec Funcdec
%type <decList> Declist Program
%type <ty> Stru Specifier
%type <def> Def
%type <defList> Deflist
%type <stmt> Stmt
%type <stmtList> Stmtlist
%type <compStmt> Compst
%type <paramDec> Paramdec
%type <paramList> Paramlist
%%

// high-level definetions
Program:    Deflist     {absyn_root=$1;}
            ;

Deflist:     Def Deflist        {$$=A_DefList($1, $2);}
            |                   {$$=NULL;}
            ;

Def:     Specifier Declist SEMICOLON    {$$=A_GlobalDef(EM_tokPos, $1, $2);}
        |Specifier SEMICOLON            {$$=A_StructDef(EM_tokPos, $1);}
        |Specifier Funcdec Compst       {$$=A_FuncDef(EM_tokPos, $1, $2, $3);}
        ;

Declist:         Dec                    {$$=A_DecList($1, NULL);}
                |Dec COMMA Declist      {$$=A_DecList($1, $3);}
                ;

// specifiers
Specifier:       INT    {$$=A_SimpleTy(EM_tokPos, A_INT);}
                |FLOAT  {$$=A_SimpleTy(EM_tokPos, A_FLOAT);}
                |CHAR   {$$=A_SimpleTy(EM_tokPos, A_CHAR);}
                |Stru   {$$=$1;}
                ;

Stru:    STRUCT Opttag LC Deflist RC    {$$=A_StruTy(EM_tokPos, $2, $4);}
        |STRUCT Id                      {$$=A_StruTy(EM_tokPos, $2, NULL);}
        ;

Opttag:  Id     {$$=$1;}
        |       {$$=NULL;}
        ;

//declarators
Var:     Id                     {$$=A_SimpleVar(EM_tokPos, $1);}
        |Var LB INT RB       {$$=A_ArrayVar(EM_tokPos, $1, $3);}
        ;

Funcdec:         Id LP Paramlist RP       {$$=A_FuncDec(EM_tokPos, $1, $3);}
                |Id LP RP               {$$=A_FuncDec(EM_tokPos, $1, NULL);}
                ;

Paramlist:       Paramdec COMMA Paramlist       {$$=A_ParamList($1, $3);}
                |Paramdec                       {$$=A_ParamList($1, NULL);}
                ;

Paramdec:        Specifier Var       {$$=A_ParamDec(EM_tokPos, $1, $2);}
                ;

// statements
Compst:  LC Declist Stmtlist RC {$$=A_CompStmt(EM_tokPos, $2, $3);}
        ;

Stmtlist:    Stmt Stmtlist      {$$=A_StmtList($1, $2);}
            |                   {$$=NULL;}
            ;

Stmt:    Exp SEMICOLON                  {$$=A_ExpStmt(EM_tokPos, $1);}
        |Compst                         {$$=A_ComppStmt(EM_tokPos, $1);}
        |RETURN Exp SEMICOLON           {$$=A_RetnStmt(EM_tokPos, $2);}
        |IF LP Exp RP Stmt              {$$=A_IfStmt(EM_tokPos, $3, $5, NULL);}
        |IF LP Exp RP Stmt ELSE Stmt    {$$=A_IfStmt(EM_tokPos, $3, $5, $7);}
        |WHILE LP Exp RP Stmt           {$$=A_WhileStmt(EM_tokPos, $3, $5);}
        |BREAK SEMICOLON                {$$=A_BreakStmt(EM_tokPos);}
        |CONTINUE SEMICOLON             {$$=A_ContinueStmt(EM_tokPos);}
        ;

// local definations
Deflist:         Def Deflist    {$$=A_DefList($1, $2);}
                |               {$$=NULL;}
                ;

Def:     Specifier Declist SEMICOLON    {$$=A_LocalDef(EM_tokPos, $1, $2);}
        ;

Declist:         Dec                    {$$=A_DecList($1, NULL);}
                |Dec COMMA Declist      {$$=A_DecList($1, $3);}
                ;

Dec:     Var                 {$$=A_VarDec(EM_tokPos, $1, NULL);}
        |Var ASSIGN Exp      {$$=A_VarDec(EM_tokPos, $1, $3);}

Id:      ID     {$$=S_Symbol($1);}
        ;

// expressions
Exp:     Exp ASSIGN Exp {$$=A_AssignExp(EM_tokPos, $1, $3);}
        |Exp AND Exp    {$$=A_OpExp(EM_tokPos, A_andOP, $1, $3);}
        |Exp OR Exp     {$$=A_OpExp(EM_tokPos, A_orOP, $1, $3);}
        |Exp REL Exp    {$$=A_OpExp(EM_tokPos, REL, $1, $3);}
        |Exp PLUS Exp   {$$=A_OpExp(EM_tokPos, A_plusOp, $1, $3);}
        |Exp MINUS Exp  {$$=A_OpExp(EM_tokPos, A_minusOp, $1, $3);}
        |Exp TIMES Exp  {$$=A_OpExp(EM_tokPos, A_timesOp, $1, $3);}
        |Exp DIVIDE Exp {$$=A_OpExp(EM_tokPos, A_divideOp, $1, $3);}
        |LP Exp RP      {$$=$2;}
        |MINUS Exp      {$$=A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), $2);}
        |NOT Exp        {$$=A_NotExp(EM_tokPos, $2);}
        |Id LP Args RP  {$$=A_FuncExp(EM_tokPos, $1, $3);}
        |Exp LB Exp RB  {$$=A_ArrayExp(EM_tokPos, $1, $3);}
        |Exp DOT Id     {$$=A_StruExp(EM_tokPos, $1, $3);}
        |Id             {$$=A_SimpleExp(EM_tokPos, $1);}
        |INT            {$$=A_IntExp(EM_tokPos, $1);}
        |FLOAT          {$$=A_FloatExp(EM_tokPos, $1);}
        |CHAR           {$$=A_IntExp(EM_tokPos, $1);}

Args:    Exp COMMA Args {$$=A_ExpList($1, $3);}
        |Exp            {$$=A_ExpList($1, NULL);}

%%
