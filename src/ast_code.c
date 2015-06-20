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
	
	switch(pExp->expType) {
		case(EXPND_VAR):
			// TODO
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
  pList = pCall->pExpList->pList;
  resetList(pList);
  while(goNext(pList)) {
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
