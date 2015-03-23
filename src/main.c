#include <stdio.h>
#include "../build/src/lex.yy.h"
#include "../src/y.tab.h"

int main()
{
	int token;
	while(token = yylex()) {
		printf("=>%d (%d|%s)", token, yylval.ival, yylval.text);
	}
  return 0;
}

