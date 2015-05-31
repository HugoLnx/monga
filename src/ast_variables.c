#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_variables.h"
#include "ast_traversing.h"
#include "stack.h"

void pushVariable(ndVariable *pVar, void *pShared);
void checkVar(ndVar *pVar, void *pShared);
void pushNewScopeVariablesIfBlock(char *evtName, void *pShared);
void popScopeVariablesIfBlock(char *evtName, void *pShared);

VAR_tpReport VAR_checkVariablesScopes(ndDeclarations *pDeclarations) {

	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
	tpScopeStack *pStackVariables = createStack();

  pEvents->onParameter = pushVariable;
  pEvents->onVariable = pushVariable; 
  pEvents->onVar = checkVar;

  pEvents->onNewLevel = pushNewScopeVariablesIfBlock; 
  pEvents->onBackLevel = popScopeVariablesIfBlock; 

  TRA_execute(pDeclarations, pEvents, (void*) &pStackVariables);
}

void pushVariable(ndVariable *pVar, void *pShared) {
	addToCurrentScope((tpScopeStack*) pShared, pVar);
}

void checkVar(ndVar *pVar, void *pShared) {
	if (getCurrentReferenceTo((tpScopeStack*) pShared, pVar->value.name) == NULL){
		// TODO erro variável não encontrada
	} else {
		// Connect to variable declaration
	}
}

//} else if(true) { // TODO Check types

void pushNewScopeVariablesIfBlock(char *evtName, void *pShared) {
	if (strcmp(evtName, "onBlock") != 0) {
		pushNewScope((tpScopeStack*) pShared);
	}
}

void popScopeVariablesIfBlock(char *evtName, void *pShared) {
	if (strcmp(evtName, "onBlock") != 0) {
		popScope((tpScopeStack*) pShared);
	}
}
