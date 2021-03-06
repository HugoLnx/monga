#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_types.h"
#include "ast_post_traversing.h"
#include "stack.h"
#include "y.tab.h"

#define REPORT(varName) ((TYP_tpReport*) varName)

enum enTokenGroup { GR_NUMBER, GR_FLOAT };

void checkAttribution(ndAttribution *pAttr, void *pShared);
int attributionIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType);
int typeIsCompatibleForAssignment(tpType *pDestType, tpType *pValueType);
enum enTokenGroup tokenGroup(int tk);

void setExpressionTypeAndVerify(ndExpression *pExp, void *pShared);
void setExpressionType(ndExpression *pExp);
void checkVoidFunctionCall(ndExpression *pExp, TYP_tpReport *pReport);
void checkBinExpression(ndExpression *pExp, TYP_tpReport *pReport);
int tokenFromBinOperation(tpType *pType1, tpType *pType2);
void checkArithmeticExp(ndExpression *pExp, TYP_tpReport *pReport);
void setTypeFromVar(ndVar* pVar, tpType *pType);
void setTypeFromNew(ndNew* pNew, tpType *pType);
void setTypeFromFunctionCall(ndFunctionCall *pCall, tpType *pType);

void setArrayVarTypeAndCheckInxExpression(ndVar *pVar, void *pShared);
void setArrayVarType(ndVar *pVar);
void checkVarPointer(ndVar *pVar, TYP_tpReport *pReport);
void checkVarInx(ndVar *pVar, TYP_tpReport *pReport);
int isValidInxType(tpType *pType);

void checkNewInx(ndNew *pNew, void *pShared);
void checkParamsFunctionCall(ndFunctionCall *pCall, void *pShared);

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations) {
	POSTTRA_tpEvents *pEvents = NEW(POSTTRA_tpEvents);
  TYP_tpReport *pReport = NEW(TYP_tpReport);
  pReport->tag = TYP_RUNNING;

  pEvents->onAttribution = checkAttribution;
  pEvents->onExp = setExpressionTypeAndVerify;
  pEvents->onVar = setArrayVarTypeAndCheckInxExpression;
  pEvents->onNew = checkNewInx;
  pEvents->onFunctionCall = checkParamsFunctionCall;

  POSTTRA_execute(pDeclarations, pEvents, (void*) pReport);
  if (pReport->tag == TYP_RUNNING) {
    pReport->tag = TYP_WELL_TYPED;
  }
  return pReport;
}

/* 
 * ATTRIBUTION CHECK
 */

void checkAttribution(ndAttribution *pAttr, void *pShared) {
  if(REPORT(pShared)->tag != TYP_RUNNING) return;
  tpVarBackDeclaration *pVarBack = pAttr->pVar->pBackDeclaration;

	if (!attributionIsCompatible(pVarBack, pAttr->pExp->pType)) {
		REPORT(pShared)->tag = TYP_UNMATCH;
		REPORT(pShared)->pExp = pAttr->pExp;
		REPORT(pShared)->pVarBack = pVarBack;
	}
}

int attributionIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType) {
  tpType *pVarDecType = pVarBack->pVarDec->pType;
	tpType *pVarType = NEW(tpType);
	pVarType->depth = pVarBack->usedDepth;
	pVarType->token = pVarDecType->token;
	return typeIsCompatibleForAssignment(pVarType, pExpType);
}

int typeIsCompatibleForAssignment(tpType *pDestType, tpType *pValueType) {
  return (pDestType->depth == pValueType->depth) && 
    tokenGroup(pDestType->token) == tokenGroup(pValueType->token);
}

enum enTokenGroup tokenGroup(int tk) {
	switch(tk) {
		case TK_INT:
		case NUMBER:
		case HEXADECIMAL:
		case TK_CHAR:
		case CHAR:
			return GR_NUMBER;
		case TK_FLOAT:
		case FLOAT:
			return GR_FLOAT;
	}
}

/* END ATTRIBUTION CHECK */




/* 
 * EXPRESSION TYPING
 */

