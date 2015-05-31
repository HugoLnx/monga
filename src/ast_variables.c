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
	STK_tpScopeStack *pStackVariables = STK_create();

  pEvents->onParameter = pushVariable;
  pEvents->onVariable = pushVariable; 
  pEvents->onVar = checkVar;

  pEvents->onNewLevel = pushNewScopeVariablesIfBlock; 
  pEvents->onBackLevel = popScopeVariablesIfBlock; 

  TRA_execute(pDeclarations, pEvents, (void*) &pStackVariables);
}

void pushVariable(ndVariable *pVar, void *pShared) {
	STK_addToCurrentScope((STK_tpScopeStack*) pShared, pVar);
}

void checkVar(ndVar *pVar, void *pShared) {
	if (STK_getCurrentReferenceTo((STK_tpScopeStack*) pShared, pVar->value.name) == NULL){
		// TODO erro variável não encontrada
	} else {
		// Connect to variable declaration
	}
}

//} else if(true) { // TODO Check types

void pushNewScopeVariablesIfBlock(char *evtName, void *pShared) {
	if (strcmp(evtName, "onBlock") != 0) {
		STK_pushNewScope((STK_tpScopeStack*) pShared);
	}
}

void popScopeVariablesIfBlock(char *evtName, void *pShared) {
	if (strcmp(evtName, "onBlock") != 0) {
		STK_popScope((STK_tpScopeStack*) pShared);
	}
}
