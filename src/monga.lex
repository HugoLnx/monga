%{
#include <stdio.h>
#include "/vagrant/monga/src/y.tab.h"
%}
%%
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
%%
yywrap()
{
	return 1;
}
