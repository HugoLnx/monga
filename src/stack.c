#include <stdio.h>
#include "stack.h"
#include <stdlib.h>
#include <string.h>


typedef struct stNode {
  void *pValue;
  struct stNode *pPrevious;
} tpNode;


tpScopeStack *createStack() {
  tpScopeStack *pStack = (tpScopeStack*) malloc(sizeof(tpScopeStack));
  
  if (pStack == NULL){
  	return;
  }
  
  pStack->pStackVariables = NULL;
  pStack->pStackCountVariables = NULL;
  return pStack;
}

void pushNewScope(tpScopeStack *pStack){
  tpNode *pNodeStackCount = (tpNode*) malloc(sizeof(tpNode));

  if (pNodeStackCount == NULL){
  	return;
  }

  pNodeStackCount->pValue = (void*) 0;

  if(pStack->pStackCountVariables == NULL){
    pNodeStackCount->pPrevious = NULL;
    pStack->pStackCountVariables = pNodeStackCount;
  }
  else{
    pNodeStackCount->pPrevious = pStack->pStackCountVariables;
    pStack->pStackCountVariables = pNodeStackCount;
  }
};

void popScope(tpScopeStack *pStack){
  int totalVariablesInScope = (int) pStack->pStackCountVariables->pValue;
  int i;
  tpNode *auxNode;

  for (i=0; i < totalVariablesInScope; i++){
	auxNode = pStack->pStackVariables;

    pStack->pStackVariables = pStack->pStackVariables->pPrevious;
    free(auxNode);
  }
  
  auxNode = pStack->pStackCountVariables;

  pStack->pStackCountVariables = pStack->pStackCountVariables->pPrevious;
  free(auxNode);
}

void addToCurrentScope(tpScopeStack *pStack, ndVariable *pVar){

  tpNode *pNodeStackVariable = (tpNode*) malloc(sizeof(tpNode));
  
  if (pNodeStackVariable == NULL){
  	return;
  }

  pStack->pStackCountVariables->pValue = (void*) ((int) pStack->pStackCountVariables->pValue + 1);
  pNodeStackVariable->pValue = pVar;

  if(pStack->pStackVariables == NULL){
    pNodeStackVariable->pPrevious = NULL;
    pStack->pStackVariables = pNodeStackVariable;
  }
  else{
    pNodeStackVariable->pPrevious = pStack->pStackVariables;
    pStack->pStackVariables = pNodeStackVariable;
  }
}

ndVariable *getCurrentReferenceTo(tpScopeStack *pStack, char *name){
  int i;
  tpNode *auxNode = NULL;
  ndVariable *currentVariable;
  int totalVariables = 0; 

  if (pStack->pStackCountVariables == NULL){
  	return NULL;
  }
  totalVariables = (int) pStack->pStackCountVariables->pValue;
  auxNode = pStack->pStackCountVariables;

  while(auxNode->pPrevious != NULL){
  	totalVariables += (int) auxNode->pValue;
  	auxNode = auxNode->pPrevious;
  }

  auxNode = NULL;
  
  for(i=0; i<totalVariables; i++){
  	if(auxNode == NULL){
      auxNode = pStack->pStackVariables;
  	}
  	else{
      auxNode = auxNode->pPrevious;
  	}

  	currentVariable = (ndVariable*) auxNode->pValue;
  	if(strcmp(name, currentVariable->name) == 0){
  		return currentVariable;
  	}
  }

  return NULL;
}