#include <stdio.h>
#include "error_reporter.h"
#include "ast_tree.h"

int ERR_printErrorsOnVarReport(VAR_tpReport *pReport) {
  switch(pReport->tag) {
  case VAR_UNDEFINED:
    printf("Error: Undefined variable '%s'\n", pReport->errorSource.pVar->pBase->value.name);
		break;
  case VAR_UNDEFINED_FUNCTION:
    printf("Error: Undefined function '%s'\n", pReport->errorSource.pFunctionCall->functionName);
		break;
  }

	return pReport->tag != VAR_ALL_REFERENCED;
}

int ERR_printErrorsOnTypesReport(TYP_tpReport *pReport) {
	ndFunctionCall *pCall;
  switch(pReport->tag) {
  case TYP_UNMATCH:
    printf("Error: Unmatching type in attribution of '%s' to type '%d'\n",
      pReport->pVarBack->pVarDec->name, pReport->pExp->pType->token);
    break;
  case TYP_NO_ARITHMETIC_TYPE:
    printf("Error: Expression in aritmetic operation have invalid type %d,%d\n",
      pReport->pExp->pType->token, pReport->pExp->pType->depth);
    break;
  case TYP_NO_VALID_ARRAY_INDEX:
    printf("Error: Expression as array index have invalid type %d,%d\n",
      pReport->pExp->pType->token, pReport->pExp->pType->depth);
    break;
	case TYP_VOID_FUNCTION_AS_EXP:
		pCall = (ndFunctionCall*) pReport->pExp->value.pNode;
    printf("Error: Void function call as expression %s\n",
				pCall->functionName);
    break;
  }

  return pReport->tag != TYP_WELL_TYPED;
}
