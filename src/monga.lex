%{
#include <stdio.h>
#include "../src/y.tab.h"
%}

/* macros */
ID_CHAR [a-zA-Z_]
CHAR_CHAR ([^'\n]|\\'|\\n|\\t|\\\\)
STR_CHAR ([^\"\n]|\\\\|\\\"|\\n|\\t)
STR_NON_ESCAPED_CHAR ([^\"\n\\]|\\\\|\\\"|\\n|\\t)
HEX_CHAR [0-9a-fA-F]
COMMENT ([^*]|\*+[^/*])*

%%
\/\*{COMMENT}\*+\/ yylval.text = yytext; return COMMENT; // comments
(\/\*|\*\/) yylval.text = yytext; return INVALID;

\"{STR_NON_ESCAPED_CHAR}*\\[a-zA-Z0-9]{STR_NON_ESCAPED_CHAR}*\" yylval.text = yytext; return INVALID;  // strings
\"{STR_CHAR}*\" yylval.text = yytext; return TEXT;
\" yylval.text = yytext; return INVALID;

'{CHAR_CHAR}?' yylval.text = yytext; return CHAR;  // chars
'\\[a-zA-Z0-9]' yylval.text = yytext; return INVALID;
'{CHAR_CHAR}+' yylval.text = yytext; return INVALID;
' yylval.text = yytext; return INVALID;

-0x0* yylval.text = yytext; return INVALID; // hexadecimals
-?0x{HEX_CHAR}+ yylval.ival = strtol(yytext, NULL, 16); return HEXADECIMAL;
-?0x{HEX_CHAR}+{ID_CHAR}+ yylval.text = yytext; return INVALID;

-?0+[0-9]+ yylval.text = yytext; return INVALID; // numbers
-0+ yylval.text = yytext; return INVALID;
-?[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
-?[0-9]+{ID_CHAR}+ yylval.text = yytext; return INVALID;

int yylval.text = "";    return TK_INT;
char yylval.text = "";   return TK_CHAR;
float yylval.text = "";  return TK_FLOAT;
void yylval.text = "";   return TK_VOID;
if yylval.text = "";     return TK_IF;
else yylval.text = "";   return TK_ELSE;
while yylval.text = "";  return TK_WHILE;
new yylval.text = "";    return TK_NEW;
return yylval.text = ""; return TK_RETURN;
[a-zA-Z_][a-zA-Z_0-9]* yylval.text = yytext; return TK_ID;

[ \t\n]+ // ignore white space
%%
yywrap()
{
	return 1;
}
