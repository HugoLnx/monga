#ifndef AST_TREE_C
#define AST_TREE_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./list.h"
#include "./ast_tree.h"
#define NEW(type) (type*) malloc(sizeof(type))
extern ndProgram *pProgram;

typedef struct stDeclaration {
	enum enDeclaration decType;
	void *pNode;
} tpDeclaration;

typedef struct stType {
	int token;
	int depth;
} tpType;

typedef struct stProgramNode {
	tpDeclaration *pDec;
} ndProgram;

typedef struct stBlockNode {
} ndBlock;

typedef struct stFunctionNode {
	char *name;
	ndBlock *pBlock;
	ndParameters *pParameters;
	tpType *pReturnType;
} ndFunction;

typedef struct stParametersNode {
	tpList *pList;
} ndParameters;

typedef struct stParameterNode {
	tpType *pType;
	char *name;
} ndParameter;

typedef struct stVariablesNode {
	tpList *pList;
	tpType *pType;
} ndVariables;

typedef struct stVariableNode {
	char *name;
} ndVariable;
 
char *strDup(char *str) {
	char* dup = (char*) malloc(sizeof(char)*(strlen(str)+1));
	strcpy(dup, str);
	return dup;
}

void incrDepth(tpType *pType) {
  pType->depth += 1;
}

ndProgram *createProgramNode(tpType *pType, tpDeclaration *pDec) {
	pProgram = NEW(ndProgram);
	if (pDec->decType == DEC_FUNCTION) {
		ndFunction *pFunc = (ndFunction*) pDec->pNode;
		pFunc->pReturnType = pType;
	} else {
		ndVariables *pVars = (ndVariables*) pDec->pNode;
		pVars->pType = pType;
  }

	pProgram->pDec = pDec;
	return pProgram;
}

ndFunction *createFunctionNode(char *name, ndParameters *pParams, ndBlock *pBlock) {
	ndFunction *pFunc = NEW(ndFunction);
	pFunc->name = strDup(name);
	pFunc->pParameters = pParams;
	pFunc->pBlock = pBlock;
	return pFunc;
}

ndBlock *createBlockNode() {
	return NEW(ndBlock);
}

tpType *newType(int token, int depth) {
	tpType *pType = NEW(tpType);
	pType->token = token;
	pType->depth = depth;
	return pType;
}

tpDeclaration *newDeclaration(void *node, enum enDeclaration decType) {
	tpDeclaration *pDec = NEW(tpDeclaration);
	pDec->decType = decType;
	pDec->pNode = node;
	return pDec;
}

ndParameters* createParametersNode(ndParameter* pParam) {
	ndParameters *pParams = NEW(ndParameters);
	pParams->pList = createList();
	addParam(pParams, pParam);
	return pParams;
}

ndParameter* createParameterNode(tpType *pType, char *name) {
	ndParameter *pParam = NEW(ndParameter);
	pParam->pType = pType;
	pParam->name = name;
	return pParam;
}

void addParam(ndParameters *pParams, ndParameter *pParam) {
	addLast(pParams->pList, (void*)pParam);
}

ndVariables *createVariablesNode(char *name) {
  ndVariables *pVars = NEW(ndVariables);
  pVars->pList = createList();
  addVariable(pVars, name);
  return pVars;
}

void addVariable(ndVariables *pVars, char *name) {
  ndVariable *pVar = NEW(ndVariable);
  pVar->name = name;
  addLast(pVars->pList, pVar);
}






/* PRINTING */

void printProgram() {
	tpDeclaration* pDec = pProgram->pDec;
	printf("program:\n");
	if(pDec->decType == DEC_FUNCTION) {
		printFunction((ndFunction*) pDec->pNode, strdup("  "));
	} else {
		printVariables((ndVariables*) pDec->pNode, strdup("  "));
  }
}

void printFunction(ndFunction* pFunc, char *ident) {
	tpType* pReturn = pFunc->pReturnType;
	printf("%sfunction:%s,%d,%d\n", ident, pFunc->name,pReturn->token,pReturn->depth);

  ident = addIdent(ident);
	printParameters(pFunc->pParameters, ident);
	printBlock(pFunc->pBlock, ident);
}

void printParameters(ndParameters *pParameters, char *ident) {
  int i;
  tpList *pList;
  if(pParameters == NULL) return;
  pList = pParameters->pList;
  printf("%sparameters:\n", ident);

  ident = addIdent(ident);
  resetList(pList);
  while(goPrevious(pList)) {
    ndParameter *pParam = (ndParameter*) getCurrentValue(pList);
    printParameter(pParam, ident);
  }
}

void printParameter(ndParameter *pParam, char *ident) {
  tpType *pType = pParam->pType;
  printf("%sparameter:%s,%d,%d\n", ident, pParam->name, pType->token, pType->depth);
}

void printVariables(ndVariables *pVariables, char *ident) {
  tpList *pList = pVariables->pList;
  tpType *pType = pVariables->pType;
  printf("%svariables:%d,%d\n", ident, pType->token, pType->depth);

  ident = addIdent(ident);
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    printVariable(pVar, ident);
  }
}

void printVariable(ndVariable *pVar, char *ident) {
  printf("%svariable:%s\n", ident, pVar->name);
}

void printBlock(ndBlock *pBlock, char *ident) {
	printf("%sblock:\n", ident);
}

char *addIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)+3));
	memset(newIdent, ' ', strlen(ident)+2);
	newIdent[strlen(ident)+3] = '\0';
	return newIdent;
}
#endif
