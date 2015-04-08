#include <stdio.h>
#include "y.tab.i"

int main()
{
	yyparse();
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

