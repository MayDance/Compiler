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
# define TYPE 274   // int | float | char
# define LP 275 // (
# define RP 276 // )
# define LB 277 // [
# define RB 278 // ]
# define LC 279 // {
# define RC 280 // }
# define IF 281 // if
# define ELSE 282   // else
# define WHILE 283  // while
# define FOR 284    // for
# define BREAK 285  // break
# define CONTINUE 286   // continue
# define STRUCT 287 // struct
# define RETURN 288 // return
# define VOID 289   // void
# define TYPEDEF 290    // typedef
# define CONST 291  // const
# define DO 292 // do
# define ENUM 293   // enum
# define EXTERN 294 // extern
# define DEFAULT 295    // default
# define STATIC 296 // static
# define SWITCH 297 // switch
# define CASE 298   //case
# define UNION 299  // union

#endif  //tokens.h

















