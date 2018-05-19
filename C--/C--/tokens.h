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


# define ID 257 // A character string consisting of 52 upper- or lower-case alphabetic, 10 numeric and one underscore characters. Besides, an identifier must not startwith a digit3
# define INT 258    // int: A sequence of digits without spaces
# define FLOAT 259 // float: A real number consisting of digits and one decimal point. The decimal point must be surrounded by at least one digit
# define COMMA 260  // ,
# define SEMICOLON 261  // ;
# define PLUS 262   // +
# define MINUS 263  // -
# define TIMES 264  // *
# define DIVIDE 265 // /
# define ASSIGN 266 // =
# define NOT 267    // !
# define REL 268    // > | < | >= | <= | == | !=
# define AND 269    // &&
# define OR 270 // ||
# define DOT 271    // .
# define TYPE 272   // int | float
# define LP 273 // (
# define RP 274 // )
# define LB 275 // [
# define RP 276 // ]
# define LC 277 // {
# define RC 278 // }
# define IF 279 // if
# define ELSE 280   // else
# define WHILE 281  // while
# define FOR 282    // for
# define BREAK 283  // break
# define CONTINUE 284   // continue
# define STRUCT 285 // struct
# define RETURN 286 // return
# define VOID 287   // void
# define TYPEDEF 289    // typedef

#endif /* tokens_h */



















