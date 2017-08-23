%{

#include "parser.tab.h"

%}

number		[0-9][0-9]*
identifier	[a-zA-Z][a-zA-Z0-9]*

%%

"int"				{printf("INTEGER");return(KEYWORD);}
{identifier}"["{number}"]"	{printf("ARRAY");return(ARRAY);}
{identifier}			{printf("IDENTI");return(IDENTIFIER);}
{number}			{printf("NUM");return(NUMBER);}
"+"				{printf("ADD");return '+';}
"-"				{printf("SUB");return '-';}
"*"				{printf("MUL");return '*';}

%%
