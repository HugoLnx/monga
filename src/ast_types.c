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
TYP_tpExpResume *resumeExp(ndExpression *pExp);
int typeIsCompatible(tpVarBackDeclaration *pVarBack, tpType *pExpType);
enum enTokenGroup tokenGroup(int tk);
void setTypeOfVar(ndVar* pVar, tpType *pType);
void setTypeOfNew(ndNew* pNew, tpType *pType);

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  TYP_tpReport *pReport = NEW(TYP_tpReport);
  pReport->type = TYP_RUNNING;

  pEvents->onAttribution = checkAttribution;

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
  int varDepth = pVarType->depth - pVarBack->usedDepth;
	return (varDepth == pExpType->depth) && (
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
	pType->depth = pVar->pBackDeclaration->pVarDec->pType->depth - pVar->pBackDeclaration->usedDepth;
}

void setTypeOfNew(ndNew* pNew, tpType *pType) {
	pType->token = pNew->pType->token;
	pType->depth = pNew->pType->depth + 1;
}
