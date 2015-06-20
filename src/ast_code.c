#include "ast_traversing.h"
#include "ast_code.h"
#include "assembly_writer.h"
#include "utils.h"
#include "label_generator.h"
#include "list.h"
#include <string.h>

void beforeEvent(char *evtName, void *pShared) {
}

void afterEvent(char *evtName, void *pShared) {
	if (strcmp(evtName, "onFunction") == 0) {
		ASY_functionEnding();
	}
}

void codeForHeader(ndDeclarations *pDeclarations, void *pShared) {
	ASY_raw(".data\n");
}

void codeForGlobalVariable(ndVariable *pVar, void *pShared) {
	//ASY_globalVar(pVar->name, );
}

void codeForFunction(ndFunction *pFunc, void *pShared) {
	ASY_raw(".text\n");
	ASY_function(pFunc->name);
	ASY_functionBeginning();
}

void codeForExp(ndExpression *pExp, void *pShared) {
	char *label = LBL_generate(LBL_next());
	ASY_raw(".data\n");
	ASY_raw("%s: .string \"%s\"\n", label, pExp->value.text);
	ASY_raw(".text\n");
	ASY_raw("movl $%s, %%eax\n", label);
}

void codeForFunctionCall(ndFunctionCall *pCall, void *pShared) {
  int qntParams = 0;
	tpList *pList;
	ASY_functionCallHeader();
  pList = pCall->pExpList->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
		codeForExp(pStat, pShared);
		ASY_raw("pushl %%eax\n");
		qntParams++;
  }
	ASY_functionCall(pCall->functionName, qntParams);
}

void COD_codeForTree(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);

  pEvents->onDeclarations = codeForHeader;
  pEvents->onVariable = codeForGlobalVariable;
  pEvents->onFunction = codeForFunction;
  pEvents->onFunctionCall = codeForFunctionCall;

  pEvents->onNewLevel = beforeEvent; 
  pEvents->onBackLevel = afterEvent; 

  TRA_execute(pDeclarations, pEvents, NULL);
}
