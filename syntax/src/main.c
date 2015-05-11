#include <stdio.h>

int main()
{
	yyparse();
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

