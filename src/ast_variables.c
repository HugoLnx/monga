#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_variables.h"
#include "ast_traversing.h"
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
  VAR_tpReport *pReport;
	ndFunction *pLastFunc;
	tpList *pFunctions;
	int ignoreNextBlockOpening;
};

VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  struct stState *pState = NEW(struct stState);
	pState->pStackVariables = STK_create();
  pState->pReport = NEW(VAR_tpReport);
  pState->pFunctions = createList();
  pState->pReport->tag = VAR_RUNNING;
	pState->ignoreNextBlockOpening = 0;

  pEvents->onParameter = pushVariable;
  pEvents->onVariable = pushVariable; 
  pEvents->onVar = checkVarName;
	pEvents->onFunction = pushFunctionDeclaration;
	pEvents->onFunctionCall = referenceFunctionBackToDeclaration;

  pEvents->onNewLevel = pushNewScopeVariablesIfBlock; 
  pEvents->onBackLevel = popScopeVariablesIfBlock; 

  TRA_execute(pDeclarations, pEvents, (void*) pState);
  if (pState->pReport->tag == VAR_RUNNING) {
    pState->pReport->tag = VAR_ALL_REFERENCED;
  }
  return pState->pReport;
}

void pushVariable(ndVariable *pVar, void *pShared) {
  if(REPORT(pShared)->tag != VAR_RUNNING) return;
	ndVariable *pFoundVar = STK_getReferenceInCurrentScope(STACK(pShared), pVar->name);
	if(pFoundVar != NULL) {
		REPORT(pShared)->tag = VAR_SCOPE_VARIABLE_OVERRIDING;
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
  if(REPORT(pShared)->tag != VAR_RUNNING) return;
	if (pVar->varTag == VAR_ID) {
		pVar->pBase = pVar;
		ndVariable *pVarDec = (ndVariable*) STK_getCurrentReferenceTo(STACK(pShared), pVar->value.name);
		if (pVarDec == NULL){
			REPORT(pShared)->tag = VAR_UNDEFINED;
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
  if(REPORT(pShared)->tag != VAR_RUNNING) return;
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
  if(REPORT(pShared)->tag != VAR_RUNNING) return;
	if (strcmp(evtName, "onBlock") == 0) {
		STK_popScope(STACK(pShared));
	}
}

void pushFunctionDeclaration(ndFunction *pFunc, void *pShared) {
  if(REPORT(pShared)->tag != VAR_RUNNING) return;

	checkFunctionAlreadyExist(pFunc, pShared);

  if(REPORT(pShared)->tag != VAR_RUNNING) return;

	pFunc->varsStackSize = 0;
	LFUNC(pShared) = pFunc;
	addLast(FUNCS(pShared), (void*) pFunc);
}

void checkFunctionAlreadyExist(ndFunction *pFunc, void *pShared) {
	resetList(FUNCS(pShared));
	while(goNext(FUNCS(pShared))) {
		ndFunction *pTmpFunc;
		pTmpFunc = (ndFunction*) getCurrentValue(FUNCS(pShared));
		if (strcmp(pTmpFunc->name, pFunc->name) == 0) {
			REPORT(pShared)->tag = VAR_FUNCTION_OVERRIDING;
			REPORT(pShared)->errorSource.pFunction = pFunc;
			break;
		}
	}
}

void referenceFunctionBackToDeclaration(ndFunctionCall *pCall, void *pShared) {
	ndFunction *pFunc = NULL;

	resetList(FUNCS(pShared));
	while(goNext(FUNCS(pShared))) {
		ndFunction *pTmpFunc;
		pTmpFunc = (ndFunction*) getCurrentValue(FUNCS(pShared));
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
		REPORT(pShared)->tag = VAR_UNDEFINED_FUNCTION;
		REPORT(pShared)->errorSource.pFunctionCall = pCall;
	}
}
