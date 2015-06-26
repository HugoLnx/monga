#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_ids.h"
#include "ast_pre_traversing.h"
#include "stack.h"
#include "y.tab.h"

#define REPORT(varName) ((struct stState*) varName)->pReport
#define STACK(varName) ((struct stState*) varName)->pStackVariables 
#define LFUNC(varName) ((struct stState*) varName)->pLastFunc 
#define FUNCS(varName) ((struct stState*) varName)->pFunctions 
#define IGNBLOCK(varName) ((struct stState*) varName)->ignoreNextBlockOpening

void pushVariable(ndVariable *pVar, void *pShared);
void checkVarName(ndVar *pVar, void *pShared);
void pushNewScopeVariablesIfBlock(char *evtName, void *pShared);
void popScopeVariablesIfBlock(char *evtName, void *pShared);

void pushFunctionDeclaration(ndFunction *pFunc, void *pShared);
void checkFunctionAlreadyExist(ndFunction *pFunc, void *pShared);
void referenceFunctionBackToDeclaration(ndFunctionCall *pCall, void *pShared);

struct stState {
  STK_tpScopeStack *pStackVariables;
  IDS_tpReport *pReport;
	ndFunction *pLastFunc;
	LIS_tpList *pFunctions;
	int ignoreNextBlockOpening;
};

IDS_tpReport *IDS_checkVariablesScopes(ndDeclarations *pDeclarations) {
	PRETRA_tpEvents *pEvents = NEW(PRETRA_tpEvents);
  struct stState *pState = NEW(struct stState);
	pState->pStackVariables = STK_create();
  pState->pReport = NEW(IDS_tpReport);
  pState->pFunctions = LIS_create();
  pState->pReport->tag = IDS_RUNNING;
	pState->ignoreNextBlockOpening = 0;

  pEvents->onParameter = pushVariable;
  pEvents->onVariable = pushVariable; 
  pEvents->onVar = checkVarName;
	pEvents->onFunction = pushFunctionDeclaration;
	pEvents->onFunctionCall = referenceFunctionBackToDeclaration;

  pEvents->onNewLevel = pushNewScopeVariablesIfBlock; 
  pEvents->onBackLevel = popScopeVariablesIfBlock; 

  PRETRA_execute(pDeclarations, pEvents, (void*) pState);
  if (pState->pReport->tag == IDS_RUNNING) {
    pState->pReport->tag = IDS_ALL_REFERENCED;
  }
  return pState->pReport;
}

void pushVariable(ndVariable *pVar, void *pShared) {
  if(REPORT(pShared)->tag != IDS_RUNNING) return;
	ndVariable *pFoundVar = STK_getReferenceInCurrentScope(STACK(pShared), pVar->name);
	if(pFoundVar != NULL) {
		REPORT(pShared)->tag = IDS_SCOPE_VARIABLE_OVERRIDING;
		REPORT(pShared)->errorSource.pVarDec = pFoundVar;
		return;
	}

	STK_addToCurrentScope(STACK(pShared), pVar);
	if(LFUNC(pShared) == NULL) {
		pVar->isGlobal = 1;
	} else {
		pVar->isGlobal = 0;
		LFUNC(pShared)->varsStackSize += 4;
	}
}

void checkVarName(ndVar *pVar, void *pShared) {
  if(REPORT(pShared)->tag != IDS_RUNNING) return;
	if (pVar->varTag == IDS_ID) {
		pVar->pBase = pVar;
		ndVariable *pVarDec = (ndVariable*) STK_getCurrentReferenceTo(STACK(pShared), pVar->value.name);
		if (pVarDec == NULL){
			REPORT(pShared)->tag = IDS_UNDEFINED;
			REPORT(pShared)->errorSource.pVar = pVar;
		} else {
			pVar->pBackDeclaration = NEW(tpVarBackDeclaration);
			pVar->pBackDeclaration->pVarDec = pVarDec;
			pVar->pBackDeclaration->usedDepth = pVarDec->pType->depth;
		}
	}
}

void pushNewScopeVariablesIfBlock(char *evtName, void *pShared) {
	int isBlock = strcmp(evtName, "onBlock") == 0;
	int isFunction = strcmp(evtName, "onFunction") == 0;
  if(REPORT(pShared)->tag != IDS_RUNNING) return;
	if (isBlock && IGNBLOCK(pShared) == 1) {
		IGNBLOCK(pShared) = 0;
	}
	else if (isBlock) {
		STK_pushNewScope(STACK(pShared));
	}
	else if (isFunction) {
		IGNBLOCK(pShared) = 1;
		STK_pushNewScope(STACK(pShared));
	}
}

void popScopeVariablesIfBlock(char *evtName, void *pShared) {
  if(REPORT(pShared)->tag != IDS_RUNNING) return;
	if (strcmp(evtName, "onBlock") == 0) {
		STK_popScope(STACK(pShared));
	}
}

void pushFunctionDeclaration(ndFunction *pFunc, void *pShared) {
  if(REPORT(pShared)->tag != IDS_RUNNING) return;

	checkFunctionAlreadyExist(pFunc, pShared);

  if(REPORT(pShared)->tag != IDS_RUNNING) return;

	pFunc->varsStackSize = 0;
	LFUNC(pShared) = pFunc;
	LIS_addLast(FUNCS(pShared), (void*) pFunc);
}

void checkFunctionAlreadyExist(ndFunction *pFunc, void *pShared) {
	LIS_reset(FUNCS(pShared));
	while(LIS_goNext(FUNCS(pShared))) {
		ndFunction *pTmpFunc;
		pTmpFunc = (ndFunction*) LIS_getCurrentValue(FUNCS(pShared));
		if (strcmp(pTmpFunc->name, pFunc->name) == 0) {
			REPORT(pShared)->tag = IDS_FUNCTION_OVERRIDING;
			REPORT(pShared)->errorSource.pFunction = pFunc;
			break;
		}
	}
}

void referenceFunctionBackToDeclaration(ndFunctionCall *pCall, void *pShared) {
	ndFunction *pFunc = NULL;

	LIS_reset(FUNCS(pShared));
	while(LIS_goNext(FUNCS(pShared))) {
		ndFunction *pTmpFunc;
		pTmpFunc = (ndFunction*) LIS_getCurrentValue(FUNCS(pShared));
		if (strcmp(pTmpFunc->name, pCall->functionName) == 0) {
			pFunc = pTmpFunc;
			break;
		}
	}

	if (pFunc != NULL) {
		pCall->pDeclaration = pFunc;
	} else if (strcmp(pCall->functionName, "printf") == 0) {
		pCall->pDeclaration = NULL;
	} else {
		REPORT(pShared)->tag = IDS_UNDEFINED_FUNCTION;
		REPORT(pShared)->errorSource.pFunctionCall = pCall;
	}
}
