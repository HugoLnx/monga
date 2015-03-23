%{
#include <stdio.h>
#include "../src/y.tab.h"
%}
%%
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
%%
yywrap()
{
	return 1;
}
