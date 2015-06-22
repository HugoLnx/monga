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

void pushVariable(ndVariable *pVar, void *pShared);
void checkVar(ndVar *pVar, void *pShared);
void pushNewScopeVariablesIfBlock(char *evtName, void *pShared);
void popScopeVariablesIfBlock(char *evtName, void *pShared);
void updateLastFunction(ndFunction *pFunc, void *pShared);

struct stState {
  STK_tpScopeStack *pStackVariables;
  VAR_tpReport *pReport;
	ndFunction *pLastFunc;
};

VAR_tpVarResume *resumeVar(ndVar *pVar);
VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations) {

	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  struct stState *pState = NEW(struct stState);
	pState->pStackVariables = STK_create();
  pState->pReport = NEW(VAR_tpReport);
  pState->pReport->type = VAR_RUNNING;

  pEvents->onParameter = pushVariable;
  pEvents->onVariable = pushVariable; 
  pEvents->onVar = checkVar;
	pEvents->onFunction = updateLastFunction;

  pEvents->onNewLevel = pushNewScopeVariablesIfBlock; 
  pEvents->onBackLevel = popScopeVariablesIfBlock; 

  TRA_execute(pDeclarations, pEvents, (void*) pState);
  if (pState->pReport->type == VAR_RUNNING) {
    pState->pReport->type = VAR_ALL_REFERENCED;
  }
  return pState->pReport;
}

void pushVariable(ndVariable *pVar, void *pShared) {
  if(REPORT(pShared)->type != VAR_RUNNING) return;
	STK_addToCurrentScope(STACK(pShared), pVar);
	if(LFUNC(pShared) == NULL) {
		pVar->isGlobal = 1;
	} else {
		pVar->isGlobal = 0;
		LFUNC(pShared)->varsStackSize += 4;
	}
}

void checkVar(ndVar *pVar, void *pShared) {
  if(REPORT(pShared)->type != VAR_RUNNING) return;
  VAR_tpVarResume *pVarResume = resumeVar(pVar);
  ndVariable *pVarDec = (ndVariable*) STK_getCurrentReferenceTo(STACK(pShared), pVarResume->name);
	if (pVarDec == NULL){
    REPORT(pShared)->type = VAR_UNDEFINED;
    REPORT(pShared)->pVarResume = pVarResume;
	} else {
		pVar->pBackDeclaration = NEW(tpVarBackDeclaration);
		pVar->pBackDeclaration->pVarDec = pVarDec;
		pVar->pBackDeclaration->usedDepth = pVarDec->pType->depth - pVarResume->depth;
	}
}

VAR_tpVarResume *resumeVar(ndVar *pVar) {
  VAR_tpVarResume *pResume = NEW(VAR_tpVarResume);
  pResume->pVarTop = pVar;
  pResume->depth = 0;
  while(pVar->varType == VAR_ARRAY) {
    pResume->depth++;
    pVar = (ndVar*) (pVar->value.address.pPointerExp->value.pNode);
  }
  pResume->name = pVar->value.name;
  return pResume;
}

void pushNewScopeVariablesIfBlock(char *evtName, void *pShared) {
  if(REPORT(pShared)->type != VAR_RUNNING) return;
	if (strcmp(evtName, "onBlock") == 0 || strcmp(evtName, "onFunction") == 0) {
		STK_pushNewScope(STACK(pShared));
	}
}

void popScopeVariablesIfBlock(char *evtName, void *pShared) {
  if(REPORT(pShared)->type != VAR_RUNNING) return;
	if (strcmp(evtName, "onBlock") == 0 || strcmp(evtName, "onFunction") == 0) {
		STK_popScope(STACK(pShared));
	}
}

void updateLastFunction(ndFunction *pFunc, void *pShared) {
	pFunc->varsStackSize = 0;
	LFUNC(pShared) = pFunc;
}
