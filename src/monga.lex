%{
#include <stdio.h>
#include "../src/y.tab.h"
%}

/* macros */
ID_CHAR [a-zA-Z_]
CHAR_CHAR ([^'\n]|\\'|\\n|\\t|\\\\)
STR_CHAR ([^\"\n]|\\\")
STR_NON_ESCAPED_CHAR ([^\"\n\\]|\\\")
HEX_CHAR [0-9a-fA-F]

%%
\"{STR_NON_ESCAPED_CHAR}*\\[a-zA-Z0-9]{STR_NON_ESCAPED_CHAR}*\" yylval.text = yytext; return INVALID;  // strings
\"{STR_CHAR}*\" yylval.text = yytext; return TEXT;
\"{STR_CHAR}*$ yylval.text = yytext; return INVALID;

'{CHAR_CHAR}?' yylval.text = yytext; return CHAR;  // chars
'\\[a-zA-Z0-9]' yylval.text = yytext; return INVALID;
'{CHAR_CHAR}+' yylval.text = yytext; return INVALID;
'{CHAR_CHAR}?[^'] yylval.text = yytext; return INVALID;

-0x0* yylval.text = yytext; return INVALID; // hexadecimals
-?0x{HEX_CHAR}+ yylval.ival = strtol(yytext, NULL, 16); return HEXADECIMAL;
-?0x{HEX_CHAR}+{ID_CHAR}+ yylval.text = yytext; return INVALID;

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
