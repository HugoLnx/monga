#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "ast_print.h"
#include "ast_traversing.h"
#include "list.h"

void printDeclarations(ndDeclarations *pDecs             ,void *pShared);
void printDeclaration(ndDeclaration *pDec                ,void *pShared);
void printFunction(ndFunction* pFunc                     ,void *pShared);
void printBlock(ndBlock *pBlock                          ,void *pShared);
void printParameters(ndParameters *pParameters           ,void *pShared);
void printParameter(ndVariable *pVar                     ,void *pShared);
void printVariable(ndVariable *pVar                      ,void *pShared);
void printVariables(ndVariables *pVariables              ,void *pShared);
void printVarDeclarations(ndVarDeclarations *pVarDecs    ,void *pShared);
void printStatements(ndStatements *pStats                ,void *pShared);
void printStatement(ndStatement *pStat                   ,void *pShared);
void printReturn(ndReturn *pReturn                       ,void *pShared);
void printAttribution(ndAttribution *pAttribution        ,void *pShared);
void printVar(ndVar *pVar                                ,void *pShared);
void printExp(ndExpression *pExp                         ,void *pShared);
void printNewNode(ndNew *pNew                            ,void *pShared);
void printFunctionCallNode(ndFunctionCall *pfunctionCall ,void *pShared);
void printExpListNode(ndExpList *pExpList                ,void *pShared);
void printIfNode(ndIfElse *pNode                         ,void *pShared);
void printElseNode(ndIfElse *pNode                       ,void *pShared);
void printWhileNode(ndWhile *pWhile                      ,void *pShared);
char *addIdent(char *ident);
void addIdentShared(char *evtName, void *pShared);
char *removeIdent(char *ident);
void removeIdentShared(char *evtName, void *pShared);


void printTree(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);
  pEvents->onDeclarations = printDeclarations;
  pEvents->onDeclaration = printDeclaration;
  pEvents->onFunction = printFunction;
  pEvents->onBlock = printBlock;
  pEvents->onParameters = printParameters;
  pEvents->onParameter = printParameter;
  pEvents->onVariable = printVariable;
  pEvents->onVariables = printVariables;
  pEvents->onVarDeclarations = printVarDeclarations;
  pEvents->onStatements = printStatements;
  pEvents->onStatement = printStatement;
  pEvents->onReturn = printReturn;
  pEvents->onAttribution = printAttribution;
  pEvents->onVar = printVar;
  pEvents->onExp = printExp;
  pEvents->onNew = printNewNode;
  pEvents->onFunctionCall = printFunctionCallNode;
  pEvents->onExpList = printExpListNode;
  pEvents->onIf = printIfNode;
  pEvents->onElse = printElseNode;
  pEvents->onWhile = printWhileNode;
  pEvents->onNewLevel = addIdentShared;
  pEvents->onBackLevel = removeIdentShared;
	char *ident = strDup("");
	TRA_execute(pDeclarations, pEvents, (void*) &ident);
}

void printDeclarations(ndDeclarations *pDecs, void *pShared) {
	char *ident = *(char**) pShared;
	printf("%sdeclarations:\n", ident);
}

void printDeclaration(ndDeclaration *pDec, void *pShared) {
	char *ident = *(char**) pShared;
	printf("%sdeclaration:\n", ident);
}

void printFunction(ndFunction* pFunc, void *pShared) {
	char *ident = *(char**) pShared;
	tpType* pReturn = pFunc->pReturnType;
	printf("%sfunction:%s,%d,%d\n", ident, pFunc->name,pReturn->token,pReturn->depth);
}

void printParameters(ndParameters *pParameters, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sparameters:\n", ident);
}

void printParameter(ndVariable *pVar, void *pShared) {
	char *ident = *(char**) pShared;
  tpType *pType = pVar->pType;
  printf("%sparameter:%s,%d,%d\n", ident, pVar->name, pType->token, pType->depth);
}

void printVariables(ndVariables *pVariables, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%svariables:\n", ident);
}

