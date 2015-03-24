%{
#include <stdio.h>
#include "../src/y.tab.h"
%}
%%
\"[^"\n]+\" yylval.text = yytext; return TEXT;
\"[^"\n]+$ yylval.text = yytext; return INVALID;
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
[0-9]+[a-zA-Z_]+ yylval.text = yytext; return INVALID;
[ \t]+
\n
%%
yywrap()
{
	return 1;
}
