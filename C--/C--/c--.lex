%{
#include <string.h>
#include <stdlib.h>
#include "errormsg.h"
#include "absyn.h"
#include "tokens.h"
#define BUFSIZE 65535

int charPos = 1;
char strbuf[BUFSIZE + 1];
char *strptr = NULL;
unsigned int strLen = 0;

int yywrap(void) {
    printf("yywrap\n");
    charPos = 1;
    return 1;
}

void init(void) {
    printf("init\n");
    strLen = 0;
    strbuf[0] = '\0';
}

void append(char *str) {
    printf("append\n");
    if (strLen + strlen(str) < BUFSIZE) {
        strcat(strbuf, str);
        strLen += strlen(str);
    }
    else
        EM_error(EM_tokPos, "Out of range");
}

void adjust(void) {
    printf("adjust\n");
    EM_tokPos = charPos;
    charPos += yyleng;
}
%}

digit [0-9]
digit_ [1-9]
letter [a-zA-Z]
letter_ [a-zA-Z_]
integer (({digit_}{digit}*)|0)
sfloat (({digit}+\.{digit}*)|({digit}*\.{digit}+))[Ee][+-]?{digit}+
floatt {sfloat}|((({digit_}{digit}*)|0)\.{digit}+)
id {letter_}({letter_}|{digit})*
relop >|<|>=|<=|==|!=
comments "/*"[^*]*[*]+([^*/][^*]*[*]+)*[*]*"/"

%%
[ \t]+   {adjust();printf("==blank\n"); continue;}  //blank
\n     {adjust(); printf("==newline\n"); EM_newline(); continue;}
(\n|\r\n)   {adjust(); printf("==new\n"); EM_newline(); continue;}
{comments}   {adjust(); printf("==comments\n"); continue;}   //comments
"//"    {adjust(); printf("==//comments\n"); EM_newline(); continue;} //comments

"if"    {adjust(); printf("==if\n"); return IF;}
"else"  {adjust(); printf("==else\n"); return ELSE;}
"while" {adjust(); printf("==while\n"); return WHILE;}
"for"   {adjust(); printf("==for\n"); return FOR;}
"break" {adjust(); printf("==break\n"); return BREAK;}
"continue"   {adjust(); printf("==continue\n"); return CONTINUE;}
"struct"    {adjust(); printf("==struct\n"); return STRUCT;}
"return"    {adjust(); printf("==return\n"); return RETURN;}
"void"  {adjust(); printf("==void\n"); return VOID;}
"typedef"   {adjust(); printf("==typedef\n"); return TYPEDEF;}
"const" {adjust(); printf("==const\n"); return CONST;}
"case"  {adjust(); printf("==case\n"); return CASE;}
"default"   {adjust(); printf("==default\n"); return DEFAULT;}
"do"    {adjust(); printf("==do\n"); return DO;}
"enum"  {adjust(); printf("==enum\n"); return ENUM;}
"extern"    {adjust(); printf("==extern\n"); return EXTERN;}
"static"    {adjust(); printf("==static\n"); return STATIC;}
"switch"    {adjust(); printf("==switch\n"); return SWITCH;}
"union" {adjust(); printf("==union\n"); return UNION;}
"int"   {adjust(); printf("==tint\n"); return T_INT;}
"float"   {adjust(); printf("==tfloat\n"); return T_FLOAT;}
"char"   {adjust(); printf("==tchar\n"); return T_CHAR;}

{integer} {adjust(); printf("==int:%s\n", yytext); yylval.ival=atoi(yytext); return INT;}
{floatt}   {adjust(); printf("==float:%s\n", yytext); yylval.fval=atof(yytext); return FLOAT;}
{id}    {adjust(); printf("==id:%s\n", yytext); yylval.sval=yytext; return ID;}

"," {adjust(); printf("==,\n"); return COMMA;}
";" {adjust(); printf("==;\n"); return SEMICOLON;}
"+" {adjust(); printf("==+\n"); return PLUS;}
"-" {adjust(); printf("==-\n"); return MINUS;}
"*" {adjust(); printf("==*\n"); return TIMES;}
"/" {adjust(); printf("==/\n"); return DIVIDE;}
"=" {adjust(); printf("===\n"); return ASSIGN;}
"!" {adjust(); printf("==!\n"); return NOT;}
{relop}   {adjust(); printf("==relop\n"); return REL;}
"&&"    {adjust(); printf("==&&\n"); return AND;}
"||"    {adjust(); printf("==||\n"); return OR;}
"." {adjust(); printf("==.\n"); return DOT;}
"(" {adjust(); printf("==(\n"); return LP;}
")" {adjust(); printf("==)\n"); return RP;}
"[" {adjust(); printf("==[\n"); return LB;}
"]" {adjust(); printf("==]\n"); return RB;}
"{" {adjust(); printf("=={\n"); return LC;}
"}" {adjust(); printf("==}\n"); return RC;}
%%
