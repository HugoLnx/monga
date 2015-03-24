%{
#include <stdio.h>
#include "../src/y.tab.h"
%}
%%
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
\"[^"\n]+\" yylval.text = yytext; return TEXT;
\"[^"\n]+$ yylval.text = yytext; return INVALID;
[ \t]+
\n
%%
yywrap()
{
	return 1;
}
