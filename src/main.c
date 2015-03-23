#include <stdio.h>
#include "../build/lex.yy.h"
#include "../src/y.tab.h"

int main()
{
	int token;
	while(token = yylex()) {
		printf("=>%d (%d)\n", token, yylval.ival);
	}
  return 0;
}

