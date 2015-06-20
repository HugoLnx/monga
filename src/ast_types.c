#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_types.h"
#include "ast_traversing.h"
#include "stack.h"
#include "y.tab.h"

#define REPORT(varName) ((TYP_tpReport*) varName)

enum enTokenGroup { GR_NUMBER, GR_FLOAT, GR_CHAR };

void checkAttribution(ndAttribution *pAttr, void *pShared);
void checkExpression(ndExpression *pExp, void *pShared);
void checkBinExpression(ndExpression *pExp, TYP_tpReport *pReport);
void checkArithmeticExp(TYP_tpExpResume *pResume, TYP_tpReport *pReport);
TYP_tpExpResume *resumeExp(ndExpression *pExp);
void checkVarInx(ndVar *pVar, void *pShared);
void checkNewInx(ndNew *pNew, void *pShared);
int isValidInxType(tpType *pType);
int typeIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType);
enum enTokenGroup tokenGroup(int tk);
void setTypeOfVar(ndVar* pVar, tpType *pType);
void setTypeOfNew(ndNew* pNew, tpType *pType);

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  TYP_tpReport *pReport = NEW(TYP_tpReport);
  pReport->type = TYP_RUNNING;

  pEvents->onAttribution = checkAttribution;
  pEvents->onExp = checkExpression;
  pEvents->onVar = checkVarInx;
  pEvents->onNew = checkNewInx;

  TRA_execute(pDeclarations, pEvents, (void*) pReport);
  if (pReport->type == TYP_RUNNING) {
    pReport->type = TYP_WELL_TYPED;
  }
  return pReport;
}

void checkAttribution(ndAttribution *pAttr, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  tpVarBackDeclaration *pVarBack = pAttr->pVar->pBackDeclaration;

	TYP_tpExpResume *pExpResume = resumeExp(pAttr->pExp);
	if (!typeIsCompatible(pVarBack, pExpResume->pType)) {
		REPORT(pShared)->type = TYP_UNMATCH;
		REPORT(pShared)->pExpResume = pExpResume;
		REPORT(pShared)->pVarBack = pVarBack;
	}
}

void checkExpression(ndExpression *pExp, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  if(pExp->expType == EXPND_BIN) {
    checkBinExpression(pExp, REPORT(pShared));
  }
}

void checkBinExpression(ndExpression *pExp, TYP_tpReport *pReport) {
  TYP_tpExpResume *pResume1 = resumeExp(pExp->value.bin.pExp1);
  TYP_tpExpResume *pResume2 = resumeExp(pExp->value.bin.pExp2);
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
      checkArithmeticExp(pResume1, pReport);
      checkArithmeticExp(pResume2, pReport);
  }
}

void checkArithmeticExp(TYP_tpExpResume *pResume, TYP_tpReport *pReport) {
  if(pReport->type != TYP_RUNNING) return;
  if (pResume->pType->depth > 0) {
    pReport->type = TYP_NO_ARITHMETIC_TYPE;
    pReport->pExpResume = pResume;
  }
}

void checkVarInx(ndVar *pVar, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  if(pVar->varType == VAR_ARRAY) {
    ndExpression *pExp = pVar->value.address.pInxExp;
    TYP_tpExpResume *pResume = resumeExp(pExp);
    if(!isValidInxType(pResume->pType)) {
      REPORT(pShared)->type = TYP_NO_VALID_ARRAY_INDEX;
      REPORT(pShared)->pExpResume = pResume;
    }
  }
}

void checkNewInx(ndNew *pNew, void *pShared) {
  if(REPORT(pShared)->type != TYP_RUNNING) return;
  TYP_tpExpResume *pResume = resumeExp(pNew->pExp);
  if(!isValidInxType(pResume->pType)) {
    REPORT(pShared)->type = TYP_NO_VALID_ARRAY_INDEX;
    REPORT(pShared)->pExpResume = pResume;
  }
}

int isValidInxType(tpType *pType) {
  return pType->depth == 0
    && tokenGroup(pType->token) != GR_FLOAT;
}

TYP_tpExpResume *resumeExp(ndExpression *pExp) {
  TYP_tpExpResume *pResume = NEW(TYP_tpExpResume);
	pResume->pType = NEW(tpType);
	pResume->pType->depth = 0;
  pResume->pExpTop = pExp;
	switch(pExp->expType) {
		case EXP_NUMBER:
			pResume->pType->token = NUMBER;
			break;
		case EXP_HEXADECIMAL:
			pResume->pType->token = HEXADECIMAL;
			break;
		case EXP_CHAR:
			pResume->pType->token = TK_CHAR;
			break;
		case EXP_FLOAT:
			pResume->pType->token = TK_FLOAT;
			break;
		case EXP_TEXT:
			pResume->pType->token = TK_CHAR;
			pResume->pType->depth = 1;
			break;
		case EXPND_VAR:
			setTypeOfVar((ndVar*)pExp->value.pNode, pResume->pType);
			break;
		case EXPND_NEW:
			setTypeOfNew(pExp->value.pNode, pResume->pType);
			break;
		case EXPND_BIN:
		case EXPND_EXCLAMATION:
		case EXPND_MINUS:
			pResume->pType->token = NUMBER;
			break;
	}
	return pResume;
}

int typeIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType) {
  tpType *pVarType = pVarBack->pVarDec->pType;
	return (pVarBack->usedDepth == pExpType->depth) && (
    (tokenGroup(pVarType->token) == tokenGroup(pExpType->token)) ||
    (tokenGroup(pVarType->token) == GR_NUMBER && tokenGroup(pExpType->token) == GR_FLOAT) ||
    (tokenGroup(pVarType->token) == GR_FLOAT && tokenGroup(pExpType->token) == GR_NUMBER)
  );
}

enum enTokenGroup tokenGroup(int tk) {
	switch(tk) {
		case TK_INT:
		case NUMBER:
		case HEXADECIMAL:
			return GR_NUMBER;
		case TK_CHAR:
		case CHAR:
			return GR_CHAR;
		case TK_FLOAT:
		case FLOAT:
			return GR_FLOAT;
	}
}

void setTypeOfVar(ndVar* pVar, tpType *pType) {
	pType->token = pVar->pBackDeclaration->pVarDec->pType->token;
	pType->depth = pVar->pBackDeclaration->usedDepth;
}

void setTypeOfNew(ndNew* pNew, tpType *pType) {
	pType->token = pNew->pType->token;
	pType->depth = pNew->pType->depth + 1;
}
