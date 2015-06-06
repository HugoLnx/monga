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
    switch(pTypReport->type) {
    case TYP_WELL_TYPED:
			printTree(pDeclarations);
      break;
    case TYP_UNMATCH:
		  printf("Error: Unmatching type in attribution of '%s' to type '%d'\n",
				pTypReport->pVarBack->pVarDec->name, pTypReport->pExpResume->pType->token);
      break;
    case TYP_NO_ARITHMETIC_TYPE:
		  printf("Error: Expression in aritmetic operation have invalid type %d,%d\n",
				pTypReport->pExpResume->pType->token, pTypReport->pExpResume->pType->depth);
      break;
    case TYP_NO_VALID_ARRAY_INDEX:
		  printf("Error: Expression as array index have invalid type %d,%d\n",
				pTypReport->pExpResume->pType->token, pTypReport->pExpResume->pType->depth);
      break;
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

