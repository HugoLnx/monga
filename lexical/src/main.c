#include <stdio.h>
#include "lex.yy.h"
#include "main.h"

int main()
{
	int token;
	
	while(token = yylex()) {
		printf("%s,", TOKEN_STR[token]);
		switch(token) {
			case NUMBER:
			case HEXADECIMAL: printf("%lld\n", yylval.ival); break;
			case FLOAT: printf("%.5f\n", yylval.fval); break;
			default: printf("%s\n", yylval.text);
		}
    	if(token == INVALID) break;
	}
  return 0;
}