void printVariable(ndVariable *pVar, void *pShared) {
	char *ident = *(char**) pShared;
  tpType *pType = pVar->pType;
  printf("%svariable:%s,%d,%d\n", ident, pVar->name, pType->token, pType->depth);
}

void printBlock(ndBlock *pBlock, void *pShared) {
	char *ident = *(char**) pShared;
	printf("%sblock:\n", ident);
}

void printVarDeclarations(ndVarDeclarations *pVarDecs, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%svar_declarations:\n", ident);
}

void printStatements(ndStatements *pStats, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sstatements:\n", ident);
}

void printStatement(ndStatement *pStat, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sstatement:\n", ident);
}

void printReturn(ndReturn *pReturn, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sreturn:\n",ident);
}

void printAttribution(ndAttribution *pAttribution, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sattribution:\n", ident);
}

void printVar(ndVar *pVar, void *pShared) {
	char *ident = *(char**) pShared;
  if(pVar->varType == VAR_ID) {
    printf("%svar:%s\n", ident, pVar->value.name);
  } else {
    printf("%svar:\n", ident);
  }
}

void printExp(ndExpression *pExp, void *pShared) {
	char *ident = *(char**) pShared;
  switch(pExp->expType) {
    case(EXP_NUMBER):
      printf("%sexp:%d,%lld\n", ident, pExp->expType, pExp->value.ival);
			break;
    case(EXP_HEXADECIMAL):
      printf("%sexp:%d,0x%llx\n", ident, pExp->expType, pExp->value.ival);
			break;
    case(EXP_CHAR):
      printf("%sexp:%d,%c\n", ident, pExp->expType, (char)pExp->value.ival);
			break;
    case(EXP_FLOAT):
      printf("%sexp:%d,%.5f\n", ident, pExp->expType, pExp->value.fval);
			break;
    case(EXP_TEXT):
      printf("%sexp:%d,\"%s\"\n", ident, pExp->expType, pExp->value.text);
			break;
		case(EXPND_VAR):
      printf("%sexp:%d\n", ident, pExp->expType);
			break;
    case(EXPND_NEW): 
      printf("%sexp:%d\n", ident, pExp->expType);
			break;
		case(EXPND_MINUS):
		case(EXPND_EXCLAMATION): 
      printf("%sexp:%d\n", ident, pExp->expType);
			break;
    case(EXPND_BIN): 
      printf("%sexp:%d\n", ident, pExp->expType);
      printf("%sbinary:%d\n", ident, pExp->value.bin.expType);	
			break;
    case(EXPND_CALL): 
      printf("%sexp:%d\n", ident, pExp->expType);
			break;
  }
}

void printNewNode(ndNew *pNew, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%snew:%d,%d\n",ident, pNew->pType->token, pNew->pType->depth);
}

void printFunctionCallNode(ndFunctionCall *pFunctionCall, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sfunction call:%s\n",ident, pFunctionCall->functionName);
}

void printExpListNode(ndExpList *pExpList, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sexp list:\n",ident);
}

void printIfNode(ndIfElse *pIfElse, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%sif:\n",ident);
}

void printElseNode(ndIfElse *pIfElse, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%selse:\n",ident);
}

void printWhileNode(ndWhile *pWhile, void *pShared) {
	char *ident = *(char**) pShared;
  printf("%swhile:\n",ident);
}

char *addIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)+3));
	memset(newIdent, ' ', strlen(ident)+2);
	newIdent[strlen(ident)+3] = '\0';
	return newIdent;
}

void addIdentShared(char *evtName, void *pShared) {
	if (strcmp(evtName, "onDeclarations") != 0) {
		char **pIdent = (char **) pShared;
		*pIdent = addIdent(*pIdent);
	}
}

char *removeIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)-1));
	memset(newIdent, ' ', strlen(ident)-1);
	newIdent[strlen(ident)-2] = '\0';
	return newIdent;
}

void removeIdentShared(char *evtName, void *pShared) {
	if (strcmp(evtName, "onDeclarations") != 0) {
		char **pIdent = (char **) pShared;
		*pIdent = removeIdent(*pIdent);
	}
}
