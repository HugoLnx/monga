#include <stdio.h>
#include "../build/lex.yy.h"
#include "../src/y.tab.h"

int main()
{
	int token;
	while(token = yylex()) {
		printf("%s,", TOKEN_STR[token]);
		switch(token) {
			case NUMBER: printf("%d\n", yylval.ival); break;
			default: printf("%s\n", yylval.text);
		}
	}
  return 0;
}

