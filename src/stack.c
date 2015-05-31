#include <stdio.h>
#include "stack.h"
#include <stdlib.h>
#include <string.h>

struct STK_stScopeStack {
  struct stNode *pStackVariables;
  struct stNode *pStackCountVariables;
};

typedef struct stNode {
  void *pValue;
  struct stNode *pPrevious;
} tpNode;


STK_tpScopeStack *STK_create() {
  STK_tpScopeStack *pStack = (STK_tpScopeStack*) malloc(sizeof(STK_tpScopeStack));
  
  if (pStack == NULL){
  	return;
  }
  
  pStack->pStackVariables = NULL;
  pStack->pStackCountVariables = NULL;
  return pStack;
}

void STK_pushNewScope(STK_tpScopeStack *pStack){
  tpNode *pNodeStackCount = (tpNode*) malloc(sizeof(tpNode));
  pNodeStackCount->pValue = malloc(sizeof(int));

  if (pNodeStackCount == NULL){
  	return;
  }

  *(int*)pNodeStackCount->pValue = 0;

  if(pStack->pStackCountVariables == NULL){
    pNodeStackCount->pPrevious = NULL;
    pStack->pStackCountVariables = pNodeStackCount;
  }
  else{
    pNodeStackCount->pPrevious = pStack->pStackCountVariables;
    pStack->pStackCountVariables = pNodeStackCount;
  }
};

void STK_popScope(STK_tpScopeStack *pStack){
  int totalVariablesInScope = *(int*) pStack->pStackCountVariables->pValue;
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

void STK_addToCurrentScope(STK_tpScopeStack *pStack, ndVariable *pVar){
  tpNode *pNodeStackVariable = (tpNode*) malloc(sizeof(tpNode));
  
  if (pNodeStackVariable == NULL){
  	return;
  }

  (*(int*) pStack->pStackCountVariables->pValue)++;
  pNodeStackVariable->pValue = (void*) pVar;

  if(pStack->pStackVariables == NULL){
		STK_pushNewScope(pStack);
    pNodeStackVariable->pPrevious = NULL;
    pStack->pStackVariables = pNodeStackVariable;
  }
  else{
    pNodeStackVariable->pPrevious = pStack->pStackVariables;
    pStack->pStackVariables = pNodeStackVariable;
  }
}

ndVariable *STK_getCurrentReferenceTo(STK_tpScopeStack *pStack, char *name){
  int i;
  tpNode *auxNode = NULL;
  ndVariable *currentVariable;
  int totalVariables = 0; 

  if (pStack->pStackCountVariables == NULL){
  	return NULL;
  }
  totalVariables = *(int*) pStack->pStackCountVariables->pValue;
  auxNode = pStack->pStackCountVariables;

  while(auxNode->pPrevious != NULL){
  	totalVariables += *(int*) auxNode->pValue;
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
