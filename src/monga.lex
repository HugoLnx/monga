%{
#include <stdio.h>
#include "../src/y.tab.h"
%}
%%
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
\"[^"]+\" yylval.text = yytext; return TEXT;
\"[^"]+$ yylval.text = yytext; return INVALID;
[ \t]+
\n
%%
yywrap()
{
	return 1;
}
