//
//  tokens.h
//  C--
//
//  Created by Starblo Hong on 19/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//

//referenced from book
#ifndef tokens_h
#define tokens_h

typedef union  {
    int pos;
    int ival;
    string sval;
} YYSTYPE;
extern YYSTYPE yylval;

// contain all keywords in C
# define ID 257 // A character string consisting of 52 upper- or lower-case alphabetic, 10 numeric and one underscore characters. Besides, an identifier must not startwith a digit3
# define INT 258    // int: A sequence of digits without spaces
# define FLOAT 259 // float: A real number consisting of digits and one decimal point. The decimal point must be surrounded by at least one digit
# define char 260   // char: character
# define COMMA 261  // ,
# define SEMICOLON 262  // ;
# define PLUS 263   // +
# define MINUS 264  // -
# define MUL 265  // *
# define DIVIDE 266 // /
# define ASSIGN 267 // =
# define NOT 268    // !
# define REL 269    // > | < | >= | <= | == | !=
# define AND 270    // &&
# define OR 271 // ||
# define DOT 272    // .
# define TYPE 273   // int | float | char
# define LP 274 // (
# define RP 275 // )
# define LB 276 // [
# define RB 277 // ]
# define LC 278 // {
# define RC 279 // }
# define IF 280 // if
# define ELSE 281   // else
# define WHILE 282  // while
# define FOR 283    // for
# define BREAK 284  // break
# define CONTINUE 285   // continue
# define STRUCT 286 // struct
# define RETURN 287 // return
# define VOID 288   // void
# define TYPEDEF 289    // typedef
# define CONST 290  // const
# define CASE 292   // case
# define DEFAULT 293    // default
# define DO 294 // do
# define ENUM 295   // enum
# define EXTERN 296 // extern
# define GOTO 297   // goto
# define SIZEOF 298 // sizeof
# define STATIC 299 // static
# define SWITCH 300 // switch
# define UNION 301  // union
# define VOLATILE 302   // volatile
# define DOUBLE 303 // double
# define LONG 304   // long
# define SHORT 305  // short
# define SIGNED 306 // signed
# define UNSIGNED 307   // unsigned

#endif /* tokens_h */



























