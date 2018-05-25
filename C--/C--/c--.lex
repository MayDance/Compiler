%{
#include <string.h>
#incldue <tokens.h>
#define BUFSIZE 65535

int charPos = 1;
char strbuf[BUFSIZE + 1];
char *strptr = NULL;
unsigned int strlen = 0;

int yywrap(void) {
charPos = 1;
return 1;
}

void init(void) {
strlen = 0;
strbuf[0] = '\0'
}

void append(char *str) {
if (strLen + strlen(str) < BUFSIZE) {
strcat(strbuf, str);
strLen += strlen(str);
}
else
EM_error(EM_tokPos, "Out of range");
}

void adjust(void) {
EM_tokPos = charPos;
charPos += yyleng;
}
%}

digit [0-9]
digit_ [1-9]
letter [a-zA-Z]
integer ({digit_}{digit}*) | 0
sfloat (({digit}+\.{digit}*)|({digit}*\.{digit}+))[Ee][+-]?{digit}+
float {sfloat}|((({digit_}{digit}*)|0)\.{digit}+)
id {letter_}({letter_}|{digit})*
relop >|<|>=|<=|==|!=
comments /\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/

%%
[ \t]+   {adjust(); continue;}  //blank
\n     {adjust(); EM_newline(); continue;}
(\n|\r\n)   {adjust(); EM_newline(); continue;}
comments   {adjust(); continue;}   //comments
integer       {adjust(); yylval.ival=atoi(yytext); return INT;}
float   {adjust(); yylval.sval=yytext; return float;}
id    {adjust(); yylval.sval=yytext; return ID;}

{integer}   {adjust(); return INT;}
{float} {adjust(); return FLOAT;}
"char"  {adjust(); return CAHR;}
"if"    {adjust(); return IF;}
"else"  {adjust(); return ELSE;}
"while" {adjust(); return WHILE;}
"for"   {adjust(); return FOR;}
"break" {adjust(); return BREAK;}
"continue"   {adjust(); return CONTINUE;}
"struct"    {adjust(); return STRUCT;}
"return"    {adjust(); return RETURN;}
"void"  {adjust(); return VOID;}
"typedef"   {adjust(); return typedef;}
"const" {adjust(); return CONST;}
"case"  {adjust(); return CASE;}
"default"   {adjust(); return DEFAULT;}
"do"    {adjust(); return DO;}
"enum"  {adjust(); return ENUM;}
"extern"    {adjust(); return EXTREN;}
"goto"  {adjust(); return GOTO;}
"sizeof"    {adjust(); return SIZROF;}
"static"    {adjust(); return STATIC;}
"switch"    {adjust(); return SWITCH;}
"union" {adjust(); return UNION;}
"volatile"  {adjust(); return VOLATILE;}
"double"    {adjust(); return DOUBLE;}
"long"  {adjust(); return LONG;}
"short" {adjust(); return SHORT;}
"signed"    {adjust(); return SIGNED;}
"unsigned"  {adjust(); return UNSIGNED;}
integer|float|char  {adjust(); return TYPE;}

"," {adjust(); return COMMA;}
";" {adjust(); return SEMICOLON;}
"+" {adjust(); return PLUS;}
"-" {adjust(); return MINUS;}
"*" {adjust(); return MUL;}
"/" {adjust(); return DIVIDE;}
"=" {adjust(); return ASSIGN;}
"!" {adjust(); return NOT;}
relop   {adjust(); return REL;}
"&&"    {adjust(); return AND;}
"||"    {adjust(); return OR;}
"." {adjust(); return DOT;}
"{" {adjust(); return LP}
"}" {adjust(); return RP;}
"[" {adjust(); return LB;}
"]" {adjust(); return RB;}
"{" {adjust(); return LC;}
"}" {adjust(); return RC;}

%%

