#include <stdio.h>
#include "../build/lex.yy.h"
#include "../src/y.tab.h"

int main()
{
	int token;
	
	while(token = yylex()) {
		printf("%s,", TOKEN_STR[token]);
		switch(token) {
			case NUMBER:
			case HEXADECIMAL: printf("%d\n", yylval.ival); break;
			case FLOAT: printf("%.5f\n", yylval.fval); break;
			default: printf("%s\n", yylval.text);
		}
    	if(token == INVALID) break;
	}
  return 0;
}

