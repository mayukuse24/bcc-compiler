/* A Bison parser, made by GNU Bison 3.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DB = 258,
    CB = 259,
    NUMBER = 260,
    IDENTIFIER = 261,
    TEXT = 262,
    INTEGER = 263,
    STRING = 264,
    LABEL = 265,
    DATATYPE = 266,
    TRUE = 267,
    FALSE = 268,
    ARRAY = 269,
    ETOK = 270,
    AND = 271,
    OR = 272,
    IF = 273,
    WHILE = 274,
    FOR = 275,
    ELSE = 276,
    GOTO = 277,
    GOT = 278,
    PRINT = 279,
    PRINTLN = 280,
    READ = 281,
    GT = 282,
    LT = 283,
    GE = 284,
    LE = 285,
    EQ = 286,
    NE = 287
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "parser.y"

    int num;
    char *id;
    arrnode *anode;
    operandnode *oper;
    varnode *var;
    prntele *pele;
    exprnode *expression;
    declnode *decl;
    declblock *dblock;
    codeblock *cblock;
    stmtnode *stmt;
    boolnode *bexpr;
    relnode *rexpr;
    list<declnode*> *dlist;
    list<stmtnode*> *clist;
    list<varnode*> *varlist;
    list<prntele*> *ele;
    prognode *prog;

#line 111 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
