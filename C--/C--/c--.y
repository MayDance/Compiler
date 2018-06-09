%{
#include <stdio.h>
#include "symbol.h" 
#include "errormsg.h"
#include "absyn.h"

extern int yylex(void); // function prototype 

A_defList absyn_root;

void yyerror(char *s) {
    printf("?????\n");
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

    A_funcDec funcDec;
    A_varList varList;
    A_expList expList;
    A_decList decList;
    A_def def;
    A_defList defList;
    A_stmt stmt;
    A_stmtList stmtList;
    A_paramDec paramDec;
    A_paramList paramList;
}

// declare tokens
%token <sval> ID
%token <ival> INT
%token <fval> FLOAT
%token <cval> CHAR
%token 
COMMA SEMICOLON PLUS MINUS TIMES DIVIDE ASSIGN NOT REL AND OR DOT T_INT T_FLOAT T_CHAR
LP RP LB RB LC RC IF ELSE WHILE FOR BREAK CONTINUE STRUCT RETURN VOID TYPEDEF
%right ASSIGN
%left OR 
%left AND
%left REL
%left PLUS MINUS 
%left TIMES DIVIDE
%right NOT
%left DOT LP RP LB RB LC RC
%nonassoc ELSE
%nonassoc STRUCT RETURN WHILE

// declare non-terminals
%type <sym> Opttag Tag Id
%type <var> Var
%type <exp> Exp
%type <expList> Args
%type <dec> Dec
%type <funcDec> Funcdec
%type <decList> Declist ExtDeclist
%type <ty> Stru Specifier
%type <def> Def ExtDef
%type <defList> Deflist ExtDeflist Program
%type <stmt> Stmt Compst
%type <stmtList> Stmtlist
%type <paramDec> Paramdec
%type <paramList> Paramlist
%%

// high-level definetions
Program:    ExtDeflist     {absyn_root=$1;printf("--program\n");}
            ;

ExtDeflist:      ExtDef ExtDeflist      {$$=A_DefList($1, $2);printf("--extdeflist\n");}
                |                       {$$=NULL;printf("--nullextdeflist\n");}
                ;

ExtDef:  Specifier ExtDeclist SEMICOLON {$$=A_GlobalDef(EM_tokPos, $1, $2);printf("--global\n");}
        |Specifier SEMICOLON            {$$=A_StructDef(EM_tokPos, $1);printf("--struct\n");}
        |Specifier Funcdec Compst       {$$=A_FuncDef(EM_tokPos, $1, $2, $3);printf("--funcdec\n");}
        ;

ExtDeclist:      Var                    {$$=A_DecList(A_Dec(EM_tokPos, $1, NULL), NULL);printf("--extdeclistvar\n");}
                |Var COMMA ExtDeclist   {$$=A_DecList(A_Dec(EM_tokPos, $1, NULL), $3);printf("--extdeclist\n");}
                ;

// specifiers
Specifier:       T_INT    {$$=A_SimpleTy(EM_tokPos, A_INT);printf("--specifierint\n");}
                |T_FLOAT  {$$=A_SimpleTy(EM_tokPos, A_FLOAT);printf("--specfloat\n");}
                |T_CHAR   {$$=A_SimpleTy(EM_tokPos, A_CHAR);printf("--specchar\n");}
                |Stru   {$$=$1;printf("--specstru\n");}
                ;

Stru:    STRUCT Opttag LC Deflist RC    {$$=A_StruTy(EM_tokPos, $2, $4);printf("--stru\n");}
        |STRUCT Tag                      {$$=A_StruTy(EM_tokPos, $2, NULL);printf("--struid\n");}
        ;

Opttag:  Id     {$$=$1;printf("--id\n");}
        |       {$$=NULL;printf("--nullid\n");}
        ;

Tag:     Id      {$$=$1;printf("--tag\n");}
        ;

//declarators
Var:     Id                     {$$=A_SimpleVar(EM_tokPos, $1);printf("--var\n");}
        |Var LB INT RB          {$$=A_ArrayVar(EM_tokPos, $1, $3);printf("--var[]\n");}
        ;

Funcdec:         Id LP Paramlist RP     {$$=A_FuncDec(EM_tokPos, $1, $3);printf("--funcdec\n");}
                |Id LP RP               {$$=A_FuncDec(EM_tokPos, $1, NULL);printf("--nullfuncdec\n");}
                ;

Paramlist:       Paramdec COMMA Paramlist       {$$=A_ParamList($1, $3);printf("--paramlist\n");}
                |Paramdec                       {$$=A_ParamList($1, NULL);printf("--paramdec\n");}
                ;

Paramdec:        Specifier Var       {$$=A_ParamDec(EM_tokPos, $1, $2);printf("--param\n");}
                ;

// statements
Compst:  LC Deflist Stmtlist RC {$$=A_CompStmt(EM_tokPos, $2, $3);printf("--compst\n");}
        ;

Stmtlist:    Stmt Stmtlist      {$$=A_StmtList($1, $2);printf("--stmtlist\n");}
            |                   {$$=NULL;printf("--nullstmtlist\n");}
            ;

Stmt:    Exp SEMICOLON                  {$$=A_ExpStmt(EM_tokPos, $1);printf("--expstmt\n");}
        |Compst                         {$$=A_ComppStmt(EM_tokPos, $1);printf("--comppstmtn");}
        |RETURN Exp SEMICOLON           {$$=A_RetnStmt(EM_tokPos, $2);printf("--retnstmt\n");}
        |IF LP Exp RP Stmt              {$$=A_IfStmt(EM_tokPos, $3, $5, NULL);printf("--ifstmt\n");}
        |IF LP Exp RP Stmt ELSE Stmt    {$$=A_IfStmt(EM_tokPos, $3, $5, $7);printf("--ifelsestmt\n");}
        |WHILE LP Exp RP Stmt           {$$=A_WhileStmt(EM_tokPos, $3, $5);printf("--whilestmt\n");}
        |BREAK SEMICOLON                {$$=A_BreakStmt(EM_tokPos);printf("--breakstmt\n");}
        |CONTINUE SEMICOLON             {$$=A_ContinueStmt(EM_tokPos);printf("--continuestmt\n");}
        ;

// local definations
Deflist:     Def Deflist        {$$=A_DefList($1, $2);printf("--deflist\n");}
            |                   {$$=NULL;printf("--nulldeflist\n");}
            ;

Def:     Specifier Declist SEMICOLON    {$$=A_LocalDef(EM_tokPos, $1, $2);printf("--localdef\n");}
        ;

Declist:         Dec                    {$$=A_DecList($1, NULL);printf("--dec\n");}
                |Dec COMMA Declist      {$$=A_DecList($1, $3);printf("--declist\n");}
                ;

Dec:     Var                 {$$=A_Dec(EM_tokPos, $1, NULL);printf("--vardec\n");}
        |Var ASSIGN Exp      {$$=A_Dec(EM_tokPos, $1, $3);printf("--var=x\n");}

Id:      ID      {$$=S_Symbol($1);}
        ;

// expressions
Exp:     Exp ASSIGN Exp {$$=A_AssignExp(EM_tokPos, $1, $3);printf("--exp\n");}
        |Exp AND Exp    {$$=A_OpExp(EM_tokPos, A_andOP, $1, $3);printf("--exp\n");}
        |Exp OR Exp     {$$=A_OpExp(EM_tokPos, A_orOP, $1, $3);printf("--exp\n");}
        |Exp REL Exp    {$$=A_OpExp(EM_tokPos, REL, $1, $3);printf("--exp\n");}
        |Exp PLUS Exp   {$$=A_OpExp(EM_tokPos, A_plusOp, $1, $3);printf("--exp\n");}
        |Exp MINUS Exp  {$$=A_OpExp(EM_tokPos, A_minusOp, $1, $3);printf("--exp\n");}
        |Exp TIMES Exp  {$$=A_OpExp(EM_tokPos, A_timesOp, $1, $3);printf("--exp\n");}
        |Exp DIVIDE Exp {$$=A_OpExp(EM_tokPos, A_divideOp, $1, $3);printf("--exp\n");}
        |LP Exp RP      {$$=$2;printf("--exp\n");}
        |MINUS Exp      {$$=A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), $2);printf("--exp\n");}
        |NOT Exp        {$$=A_NotExp(EM_tokPos, $2);printf("--exp\n");}
        |Id LP Args RP  {$$=A_FuncExp(EM_tokPos, $1, $3);printf("--exp\n");}
        |Exp LB Exp RB  {$$=A_ArrayExp(EM_tokPos, $1, $3);printf("--exp\n");}
        |Exp DOT Id     {$$=A_StruExp(EM_tokPos, $1, $3);printf("--exp\n");}
        |Id             {$$=A_SimpleExp(EM_tokPos, $1);printf("--exp\n");}
        |INT            {$$=A_IntExp(EM_tokPos, $1);printf("--exp\n");}
        |FLOAT          {$$=A_FloatExp(EM_tokPos, $1);printf("--exp\n");}
        |CHAR           {$$=A_IntExp(EM_tokPos, $1);printf("--exp\n");}

Args:    Exp COMMA Args {$$=A_ExpList($1, $3);printf("--args\n");}
        |Exp            {$$=A_ExpList($1, NULL);printf("--args\n");}

%%
