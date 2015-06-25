#include <stdio.h>
#include "error_reporter.h"

int ERR_printErrorsOnVarReport(VAR_tpReport *pReport) {
  if (pReport->type == VAR_ALL_REFERENCED) {
    return 0;
  } else {
    printf("Error: Undefined variable '%s'\n", pReport->pVar->pBase->value.name);
    return 1;
  }
}

int ERR_printErrorsOnTypesReport(TYP_tpReport *pReport) {
  switch(pReport->type) {
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
  }

  if(pReport->type == TYP_WELL_TYPED) {
    return 0;
  } else {
    return 1;
  }
}
