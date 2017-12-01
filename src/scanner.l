%{

#include <string.h>
#include "ast.h"
#include "parser.hpp"

extern "C" {
 int yylex();
}

%}
			
number		[0-9][0-9]*
identifier	[a-zA-Z][a-zA-Z0-9]*

%%

"declblock"  {printf("DECLBLOCK ");return(DB);}
"codeblock"  {printf("CODEBLOCK ");return(CB);}

"for" {printf("FOR ");return(FOR);}
"if"  {printf("IF ");return(IF);}
"else"  {printf("ELSE ");return(ELSE);}
"while" {printf("WHILE ");return(WHILE);}
"L"{number}  {printf("LABEL ");return(LABEL);}
"goto"  {printf("GOTO ");return(GOTO);}
"got"  {printf("GOT ");return(GOT);}

"print"  {printf("PRINT ");return(PRINT);}
"println"  {printf("PRINTLN ");return(PRINTLN);}
"read"  {printf("READ ");return(READ);}

"||"  {printf("OR ");return OR;}
"&&"  {printf("AND ");return AND;}

"int"	{printf("INTEGER ");return(DATATYPE);}
"string"  {printf("STRING ");return(DATATYPE);}

"true"  {printf("TRUE ");return(TRUE);}
"false"  {printf("FALSE ");return(FALSE);}

\"(\\.|[^"])*\"   { printf("TEXT ");yylval.id = strdup(yytext);
                    (yylval.id)++;
                    yylval.id[strlen(yylval.id)-1] = 0;
                    return(TEXT);
                    //CAUTION: Buffer overflow 
                   }

{identifier}  {printf("IDENTI "); yylval.id = strdup(yytext) ;return(IDENTIFIER);}
{number}  {printf("NUM ");yylval.num=atoi(yytext);return(NUMBER);}

"+"  {printf("+ ");return '+';}
"-"  {printf("- ");return '-';}
"*"  {printf("* ");return '*';}
"="  {printf("= ");return '=';}

"{"  {printf("{ ");return '{';}
"}"  {printf("} ");return '}';}

">"  {printf("> ");return GT;}
">=" {printf(">= ");return GE;}
"<"  {printf("< ");return LT;}
"<=" {printf("<= ");return LE;}
"==" {printf("== ");return EQ;}
"!=" {printf("!= ");return NE;}

","  {printf(", ");return ',';}
":"  {printf(": ");return ':';}
";"  {printf("; ");return ';';}
"["  {printf("[ ");return '[';}
"]"  {printf("] ");return ']';}

[ \t]  { /* Do nothing */ }

.		                { printf("Unexpected token : %s\n", yytext);
                                  return ETOK;
		                }
%%
