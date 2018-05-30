//
//  tokens.h
//  C--
//
//  Created by Starblo Hong on 19/05/2018.
//  Copyright © 2018 Starblo Hong. All rights reserved.
//
#ifndef tokens_h
#define tokens_h

//referenced from book
typedef union  {
    int pos;
    int ival;
    float fval;
    char cval;
    string sval;
} YYSTYPE;
extern YYSTYPE yylval;

# define ID 258 // A character string consisting of 52 upper- or lower-case alphabetic, 10 numeric and one underscore characters. Besides, an identifier must not startwith a digit3
# define INT 259    // int: A sequence of digits without spaces
# define FLOAT 260 // float: A real number consisting of digits and one decimal point. The decimal point must be surrounded by at least one digit
# define CHAR 261
# define COMMA 262  // ,
# define SEMICOLON 263  // ;
# define PLUS 264   // +
# define MINUS 265  // -
# define TIMES 266  // *
# define DIVIDE 267 // /
# define ASSIGN 268 // =
# define NOT 269    // !
# define REL 270    // > | < | >= | <= | == | !=
# define AND 271    // &&
# define OR 272 // ||
# define DOT 273    // .
# define T_INT 274   // int
# define T_FLOAT 275   // float
# define T_CHAR 276   // char
# define LP 277 // (
# define RP 278 // )
# define LB 279 // [
# define RB 280 // ]
# define LC 281 // {
# define RC 282 // }
# define IF 283 // if
# define ELSE 284   // else
# define WHILE 285  // while
# define FOR 286    // for
# define BREAK 287  // break
# define CONTINUE 288   // continue
# define STRUCT 289 // struct
# define RETURN 290 // return
# define VOID 291   // void
# define TYPEDEF 292    // typedef
# define CONST 293  // const
# define DO 294 // do
# define ENUM 295   // enum
# define EXTERN 296 // extern
# define DEFAULT 297    // default
# define STATIC 298 // static
# define SWITCH 299 // switch
# define CASE 300   //case
# define UNION 301  // union

#endif  //tokens.h

















