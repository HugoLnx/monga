#include <stdio.h>
#include "ast_tree.h"

extern ndDeclarations *pDeclarations;

int main()
{
	yyparse();
  printTree(pDeclarations);
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

