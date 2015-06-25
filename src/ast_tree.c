#ifndef AST_TREE_C
#define AST_TREE_C

#include <string.h>
#include "ast_tree.h"
#include "utils.h"
extern ndDeclarations *pDeclarations;
 
char *noQuotes(char *str) {
	char *newStr = (char*) malloc(sizeof(char)*strlen(str)-1);
	strncpy(newStr, str+1, strlen(str)-2);
	newStr[strlen(str)-2] = '\0';
	return newStr;
}

void incrDepth(tpType *pType) {
  pType->depth += 1;
}

void addTypeToVars(tpType *pType, ndVariables *pVars) {
	tpList *pList = pVars->pList;	
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
		pVar->pType = pType;
  }
}

ndDeclaration *finishDeclaration(tpType *pType, ndDeclaration *pDec) {
	if (pDec->decTag == DEC_FUNCTION) {
		ndFunction *pFunc = (ndFunction*) pDec->pNode;
		pFunc->pReturnType = pType;
	} else {
		ndVariables *pVars = (ndVariables*) pDec->pNode;
		addTypeToVars(pType, pVars);
  }

	return pDec;
}

ndDeclarations *createFullDeclarationsNode(ndDeclaration *pDec) {
	pDeclarations = NEW(ndDeclarations);
	pDeclarations->pList = createList();
	addFullDeclaration(pDeclarations, pDec);
	return pDeclarations;
}

ndDeclarations *addFullDeclaration(ndDeclarations *pDecs, ndDeclaration *pDec) {
	addLast(pDecs->pList, (void*)pDec);
	return pDecs;
}

ndFunction *createFunctionNode(char *name, ndParameters *pParams, ndBlock *pBlock) {
	ndFunction *pFunc = NEW(ndFunction);
	pFunc->name = name;
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

ndDeclaration *createDeclarationNode(void *node, enum enDeclaration decTag) {
	ndDeclaration *pDec = NEW(ndDeclaration);
	pDec->decTag = decTag;
	pDec->pNode = node;
	return pDec;
}

ndParameters* createParametersNode(ndVariable* pVar) {
	ndParameters *pParams = NEW(ndParameters);
	pParams->pList = createList();
	addParam(pParams, pVar);
	return pParams;
}

ndVariable* createParameterNode(tpType *pType, char *name) {
  ndVariable *pVar = NEW(ndVariable);
  pVar->pType = pType;
  pVar->name = name;
  return pVar;
}

void addParam(ndParameters *pParams, ndVariable *pVar) {
	addLast(pParams->pList, (void*)pVar);
}

ndExpList* createExpListNode(ndExpression *pExp) {
  ndExpList *pExpList = NEW(ndExpList);
  pExpList->pList =  createList();
  addExpListNode(pExpList, pExp);
  return pExpList;
}

void addExpListNode(ndExpList *pExpList, ndExpression *pExp) {
  addLast(pExpList->pList, (void*)pExp);
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
	addTypeToVars(pType, pVars);
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

ndStatement *createStatement(void *pNode, enum enStatTag statTag) {
  ndStatement *pStat = NEW(ndStatement);
  pStat->pNode = pNode;
  pStat->statTag = statTag;
  return pStat;
}

ndReturn *createReturnNode(ndExpression *pExp) {
  ndReturn *pReturn = NEW(ndReturn);
  pReturn->pExp = pExp;
  return pReturn;
}

ndAttribution *createAttributionNode(ndVar *pVar, ndExpression *pExp) {
  ndAttribution *pAttribution = NEW(ndAttribution);
  pAttribution->pVar = pVar;
  pAttribution->pExp = pExp;
  return pAttribution;
}

ndVar *createVarNode(char *name) {
  ndVar *pVar = NEW(ndVar);
  pVar->value.name = name;
  pVar->varTag = VAR_ID;
  return pVar;
}

ndVar *createArrayVarNode(ndExpression *pPointerExp, ndExpression *pInxExp) {
  ndVar *pVar = NEW(ndVar);
  pVar->value.address.pPointerExp = pPointerExp;
  pVar->value.address.pInxExp = pInxExp;
  pVar->varTag = VAR_ARRAY;
  return pVar;
}

ndExpression *createExpressionIntegerNode(int val, enum enExpTag expTag) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.ival = val;
  pExp->expTag = expTag;
  return pExp;
}

ndExpression *createExpressionFloatNode(double val) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.fval = val;
  pExp->expTag = EXP_FLOAT;
  return pExp;
}

ndExpression *createExpressionTextNode(char *val) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.text = noQuotes(val);
  pExp->expTag = EXP_TEXT;
  return pExp;
}

ndExpression *createExpressionGenericNode(void *pNode, enum enExpTag expTag) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.pNode = pNode;
  pExp->expTag = expTag;
  return pExp;
}

ndExpression *createExpressionBinaryNode(ndExpression *pExp1, ndExpression *pExp2, enum enExpBinTag expBinTag) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.bin.pExp1 = pExp1;
  pExp->value.bin.pExp2 = pExp2;
  pExp->value.bin.expTag = expBinTag;
  pExp->expTag = EXPND_BIN;
  return pExp;
}

ndNew *createNewNode(tpType *pType, ndExpression *pExp) {
  ndNew *pNew = NEW(ndNew);
  pNew->pType = pType;
  pNew->pExp = pExp;
  return pNew;
}

ndFunctionCall *createFunctionCall(char *functionName, ndExpList *pExpList) {
  ndFunctionCall *pFunctionCall = NEW(ndFunctionCall);
  pFunctionCall->functionName = functionName;
  pFunctionCall->pExpList = pExpList;

  return pFunctionCall;
}

ndIfElse *createIfElseNode(ndExpression *nExpIf, ndStatement *nStatementIf){
  ndIfElse *pIfElse = NEW(ndIfElse);

  pIfElse->nExpIf = nExpIf;
  pIfElse->nStatementIf = nStatementIf;
  pIfElse->nStatementElse = NULL;

  return pIfElse;
}

ndWhile *createWhileNode(ndExpression *pExp, ndStatement *pStat){
  ndWhile *pWhile = NEW(ndWhile);

  pWhile->pExp = pExp;
  pWhile->pStat = pStat;

  return pWhile;
}

ndIfElse *addElseStatement(ndIfElse *pIfElse, ndStatement *nStatementElse){
  pIfElse->nStatementElse = nStatementElse;
  return pIfElse;
}
#endif
