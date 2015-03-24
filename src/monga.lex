%{
#include <stdio.h>
#include "../src/y.tab.h"
%}

/* macros */
ID_CHAR [a-zA-Z_]

%%
\"[^"\n]+\" yylval.text = yytext; return TEXT; // strings
\"[^"\n]+$ yylval.text = yytext; return INVALID;

[0-9]+ yylval.ival = atoi(yytext); return NUMBER; // numbers
[0-9]+{ID_CHAR}+ yylval.text = yytext; return INVALID;

[ \t\n]+ // ignore white space
%%
yywrap()
{
	return 1;
}
