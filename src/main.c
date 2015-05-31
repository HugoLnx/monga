#include <stdio.h>
#include "ast_tree.h"
#include "ast_variables.h"

extern ndDeclarations *pDeclarations;

int main()
{
	yyparse();
  VAR_tpReport *pReport = VAR_checkVariablesScopes(pDeclarations);
  if (pReport->type == CORRECT) {
    printTree(pDeclarations);
  } else if(pReport->type == UNDEFINED_VARIABLE) {
    printf("Error: Undefined variable '%s'\n", pReport->pVarResume->name);
  }
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