void setExpressionTypeAndVerify(ndExpression *pExp, void *pShared) {
  if(REPORT(pShared)->tag != TYP_RUNNING) return;
  if(pExp->expTag == EXPND_BIN) {
    checkBinExpression(pExp, REPORT(pShared));
  }
	checkVoidFunctionCall(pExp, REPORT(pShared));

	if(REPORT(pShared)->tag != TYP_RUNNING) return;
	setExpressionType(pExp);
}

void setExpressionType(ndExpression *pExp) {
	tpType *pType = NEW(tpType);
	pType->depth = 0;
	switch(pExp->expTag) {
		case EXP_NUMBER:
			pType->token = NUMBER;
			break;
		case EXP_HEXADECIMAL:
			pType->token = HEXADECIMAL;
			break;
		case EXP_CHAR:
			pType->token = TK_CHAR;
			break;
		case EXP_FLOAT:
			pType->token = TK_FLOAT;
			break;
		case EXP_TEXT:
			pType->token = TK_CHAR;
			pType->depth = 1;
			break;
		case EXPND_VAR:
			setTypeFromVar((ndVar*)pExp->value.pNode, pType);
			break;
		case EXPND_NEW:
			setTypeFromNew(pExp->value.pNode, pType);
			break;
		case EXPND_CALL:
      setTypeFromFunctionCall((ndFunctionCall*)pExp->value.pNode, pType);
			break;
		case EXPND_BIN:
			pType->token = tokenFromBinOperation(
				pExp->value.bin.pExp1->pType, pExp->value.bin.pExp2->pType);
			break;
		case EXPND_EXCLAMATION:
			pType->token = NUMBER;
		case EXPND_MINUS:
			pType->token = ((ndExpression*)pExp->value.pNode)->pType->token;
			break;
	}
  pExp->pType = pType;
}

void checkVoidFunctionCall(ndExpression *pExp, TYP_tpReport *pReport) {
  if(pReport->tag != TYP_RUNNING) return;
  if (pExp->expTag == EXPND_CALL) {
		ndFunctionCall *pCall = (ndFunctionCall*) pExp->value.pNode;
		if (strcmp(pCall->functionName, "printf") == 0
				|| pCall->pDeclaration->pReturnType->token == TK_VOID) {
			pReport->tag = TYP_VOID_FUNCTION_AS_EXP;
			pReport->pExp = pExp;
		}
	}
}

void checkBinExpression(ndExpression *pExp, TYP_tpReport *pReport) {
  switch(pExp->value.bin.expTag) {
    case EXPBIN_PLUS:
    case EXPBIN_MINUS:
    case EXPBIN_ASTERISK:
    case EXPBIN_SLASH:
    case EXPBIN_LESS_EQUAL:
    case EXPBIN_GREATER_EQUAL:
    case EXPBIN_LESS:
    case EXPBIN_GREATER:
    case EXPBIN_AND:
    case EXPBIN_OR:
      checkArithmeticExp(pExp->value.bin.pExp1, pReport);
      checkArithmeticExp(pExp->value.bin.pExp2, pReport);
  }
}

int tokenFromBinOperation(tpType *pType1, tpType *pType2) {
	if(pType1->token == TK_FLOAT || pType2->token == TK_FLOAT) {
		return TK_FLOAT;
	}
	if(pType1->token == NUMBER || pType2->token == NUMBER) {
		return NUMBER;
	}
	return CHAR;
}

void checkArithmeticExp(ndExpression *pExp, TYP_tpReport *pReport) {
  if(pReport->tag != TYP_RUNNING) return;
  if (pExp->pType->depth > 0) {
    pReport->tag = TYP_NO_ARITHMETIC_TYPE;
    pReport->pExp = pExp;
  }
}

void setTypeFromVar(ndVar* pVar, tpType *pType) {
	pType->token = pVar->pBackDeclaration->pVarDec->pType->token;
	pType->depth = pVar->pBackDeclaration->usedDepth;
}

void setTypeFromNew(ndNew* pNew, tpType *pType) {
	pType->token = pNew->pType->token;
	pType->depth = pNew->pType->depth + 1;
}

void setTypeFromFunctionCall(ndFunctionCall *pCall, tpType *pType) {
	ndFunction *pFunc = pCall->pDeclaration;
	pType->token = pFunc->pReturnType->token;
	pType->depth = pFunc->pReturnType->depth;
}


