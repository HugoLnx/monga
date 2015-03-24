%{
#include <stdio.h>
#include "../src/y.tab.h"
%}
%%
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
\"[^"]+\" yylval.text = yytext; return TEXT;
[ \t]+
\n
%%
yywrap()
{
	return 1;
}
