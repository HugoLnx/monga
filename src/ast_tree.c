#ifndef AST_TREE_C
#define AST_TREE_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./list.h"
#include "./ast_tree.h"
#define NEW(type) (type*) malloc(sizeof(type))
extern ndDeclarations *pDeclarations;

typedef struct stDeclaration {
	enum enDeclaration decType;
	void *pNode;
} ndDeclaration;

typedef struct stDeclarations {
	tpList *pList;
} ndDeclarations;

typedef struct stType {
	int token;
	int depth;
} tpType;

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

typedef struct stVariablesNode {
	tpList *pList;
} ndVariables;

typedef struct stVariableNode {
	tpType *pType;
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

typedef struct stIfElseNode {
  ndExpression *nExpIf;
  ndStatement *nStatementIf;
  ndStatement *nStatementElse;
} ndIfElse;

typedef struct stWhileNode {
  ndExpression *pExp;
  ndStatement *pStat;
} ndWhile;

 
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
	if (pDec->decType == DEC_FUNCTION) {
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

ndDeclaration *createDeclarationNode(void *node, enum enDeclaration decType) {
	ndDeclaration *pDec = NEW(ndDeclaration);
	pDec->decType = decType;
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





/* PRINTING */
void printDeclaration(ndDeclaration *pDec, char *ident);
void printFunction(ndFunction* pFunc, char *ident);
void printBlock(ndBlock *pBlock, char *ident);
void printParameters(ndParameters *pParameters, char *ident);
void printParameter(ndVariable *pVar, char *ident);
void printVariable(ndVariable *pVar, char *ident);
void printVariables(ndVariables *pVariables, char *ident);
void printVarDeclarations(ndVarDeclarations *pVarDecs, char *ident);
void printStatements(ndStatements *pStats, char *ident);
void printStatement(ndStatement *pStat, char *ident);
void printReturn(ndReturn *pReturn, char *ident);
void printAttribution(ndAttribution *pAttribution, char *ident);
void printVar(ndVar *pVar, char *ident);
void printExp(ndExpression *pExp, char *ident);
void printNewNode(ndNew *pNew, char *ident);
void printFunctionCallNode(ndFunctionCall *pfunctionCall, char *ident);
void printExpListNode(ndExpList *pExpList, char *ident);
void printIfElseNode(ndIfElse *pNode, char *ident);
void printWhileNode(ndWhile *pWhile, char *ident);
char *addIdent(char *ident);


void printDeclarations() {
	printf("declarations:\n");
  tpList *pList;
  if(pDeclarations == NULL) return;
  pList = pDeclarations->pList;
  resetList(pList);

  while(goPrevious(pList)) {
    ndDeclaration *pDec = (ndDeclaration*) getCurrentValue(pList);
    printDeclaration(pDec, strdup("  "));
  }
}

void printDeclaration(ndDeclaration *pDec, char *ident) {
	printf("%sdeclaration:\n", ident);
	ident = addIdent(ident);
	if(pDec->decType == DEC_FUNCTION) {
		printFunction((ndFunction*) pDec->pNode, ident);
	} else {
		printVariables((ndVariables*) pDec->pNode, ident);
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
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    printParameter(pVar, ident);
  }
}

void printParameter(ndVariable *pVar, char *ident) {
  tpType *pType = pVar->pType;
  printf("%sparameter:%s,%d,%d\n", ident, pVar->name, pType->token, pType->depth);
}

void printVariables(ndVariables *pVariables, char *ident) {
  tpList *pList = pVariables->pList;
  printf("%svariables:\n", ident);

  ident = addIdent(ident);
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    printVariable(pVar, ident);
  }
}

void printVariable(ndVariable *pVar, char *ident) {
  tpType *pType = pVar->pType;
  printf("%svariable:%s,%d,%d\n", ident, pVar->name, pType->token, pType->depth);
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
    case(STAT_IF): printIfElseNode((ndIfElse*) pStat->pNode, ident);break;
    case(STAT_WHILE): printWhileNode((ndWhile*) pStat->pNode, ident);break;
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
		case(EXPND_VAR):
      printf("%sexp:%d\n", ident, pExp->expType);
			printVar((ndVar*) (pExp->value.pNode), addIdent(ident));
			break;
    case(EXPND_NEW): 
      printf("%sexp:%d\n", ident, pExp->expType);
			printNewNode((ndNew*) (pExp->value.pNode), addIdent(ident));
			break;
		case(EXPND_MINUS):
		case(EXPND_EXCLAMATION): 
      printf("%sexp:%d\n", ident, pExp->expType);
			printExp((ndExpression*) (pExp->value.pNode), addIdent(ident));
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

void printIfElseNode(ndIfElse *pIfElse, char *ident) {
  printf("%sif:\n",ident);
  printExp(pIfElse->nExpIf, addIdent(ident));
  
  if (pIfElse->nStatementIf != NULL){
    printStatement(pIfElse->nStatementIf, addIdent(ident));
  }
  if (pIfElse->nStatementElse != NULL){
    printf("%selse:\n",ident);
    printStatement(pIfElse->nStatementElse, addIdent(ident));
  }
}

void printWhileNode(ndWhile *pWhile, char *ident) {
  printf("%swhile:\n",ident);
  ident = addIdent(ident);
  printExp(pWhile->pExp, ident);
  printStatement(pWhile->pStat, ident);
}

char *addIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)+3));
	memset(newIdent, ' ', strlen(ident)+2);
	newIdent[strlen(ident)+3] = '\0';
	return newIdent;
}
#endif