/* END EXPRESSION TYPING */



/* 
 * ARRAY VAR TYPING
 */

void setArrayVarTypeAndCheckInxExpression(ndVar *pVar, void *pShared) {
  if(REPORT(pShared)->tag != TYP_RUNNING) return;
  if(pVar->varTag == IDS_ARRAY) {
		setArrayVarType(pVar);
		checkVarPointer(pVar, REPORT(pShared));
		checkVarInx(pVar, REPORT(pShared));
  }
}

void setArrayVarType(ndVar *pVar) {
	ndExpression *pExp = pVar->value.address.pPointerExp;
	if (pExp->expTag == EXPND_VAR) {
		ndVar *pChildVar = (ndVar*) (pExp->value.pNode);
		pVar->pBase = pChildVar->pBase;
		pVar->pBackDeclaration = NEW(tpVarBackDeclaration);
		pVar->pBackDeclaration->pVarDec = pChildVar->pBackDeclaration->pVarDec;
		pVar->pBackDeclaration->usedDepth = pChildVar->pBackDeclaration->usedDepth - 1;
	}
}

void checkVarPointer(ndVar *pVar, TYP_tpReport *pReport) {
	ndExpression *pExp = pVar->value.address.pPointerExp;
	if (pExp->expTag == EXPND_CALL) {
		pReport->tag = TYP_TODO_FUNCTION_CALL_AS_POINTER;
		pReport->pExp = pExp;
	} else if (pExp->pType->depth == 0) {
		pReport->tag = TYP_NO_POINTER;
		pReport->pExp = pExp;
	}
}

void checkVarInx(ndVar *pVar, TYP_tpReport *pReport) {
	ndExpression *pExp = pVar->value.address.pInxExp;
	if(!isValidInxType(pExp->pType)) {
		pReport->tag = TYP_NO_VALID_ARRAY_INDEX;
		pReport->pExp = pExp;
	}
}

int isValidInxType(tpType *pType) {
  return pType->depth == 0
    && tokenGroup(pType->token) != GR_FLOAT;
}

/* END ARRAY VAR TYPING */



/* 
 * NEW ARRAY TYPING
 */

void checkNewInx(ndNew *pNew, void *pShared) {
  if(REPORT(pShared)->tag != TYP_RUNNING) return;
  if(!isValidInxType(pNew->pExp->pType)) {
    REPORT(pShared)->tag = TYP_NO_VALID_ARRAY_INDEX;
    REPORT(pShared)->pExp = pNew->pExp;
  }
}
/* END NEW ARRAY TYPING */





/* 
 * CHECK PARAMS FUNCTION
 */

void checkParamsFunctionCall(ndFunctionCall *pCall, void *pShared) {
	int argsCount=0, paramsCount=0;
	LIS_tpList *pArgs, *pParams;
	
  if(REPORT(pShared)->tag != TYP_RUNNING || strcmp(pCall->functionName, "printf") == 0) return;
	if (pCall->pExpList != NULL)  {
		pArgs = pCall->pExpList->pList;
		argsCount = LIS_getCount(pArgs);
	}
	if (pCall->pDeclaration->pParameters != NULL) {
		pParams = pCall->pDeclaration->pParameters->pList;
		paramsCount = LIS_getCount(pParams);
	}

	if (argsCount == 0 && paramsCount == 0) return;
	if (argsCount != paramsCount) {
		REPORT(pShared)->tag = TYP_WRONG_PARAMETERS;
		REPORT(pShared)->errorSource.pCall = pCall;
		return;
	}

	LIS_reset(pArgs);
	LIS_reset(pParams);
	while(LIS_goNext(pArgs) && LIS_goNext(pParams)) {
		ndVariable *pParam = (ndVariable*) LIS_getCurrentValue(pParams);
		ndExpression *pArg = (ndExpression*) LIS_getCurrentValue(pArgs);
		
		if(!typeIsCompatibleForAssignment(pParam->pType, pArg->pType)) {
			REPORT(pShared)->tag = TYP_WRONG_PARAMETERS;
			REPORT(pShared)->errorSource.pCall = pCall;
			return;
		}
	}
}
/* END CHECK PARAMS FUNCTION */
