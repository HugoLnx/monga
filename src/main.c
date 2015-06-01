#include <stdio.h>
#include "ast_tree.h"
#include "ast_variables.h"

extern ndDeclarations *pDeclarations;

int main()
{
	yyparse();
  VAR_tpReport *pReport = VAR_checkVariablesScopes(pDeclarations);
  if (pReport->type == VAR_ALL_REFERENCED) {
    printTree(pDeclarations);
  } else if(pReport->type == VAR_UNDEFINED) {
    printf("Error: Undefined variable '%s'\n", pReport->pVarResume->name);
  //} else if(pReport->type == UNMATCH_TYPE) {
  //  printf("Error: Unmatching type in attribution of '%s' to type '%d'\n", pReport->pVarResume->name, pReport->pExpResume->pType->token);
  }
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

