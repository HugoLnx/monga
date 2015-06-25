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
void setExpressionTypeAndVerify(ndExpression *pExp, void *pShared);
void setExpressionType(ndExpression *pExp);
void checkBinExpression(ndExpression *pExp, TYP_tpReport *pReport);
void checkArithmeticExp(ndExpression *pExp, TYP_tpReport *pReport);
void checkVarInx(ndVar *pVar, void *pShared);
void checkNewInx(ndNew *pNew, void *pShared);
int isValidInxType(tpType *pType);
int typeIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType);
enum enTokenGroup tokenGroup(int tk);
void setTypeFromVar(ndVar* pVar, tpType *pType);
void setTypeFromNew(ndNew* pNew, tpType *pType);

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations) {
	POSTTRA_tpEvents *pEvents = NEW(POSTTRA_tpEvents);
  TYP_tpReport *pReport = NEW(TYP_tpReport);
  pReport->type = TYP_RUNNING;

  pEvents->onAttribution = checkAttribution;
  pEvents->onExp = setExpressionTypeAndVerify;
  pEvents->onVar = checkVarInx;
  pEvents->onNew = checkNewInx;

  POSTTRA_execute(pDeclarations, pEvents, (void*) pReport);
  if (pReport->type == TYP_RUNNING) {
    pReport->type = TYP_WELL_TYPED;
  }
  return pReport;
}

void checkAttribution(ndAttribution *pAttr, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  tpVarBackDeclaration *pVarBack = pAttr->pVar->pBackDeclaration;

	if (!typeIsCompatible(pVarBack, pAttr->pExp->pType)) {
		REPORT(pShared)->type = TYP_UNMATCH;
		REPORT(pShared)->pExp = pAttr->pExp;
		REPORT(pShared)->pVarBack = pVarBack;
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

void setExpressionType(ndExpression *pExp) {
	tpType *pType = NEW(tpType);
	pType->depth = 0;
	switch(pExp->expType) {
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
      pType->token = NUMBER;
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

void setExpressionTypeAndVerify(ndExpression *pExp, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  if(pExp->expType == EXPND_BIN) {
    checkBinExpression(pExp, REPORT(pShared));
  }
	setExpressionType(pExp);
}

void checkBinExpression(ndExpression *pExp, TYP_tpReport *pReport) {
  switch(pExp->value.bin.expType) {
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

void checkArithmeticExp(ndExpression *pExp, TYP_tpReport *pReport) {
  if(pReport->type != TYP_RUNNING) return;
  if (pExp->pType->depth > 0) {
    pReport->type = TYP_NO_ARITHMETIC_TYPE;
    pReport->pExp = pExp;
  }
}

void checkVarInx(ndVar *pVar, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  if(pVar->varType == VAR_ARRAY) {
    ndExpression *pExp = pVar->value.address.pInxExp;
    if(!isValidInxType(pExp->pType)) {
      REPORT(pShared)->type = TYP_NO_VALID_ARRAY_INDEX;
      REPORT(pShared)->pExp = pExp;
    }
  }
}

void checkNewInx(ndNew *pNew, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  if(!isValidInxType(pNew->pExp->pType)) {
    REPORT(pShared)->type = TYP_NO_VALID_ARRAY_INDEX;
    REPORT(pShared)->pExp = pNew->pExp;
  }
}

int isValidInxType(tpType *pType) {
  return pType->depth == 0
    && tokenGroup(pType->token) != GR_FLOAT;
}

int typeIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType) {
  tpType *pVarType = pVarBack->pVarDec->pType;
	return (pVarBack->usedDepth == pExpType->depth) && 
    tokenGroup(pVarType->token) == tokenGroup(pExpType->token);
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

void setTypeFromVar(ndVar* pVar, tpType *pType) {
	pType->token = pVar->pBackDeclaration->pVarDec->pType->token;
	pType->depth = pVar->pBackDeclaration->usedDepth;
}

void setTypeFromNew(ndNew* pNew, tpType *pType) {
	pType->token = pNew->pType->token;
	pType->depth = pNew->pType->depth + 1;
}
