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

typedef struct stReturnNode {
  ndExpression *pExp;
} ndReturn;

typedef struct stExpressionNode {
  enum enExpType expType;
  union {
    long long int ival;
    char *text;
    double fval;
    void *pNode;
    struct {
      ndExpression *pExp1;
      ndExpression *pExp2;
      enum enExpBinType expType;
    } bin;
  } value;
} ndExpression;

typedef struct stVarNode {
  enum enVarType varType;
  union {
    char *name;
    struct {
      ndExpression *pPointerExp;
      ndExpression *pInxExp;
    } address;
  } value;
} ndVar;

typedef struct stAttributionNode {
  ndVar *pVar;
  ndExpression *pExp;
} ndAttribution;

typedef struct stNewNode {
  tpType *pType;
  ndExpression *pExp;
} ndNew;

typedef struct stFunctionCallNode {
  char *functionName;
  ndExpList *pExpList;
} ndFunctionCall;

typedef struct stExpListNode {
  tpList *pList;
} ndExpList;

 
char *strDup(char *str) {
	char* dup = (char*) malloc(sizeof(char)*(strlen(str)+1));
	strcpy(dup, str);
	return dup;
}

char *noQuotes(char *str) {
	char *newStr = (char*) malloc(sizeof(char)*strlen(str)-1);
	strncpy(newStr, str+1, strlen(str)-2);
	newStr[strlen(str)-2] = '\0';
	return newStr;
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
  pVar->varType = VAR_ID;
  return pVar;
}

ndVar *createArrayVarNode(ndExpression *pPointerExp, ndExpression *pInxExp) {
  ndVar *pVar = NEW(ndVar);
  pVar->value.address.pPointerExp = pPointerExp;
  pVar->value.address.pInxExp = pInxExp;
  pVar->varType = VAR_ARRAY;
  return pVar;
}

ndExpression *createExpressionIntegerNode(int val, enum enExpType expType) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.ival = val;
  pExp->expType = expType;
  return pExp;
}

ndExpression *createExpressionFloatNode(double val) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.fval = val;
  pExp->expType = EXP_FLOAT;
  return pExp;
}

ndExpression *createExpressionTextNode(char *val) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.text = noQuotes(val);
  pExp->expType = EXP_TEXT;
  return pExp;
}

ndExpression *createExpressionGenericNode(void *pNode, enum enExpType expType) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.pNode = pNode;
  pExp->expType = expType;
  return pExp;
}

ndExpression *createExpressionBinaryNode(ndExpression *pExp1, ndExpression *pExp2, enum enExpBinType expBinType) {
  ndExpression *pExp = NEW(ndExpression);
  pExp->value.bin.pExp1 = pExp1;
  pExp->value.bin.pExp2 = pExp2;
  pExp->value.bin.expType = expBinType;
  pExp->expType = EXPND_BIN;
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
    case(STAT_ATTRIBUTION): printAttribution((ndAttribution*) pStat->pNode, ident);break;
    case(STAT_RETURN_CALL): printReturn((ndReturn*) pStat->pNode, ident);break;
    case(STAT_FUNCTION_CALL): printFunctionCallNode((ndFunctionCall*) pStat->pNode, ident);break;
  }
}

void printReturn(ndReturn *pReturn, char *ident) {
  printf("%sreturn:\n",ident);

  ident = addIdent(ident);
  printExp(pReturn->pExp, ident);
}

void printAttribution(ndAttribution *pAttribution, char *ident) {
  printf("%sattribution:\n", ident);
  ident = addIdent(ident);

  printVar(pAttribution->pVar, ident);
  printExp(pAttribution->pExp, ident);
}

void printVar(ndVar *pVar, char *ident) {
  if(pVar->varType == VAR_ID) {
    printf("%svar:%s\n", ident, pVar->value.name);
  } else {
    printf("%svar:\n", ident);
    printExp(pVar->value.address.pPointerExp, ident);
    printExp(pVar->value.address.pInxExp, ident);
  }
}

void printExp(ndExpression *pExp, char *ident) {
  switch(pExp->expType) {
    case(EXP_NUMBER):
      printf("%sexp:%d,%lld\n", ident, pExp->expType, pExp->value.ival);
			break;
    case(EXP_HEXADECIMAL):
      printf("%sexp:%d,0x%x\n", ident, pExp->expType, pExp->value.ival);
			break;
    case(EXP_CHAR):
      printf("%sexp:%d,%c\n", ident, pExp->expType, pExp->value.ival);
			break;
    case(EXP_FLOAT):
      printf("%sexp:%d,%.5f\n", ident, pExp->expType, pExp->value.fval);
			break;
    case(EXP_TEXT):
      printf("%sexp:%d,\"%s\"\n", ident, pExp->expType, pExp->value.text);
			break;
    case(EXPND_NEW): 
      printf("%sexp:%d\n", ident, pExp->expType);
			printNewNode((ndNew*) (pExp->value.pNode), addIdent(ident));
			break;
    case(EXPND_BIN): 
      printf("%sexp:%d\n", ident, pExp->expType);

			ident = addIdent(ident);
      printf("%sbinary:%d\n", ident, pExp->value.bin.expType);	
			ident = addIdent(ident);
			printExp(pExp->value.bin.pExp1, ident);
			printExp(pExp->value.bin.pExp2, ident);
			break;
  }
}

void printNewNode(ndNew *pNew, char *ident) {
  printf("%snew:%d,%d\n",ident, pNew->pType->token, pNew->pType->depth);
  printExp(pNew->pExp, addIdent(ident));
}

void printFunctionCallNode(ndFunctionCall *pFunctionCall, char *ident) {
  printf("%sfunction call:%s\n",ident, pFunctionCall->functionName);
  
  if (pFunctionCall->pExpList != NULL){
    printExpListNode(pFunctionCall->pExpList, addIdent(ident));
  }
}

void printExpListNode(ndExpList *pExpList, char *ident) {
  tpList *pList;
  if(pExpList == NULL) return;
  pList = pExpList->pList;
  printf("%sexp list:\n",ident);
  
  ident = addIdent(ident);
  resetList(pList);
  while(goPrevious(pList)) {
    ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
    printExp(pStat, ident);
  }
}

char *addIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)+3));
	memset(newIdent, ' ', strlen(ident)+2);
	newIdent[strlen(ident)+3] = '\0';
	return newIdent;
}
#endif
