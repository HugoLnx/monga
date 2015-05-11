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
  ndVarDeclarations *pVarDecs;
  ndStatements *pStats;
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

typedef struct stVarDeclarationsNode {
	tpList *pList;
} ndVarDeclarations;

typedef struct stStatementsNode {
	tpList *pList;
} ndStatements;

typedef struct stStatementNode {
	void *pNode;
  enum enStatType statType;
} ndStatement;

 
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

ndBlock *createBlockNode(ndVarDeclarations *pVarDecs, ndStatements *pStats) {
	ndBlock *pBlock = NEW(ndBlock);
  pBlock->pVarDecs = pVarDecs;
  pBlock->pStats = pStats;
  return pBlock;
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

ndVarDeclarations *createVarDeclarations(tpType *pType, ndVariables *pVars) {
  ndVarDeclarations *pVarDecs = NEW(ndVarDeclarations);
  pVarDecs->pList = createList();
  addVarDeclaration(pVarDecs, pType, pVars);
  return pVarDecs;
}

void addVarDeclaration(ndVarDeclarations *pVarDecs, tpType *pType, ndVariables *pVars) {
  pVars->pType = pType;
  tpList *pList = pVarDecs->pList;
  addLast(pList, (void*)pVars);
}

ndStatements *createStatements(ndStatement *pStat) {
  ndStatements *pStats = NEW(ndStatements);
  pStats->pList = createList();
  addStatement(pStats, pStat);
  return pStats;
}

void addStatement(ndStatements *pStats, ndStatement *pStat) {
  addLast(pStats->pList, (void*) pStat);
}

ndStatement *createStatement(void *pNode, enum enStatType statType) {
  ndStatement *pStat = NEW(ndStatement);
  pStat->pNode = pNode;
  pStat->statType = statType;
  return pStat;
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
  ident = addIdent(ident);
  printVarDeclarations(pBlock->pVarDecs, ident);
  printStatements(pBlock->pStats, ident);
}

void printVarDeclarations(ndVarDeclarations *pVarDecs, char *ident) {
  tpList *pList;
  if(pVarDecs == NULL) return;
  pList = pVarDecs->pList;
  printf("%svar_declarations:\n", ident);

  ident = addIdent(ident);
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariables *pVars = (ndVariables*) getCurrentValue(pList);
    printVariables(pVars, ident);
  }
}

void printStatements(ndStatements *pStats, char *ident) {
  tpList *pList;
  if(pStats == NULL) return;
  pList = pStats->pList;
  printf("%sstatements:\n", ident);

  ident = addIdent(ident);
  resetList(pList);
  while(goPrevious(pList)) {
    ndStatement *pStat = (ndStatement*) getCurrentValue(pList);
    printStatement(pStat, ident);
  }
}

void printStatement(ndStatement *pStat, char *ident) {
  printf("%sstatement:\n", ident);
  ident = addIdent(ident);
  switch(pStat->statType){
    case(STAT_BLOCK): printBlock((ndBlock*) pStat->pNode, ident); break;
  }
}

char *addIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)+3));
	memset(newIdent, ' ', strlen(ident)+2);
	newIdent[strlen(ident)+3] = '\0';
	return newIdent;
}
#endif
