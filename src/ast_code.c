#include "ast_traversing.h"
#include "ast_code.h"
#include "assembly_writer.h"
#include "utils.h"
#include "label_generator.h"
#include "list.h"
#include <string.h>

#define STATE(varName) ((tpState*) varName)

typedef struct stState {
	int lastVarPadding;
} tpState;

void beforeEvent(char *evtName, void *pShared) {
}

void afterEvent(char *evtName, void *pShared) {
	if (strcmp(evtName, "onFunction") == 0) {
		ASY_functionEnding();
	}
}

void codeForHeader(ndDeclarations *pDeclarations, void *pShared) {
}

void codeForGlobalVariable(ndVariable *pVar, void *pShared) {
}

void codeForVarDeclaration(ndVariable *pVar, void *pShared) {
	pVar->stackPadding = STATE(pShared)->lastVarPadding;
	STATE(pShared)->lastVarPadding += 4;
}

void codeForParameters(ndParameters *pParams, void *pShared) {
	tpList *pList;
	int paramPadding = 8;
	if (pParams == NULL) return;

  pList = pParams->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
		int padding = STATE(pShared)->lastVarPadding;
		codeForVarDeclaration(pVar, pShared);
		ASY_raw("movl $vars, %%ecx\n");
		ASY_raw("addl $%d, %%ecx\n", padding);
		ASY_raw("movl %d(%%ebp), %%edx\n", paramPadding);
		ASY_raw("movl %%edx, (%%ecx)\n");
		paramPadding += 4;
  }
}

void codeForFunction(ndFunction *pFunc, void *pShared) {
	ASY_raw(".text\n");
	ASY_function(pFunc->name);
	ASY_functionBeginning();
}

void codeForVar(ndVar *pVar, void *pShared) {
	int padding = pVar->pBackDeclaration->pVarDec->stackPadding;
	ASY_raw("movl $vars, %%eax\n");
	ASY_raw("addl $%d, %%eax\n", padding);
}

void codeForExp(ndExpression *pExp, void *pShared) {
	char *label = LBL_generate(LBL_next());
	
	switch(pExp->expType) {
		case(EXPND_VAR):
      codeForVar(pExp->value.pNode, pShared);
			ASY_raw("movl (%%eax), %%eax\n");
			break;
		case(EXPND_NEW): 
			// TODO
			break;
		case(EXP_TEXT):
			ASY_raw(".data\n");
			ASY_raw("%s: .string \"%s\"\n", label, pExp->value.text);
			ASY_raw(".text\n");
			ASY_raw("movl $%s, %%eax\n", label);
			break;
		case(EXP_FLOAT):
			// ASY_raw("fstpl ")
			break;
		case(EXP_HEXADECIMAL):
		case(EXP_NUMBER):
		case(EXP_CHAR):
			ASY_raw("movl $%lld, %%eax\n", pExp->value.ival);
			break;
		case(EXPND_MINUS):
		case(EXPND_EXCLAMATION): 
			// TODO
			break;
		case(EXPND_BIN): 
			// TODO
			break;
	}
}

void codeForFunctionCall(ndFunctionCall *pCall, void *pShared) {
  int qntParams = 0;
	tpList *pList;
	ASY_functionCallHeader();
	if(pCall->pExpList != NULL) {
    pList = pCall->pExpList->pList;
		resetList(pList);
		while(goNext(pList)) {
			ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
			codeForExp(pStat, pShared);
			ASY_raw("pushl %%eax\n");
			qntParams++;
		}
	}
	ASY_functionCall(pCall->functionName, qntParams);
}

void initVarDeclarationsState(ndVarDeclarations *pVars, void *pShared) {
	//STATE(pShared)->lastVarPadding = 0;
}

void codeForAttribution(ndAttribution *pAttr, void *pShared) {
	codeForVar(pAttr->pVar, pShared);
	ASY_raw("movl %%eax, %%ecx\n");
	codeForExp(pAttr->pExp, pShared);
	ASY_raw("movl %%eax, (%%ecx)\n");
}

void COD_codeForTree(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);

  pEvents->onDeclarations = codeForHeader;
  pEvents->onVariable = codeForGlobalVariable;
  pEvents->onFunction = codeForFunction;
  pEvents->onFunctionCall = codeForFunctionCall;
  pEvents->onVariable = codeForVarDeclaration;
	pEvents->onVarDeclarations = initVarDeclarationsState;
	pEvents->onAttribution = codeForAttribution;
	pEvents->onParameters = codeForParameters;

  pEvents->onNewLevel = beforeEvent; 
  pEvents->onBackLevel = afterEvent; 

	tpState *pState = NEW(tpState);
	pState->lastVarPadding = 0;

	ASY_raw(".data\n");
	ASY_raw("vars: .space 4000\n");
  TRA_execute(pDeclarations, pEvents, (void*)pState);
	ASY_raw(".end\n\n");
}
