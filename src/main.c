#include <stdio.h>

int main()
{
	yyparse();
  printProgram();
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

