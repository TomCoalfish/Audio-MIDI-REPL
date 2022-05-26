/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_TTL_TURTLE_LEXER_H_INCLUDED
# define YY_TTL_TURTLE_LEXER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int ttldebug;
#endif
/* "%code requires" blocks.  */
#line 60 "turtle_parser.y" /* yacc.c:1909  */


#include <cstdlib>
#include <string>
#include <vector>

#define yyconst const

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

/* Size of default input buffer. */
#ifndef YY_BUF_SIZE
#ifdef __ia64__
/* On IA-64, the buffer size is 16k, not 8k.
 * Moreover, YY_BUF_SIZE is 2*YY_READ_BUF_SIZE in the general case.
 * Ditto for the __ia64__ case accordingly.
 */
#define YY_BUF_SIZE 32768
#else
#define YY_BUF_SIZE 16384
#endif /* __ia64__ */
#endif


struct TurtleParseContext;


#line 80 "turtle_lexer.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BASE = 258,
    SPARQL_BASE = 259,
    PREFIX = 260,
    SPARQL_PREFIX = 261,
    BOOL_TRUE = 262,
    BOOL_FALSE = 263,
    END_OF_FILE = 264,
    DECIMAL = 265,
    DOUBLE = 266,
    INTEGER = 267,
    IRI = 268,
    IDENTIFIER = 269,
    STRING = 270,
    TYPE = 271,
    ANON = 272
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 97 "turtle_parser.y" /* yacc.c:1909  */

  char* strval;

#line 114 "turtle_lexer.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int ttlparse (yyscan_t scanner, TurtleParseContext& context, std::vector<std::tuple<std::string, std::string, std::string>>& triples);
/* "%code provides" blocks.  */
#line 125 "turtle_parser.y" /* yacc.c:1909  */


YY_BUFFER_STATE ttl_scan_string (yyconst char *str, yyscan_t scanner);
YY_BUFFER_STATE ttl_create_buffer(FILE *file, int size, yyscan_t scanner);
void ttl_switch_to_buffer(YY_BUFFER_STATE new_buffer, yyscan_t scanner);
void ttl_delete_buffer (YY_BUFFER_STATE buf, yyscan_t scanner);

int ttllex_init (yyscan_t* scanner);
int ttllex_destroy (yyscan_t scanner);
int ttllex(YYSTYPE * yylval, YYLTYPE * yylloc, yyscan_t scanner);


#line 153 "turtle_lexer.h" /* yacc.c:1909  */

#endif /* !YY_TTL_TURTLE_LEXER_H_INCLUDED  */
