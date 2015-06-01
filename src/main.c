#include <stdio.h>
#include "ast_tree.h"
#include "ast_variables.h"
#include "ast_types.h"

extern ndDeclarations *pDeclarations;

int main()
{
	yyparse();
  VAR_tpReport *pVarReport = VAR_checkVariablesScopes(pDeclarations);
  if (pVarReport->type == VAR_ALL_REFERENCED) {
		TYP_tpReport *pTypReport = TYP_checkMatchingTypes(pDeclarations);
		if(pTypReport->type == TYP_WELL_TYPED) {
			printTree(pDeclarations);
		} else {
		  printf("Error: Unmatching type in attribution of '%s' to type '%d'\n",
				pTypReport->pVarBack->pVarDec->name, pTypReport->pExpResume->pType->token);
		}
  } else {
    printf("Error: Undefined variable '%s'\n", pVarReport->pVarResume->name);
  }
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

