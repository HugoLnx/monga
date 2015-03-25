%{
#include <stdio.h>
#include "../src/y.tab.h"
%}

/* macros */
ID_CHAR [a-zA-Z_]
STR_CHAR ([^\"\n]|\\\")
STR_NON_ESCAPED_CHAR ([^\"\n\\]|\\\")

%%
\"{STR_NON_ESCAPED_CHAR}*\\[a-zA-Z0-9]{STR_NON_ESCAPED_CHAR}*\" yylval.text = yytext; return INVALID;  // strings
\"{STR_CHAR}*\" yylval.text = yytext; return TEXT;
\"{STR_CHAR}*$ yylval.text = yytext; return INVALID;

-?0+[0-9]+ yylval.text = yytext; return INVALID; // numbers
-0+ yylval.text = yytext; return INVALID;
-?[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
-?[0-9]+{ID_CHAR}+ yylval.text = yytext; return INVALID;

[ \t\n]+ // ignore white space
%%
yywrap()
{
	return 1;
}
