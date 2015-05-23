%{
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "utils.h"
#include "y.tab.h"
%}

/* macros */
ID_CHAR [a-zA-Z_]
CHAR_CHAR ([^'\n]|\\'|\\n|\\t|\\\\)
STR_CHAR ([^\"\n]|\\\\|\\\"|\\n|\\t)
STR_NON_ESCAPED_CHAR ([^\"\n\\]|\\\\|\\\"|\\n|\\t)
HEX_CHAR [0-9a-fA-F]
COMMENT ([^*]|\*+[^/*])*

%%
\/\*{COMMENT}\*+\/ yylval.text = strDup(yytext); return COMMENT; // comments
(\/\*|\*\/) yylval.text = strDup(yytext); return INVALID;

\"\\\" yylval.text = strDup(yytext); return INVALID;
\"{STR_NON_ESCAPED_CHAR}*\\[^nt\\\"]{STR_NON_ESCAPED_CHAR}*\" yylval.text = strDup(yytext); return INVALID;  // strings
\"{STR_CHAR}*\" yylval.text = strDup(yytext); return TEXT;

'\\' yylval.text = strDup(yytext); return INVALID;
'{CHAR_CHAR}?' yylval.text = strDup(yytext); return CHAR;  // chars
'\\[a-zA-Z0-9]' yylval.text = strDup(yytext); return INVALID;
'{CHAR_CHAR}+' yylval.text = strDup(yytext); return INVALID;

0x{HEX_CHAR}+ yylval.ival = strtol(yytext, NULL, 16); return HEXADECIMAL; // hexadecimals
0x{HEX_CHAR}+{ID_CHAR}+ yylval.text = strDup(yytext); return INVALID;

0+[0-9]+ yylval.text = strDup(yytext); return INVALID; // numbers
[0-9]+ yylval.ival = atoi(yytext); return NUMBER;
[0-9]+{ID_CHAR}+ yylval.text = strDup(yytext); return INVALID;

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*|[0-9]+)([eE][-+]?[0-9]+)?f? yylval.fval = atof(yytext); return FLOAT; // floats
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*|[0-9]+)[eE][-+]f? yylval.text = strDup(yytext); return INVALID;
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*|[0-9]+)[eE]([+-][\n ]|[+-][^0-9\n ]+) yylval.text = strDup(yytext); return INVALID;
\.[^0-9] yylval.text = strDup(yytext); return INVALID;

\[   yylval.text = ""; return TK_SQUARE_BRACKET_OPEN;
\]   yylval.text = ""; return TK_SQUARE_BRACKET_CLOSE;
\(   yylval.text = ""; return TK_PARENTHESES_OPEN;
\)   yylval.text = ""; return TK_PARENTHESES_CLOSE;
\;   yylval.text = ""; return TK_SEMICOLON;
\+   yylval.text = ""; return TK_PLUS;
\-   yylval.text = ""; return TK_MINUS;
\*   yylval.text = ""; return TK_ASTERISK;
\/   yylval.text = ""; return TK_SLASH;
[{]  yylval.text = ""; return TK_CURLY_BRACE_OPEN;
[}]  yylval.text = ""; return TK_CURLY_BRACE_CLOSE;
[!]  yylval.text = ""; return TK_EXCLAMATION_MARK;
==   yylval.text = ""; return TK_DOUBLE_EQUAL;
!=   yylval.text = ""; return TK_BANG_EQUAL;
=    yylval.text = ""; return TK_ONE_EQUAL;
(<=) yylval.text = ""; return TK_LESS_EQUAL;
(>=) yylval.text = ""; return TK_GREATER_EQUAL;
[<]  yylval.text = ""; return TK_LESS;
[>]  yylval.text = ""; return TK_GREATER;
\|\| yylval.text = ""; return TK_OR;
(&&) yylval.text = ""; return TK_AND;
[,]  yylval.text = ""; return TK_COMMA;

int yylval.text = ""; 	 return TK_INT;
char yylval.text = "";   return TK_CHAR;
float yylval.text = "";  return TK_FLOAT;
void yylval.text = "";   return TK_VOID;
if yylval.text = "";     return TK_IF;
else yylval.text = "";   return TK_ELSE;
while yylval.text = "";  return TK_WHILE;
new yylval.text = "";    return TK_NEW;
return yylval.text = ""; return TK_RETURN;

[a-zA-Z_][a-zA-Z_0-9]* yylval.text = strDup(yytext); return TK_ID;

[ \t\n]+ // ignore white space

. yylval.text = strDup(yytext); return INVALID;
%%
yywrap()
{
	return 1;
}
