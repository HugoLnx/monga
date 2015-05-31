#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_variables.h"
#include "ast_traversing.h"
#include "stack.h"

#define REPORT(varName) ((struct stState*) varName)->pReport
#define STACK(varName) ((struct stState*) varName)->pStackVariables 

void pushVariable(ndVariable *pVar, void *pShared);
void checkAttribution(ndAttribution *pAttr, void *pShared);
void pushNewScopeVariablesIfBlock(char *evtName, void *pShared);
void popScopeVariablesIfBlock(char *evtName, void *pShared);

struct stState {
  STK_tpScopeStack *pStackVariables;
  VAR_tpReport *pReport;
};

VAR_tpVarResume *resumeVar(ndVar *pVar);
VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations) {

	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  struct stState *pState = NEW(struct stState);
	pState->pStackVariables = STK_create();
  pState->pReport = NEW(VAR_tpReport);
  pState->pReport->type = RUNNING;

  pEvents->onParameter = pushVariable;
  pEvents->onVariable = pushVariable; 
  pEvents->onAttribution = checkAttribution;

  pEvents->onNewLevel = pushNewScopeVariablesIfBlock; 
  pEvents->onBackLevel = popScopeVariablesIfBlock; 

  TRA_execute(pDeclarations, pEvents, (void*) pState);
  if (pState->pReport->type == RUNNING) {
    pState->pReport->type = CORRECT;
  }
  return pState->pReport;
}

void pushVariable(ndVariable *pVar, void *pShared) {
  if(REPORT(pShared)->type != RUNNING) return;
	STK_addToCurrentScope(STACK(pShared), pVar);
}

void checkAttribution(ndAttribution *pAttr, void *pShared) {
  if(REPORT(pShared)->type != RUNNING) return;
  VAR_tpVarResume *pVarResume = resumeVar(pAttr->pVar);
  //tpExpResume *pExpResume = resumeExp(pAttr->pVar);
  ndVariable *pVarDec = (ndVariable*) STK_getCurrentReferenceTo(STACK(pShared), pVarResume->name);
	if (pVarDec == NULL){
    REPORT(pShared)->type = UNDEFINED_VARIABLE;
    REPORT(pShared)->pVarResume = pVarResume;
	} else {
		// Connect to variable declaration
	}
}

VAR_tpVarResume *resumeVar(ndVar *pVar) {
  VAR_tpVarResume *pResume = (VAR_tpVarResume*) malloc(sizeof(VAR_tpVarResume));
  pResume->pVarTop = pVar;
  pResume->depth = 0;
  while(pVar->varType == VAR_ARRAY) {
    pResume->depth++;
    pVar = (ndVar*) (pVar->value.address.pPointerExp->value.pNode);
  }
  pResume->name = pVar->value.name;
  return pResume;
}

//} else if(true) { // TODO Check types

void pushNewScopeVariablesIfBlock(char *evtName, void *pShared) {
  if(REPORT(pShared)->type != RUNNING) return;
	if (strcmp(evtName, "onBlock") == 0) {
		STK_pushNewScope(STACK(pShared));
	}
}

void popScopeVariablesIfBlock(char *evtName, void *pShared) {
  if(REPORT(pShared)->type != RUNNING) return;
	if (strcmp(evtName, "onBlock") == 0) {
		STK_popScope(STACK(pShared));
	}
}
