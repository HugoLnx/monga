#include <stdio.h>

int main()
{
	yyparse();
  printDeclarations();
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

