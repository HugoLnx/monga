#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_variables.h"
#include "ast_traversing.h"
#include "stack.h"
#include "y.tab.h"

#define REPORT(varName) ((TYP_tpReport*) varName)

void checkAttribution(ndAttribution *pAttr, void *pShared);

TYP_tpVarResume *resumeVar(ndVar *pVar);
TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations) {

	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  pReport = NEW(TYP_tpReport);
  pReport->type = RUNNING;

  pEvents->onAttribution = checkAttribution;

  TRA_execute(pDeclarations, pEvents, (void*) pReport);
  if (pState->pReport->type == RUNNING) {
    pState->pReport->type = CORRECT;
  }
  return pState->pReport;
}

void setTypeOfVar(ndVar* pVar, tpType *pType) {
}

TYP_tpExpResume *resumeExp(ndExpression *pExp) {
  TYP_tpExpResume *pResume = (TYP_tpExpResume*) malloc(sizeof(TYP_tpExpResume));
	pResume->pType = (tpType*) malloc(sizeof(tpType));
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
		// TODO
		//case EXPND_TYP:
		//	setTypeOfVar((ndVar*)pExp->value.pNode, pResume->pType);
		//	break;
		//case EXPND_NEW:
		//	setTypeOfNew(pExp->value.pNode, pResume->pType);
		//	break;
		case EXPND_BIN:
		case EXPND_EXCLAMATION:
		case EXPND_MINUS:
			pResume->pType->token = NUMBER;
			break;
	}
	return pResume;
}

enum enTokenGroup { GR_NUMBER, GR_FLOAT, GR_CHAR };

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

int typeIsCompatible(tpType *pVarType, tpType *pExpType) {
	return tokenGroup(pVarType->token) == tokenGroup(pExpType->token)
		&& pVarType->depth == pExpType->depth;
}

void checkAttribution(ndAttribution *pAttr, void *pShared) {
  if(REPORT(pShared)->type != RUNNING) return;
  TYP_tpVarResume *pVarResume = resumeVar(pAttr->pVar);
  ndVariable *pVarDec = pAttr->pVar->pVarDec;

	TYP_tpExpResume *pExpResume = resumeExp(pAttr->pExp);
	if (!typeIsCompatible(pVarDec->pType, pExpResume->pType)) {
		REPORT(pShared)->type = UNMATCH_TYPE;
		REPORT(pShared)->pVarResume = pVarResume;
		REPORT(pShared)->pExpResume = pExpResume;
	}
}
