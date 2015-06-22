#include "ast_traversing.h"
#define CALL_EVENT(evtName, evtVar) if (pEvents->evtName != NULL) pEvents->evtName(evtVar, pShared)
#define CALL_LEVEL_EVENT(evtName,relatedEvt) if (pEvents->evtName != NULL) pEvents->evtName(#relatedEvt, pShared)

void goDeclarations(ndDeclarations *pDecs             ,TRA_tpEvents *pEvents, void *pShared);
void goDeclaration(ndDeclaration *pDec                ,TRA_tpEvents *pEvents, void *pShared);
void goFunction(ndFunction* pFunc                     ,TRA_tpEvents *pEvents, void *pShared);
void goBlock(ndBlock *pBlock                          ,TRA_tpEvents *pEvents, void *pShared);
void goParameters(ndParameters *pParameters           ,TRA_tpEvents *pEvents, void *pShared);
void goParameter(ndVariable *pVar                     ,TRA_tpEvents *pEvents, void *pShared);
void goVariable(ndVariable *pVar                      ,TRA_tpEvents *pEvents, void *pShared);
void goVariables(ndVariables *pVariables              ,TRA_tpEvents *pEvents, void *pShared);
void goVarDeclarations(ndVarDeclarations *pVarDecs    ,TRA_tpEvents *pEvents, void *pShared);
void goStatements(ndStatements *pStats                ,TRA_tpEvents *pEvents, void *pShared);
void goStatement(ndStatement *pStat                   ,TRA_tpEvents *pEvents, void *pShared);
void goReturn(ndReturn *pReturn                       ,TRA_tpEvents *pEvents, void *pShared);
void goAttribution(ndAttribution *pAttribution        ,TRA_tpEvents *pEvents, void *pShared);
void goVar(ndVar *pVar                                ,TRA_tpEvents *pEvents, void *pShared);
void goExp(ndExpression *pExp                         ,TRA_tpEvents *pEvents, void *pShared);
void goNewNode(ndNew *pNew                            ,TRA_tpEvents *pEvents, void *pShared);
void goFunctionCallNode(ndFunctionCall *pfunctionCall ,TRA_tpEvents *pEvents, void *pShared);
void goExpListNode(ndExpList *pExpList                ,TRA_tpEvents *pEvents, void *pShared);
void goIfNode(ndIfElse *pNode                         ,TRA_tpEvents *pEvents, void *pShared);
void goElseNode(ndIfElse *pNode                       ,TRA_tpEvents *pEvents, void *pShared);
void goWhileNode(ndWhile *pWhile                      ,TRA_tpEvents *pEvents, void *pShared);


void TRA_execute(ndDeclarations* pDeclarations, TRA_tpEvents *pEvents, void *pShared) {
	goDeclarations(pDeclarations, pEvents, pShared);
}

void goDeclarations(ndDeclarations *pDeclarations ,TRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pDeclarations == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onDeclarations);
	CALL_EVENT(onDeclarations, pDeclarations);
  pList = pDeclarations->pList;
  resetList(pList);

  while(goPrevious(pList)) {
    ndDeclaration *pDec = (ndDeclaration*) getCurrentValue(pList);
    goDeclaration(pDec, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onDeclarations);
}

void goDeclaration(ndDeclaration *pDec, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onDeclaration);
	CALL_EVENT(onDeclaration, pDec);
	if(pDec->decType == DEC_FUNCTION) {
		goFunction((ndFunction*) pDec->pNode, pEvents, pShared);
	} else {
		goVariables((ndVariables*) pDec->pNode, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onDeclaration);
}

void goFunction(ndFunction* pFunc, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onFunction);
	CALL_EVENT(onFunction, pFunc);

	goParameters(pFunc->pParameters, pEvents, pShared);
	goBlock(pFunc->pBlock, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onFunction);
}

void goParameters(ndParameters *pParameters, TRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pParameters == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onParameters);
	CALL_EVENT(onParameters, pParameters);
  pList = pParameters->pList;

  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    goParameter(pVar, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onParameters);
}

void goParameter(ndVariable *pVar, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onParameter);
	CALL_EVENT(onParameter, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onParameter);
}

void goVariables(ndVariables *pVariables, TRA_tpEvents *pEvents, void *pShared) {
  tpList *pList = pVariables->pList;
	CALL_LEVEL_EVENT(onNewLevel,onVariables);
	CALL_EVENT(onVariables, pVariables);

  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    goVariable(pVar, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onVariables);
}

void goVariable(ndVariable *pVar, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onVariable);
	CALL_EVENT(onVariable, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onVariable);
}

void goBlock(ndBlock *pBlock ,TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onBlock);
	CALL_EVENT(onBlock, pBlock);
  goVarDeclarations(pBlock->pVarDecs, pEvents, pShared);
  goStatements(pBlock->pStats, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onBlock);
}

void goVarDeclarations(ndVarDeclarations *pVarDecs, TRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pVarDecs == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onVarDeclarations);
	CALL_EVENT(onVarDeclarations, pVarDecs);

  pList = pVarDecs->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariables *pVars = (ndVariables*) getCurrentValue(pList);
    goVariables(pVars, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onVarDeclarations);
}

void goStatements(ndStatements *pStats, TRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pStats == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onStatements);
	CALL_EVENT(onStatements, pStats);

  pList = pStats->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndStatement *pStat = (ndStatement*) getCurrentValue(pList);
    goStatement(pStat, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onStatements);
}

void goStatement(ndStatement *pStat, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onStatement);
	CALL_EVENT(onStatement, pStat);
  switch(pStat->statType){
    case(STAT_BLOCK): goBlock((ndBlock*) pStat->pNode, pEvents, pShared); break;
    case(STAT_ATTRIBUTION): goAttribution((ndAttribution*) pStat->pNode, pEvents, pShared);break;
    case(STAT_RETURN_CALL): goReturn((ndReturn*) pStat->pNode, pEvents, pShared);break;
    case(STAT_FUNCTION_CALL): goFunctionCallNode((ndFunctionCall*) pStat->pNode, pEvents, pShared);break;
    case(STAT_IF): goIfNode((ndIfElse*) pStat->pNode, pEvents, pShared);break;
    case(STAT_WHILE): goWhileNode((ndWhile*) pStat->pNode, pEvents, pShared);break;
  }
	CALL_LEVEL_EVENT(onBackLevel,onStatement);
}

void goReturn(ndReturn *pReturn, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onReturn);
	CALL_EVENT(onReturn, pReturn);
  goExp(pReturn->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onReturn);
}

void goAttribution(ndAttribution *pAttribution, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onAttribution);
	CALL_EVENT(onAttribution, pAttribution);

  goVar(pAttribution->pVar, pEvents, pShared);
  goExp(pAttribution->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onAttribution);
}

void goVar(ndVar *pVar, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onVar);
	CALL_EVENT(onVar, pVar);
  if(pVar->varType == VAR_ARRAY) {
    goExp(pVar->value.address.pPointerExp, pEvents, pShared);
    goExp(pVar->value.address.pInxExp, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onVar);
}

void goExp(ndExpression *pExp, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onExp);
	CALL_EVENT(onExp, pExp);
  switch(pExp->expType) {
		case(EXPND_VAR):
			goVar((ndVar*) (pExp->value.pNode), pEvents, pShared);
			break;
    case(EXPND_NEW): 
			goNewNode((ndNew*) (pExp->value.pNode), pEvents, pShared);
			break;
		case(EXPND_MINUS):
		case(EXPND_EXCLAMATION): 
			goExp((ndExpression*) (pExp->value.pNode), pEvents, pShared);
			break;
    case(EXPND_BIN): 
			goExp(pExp->value.bin.pExp1, pEvents, pShared);
			goExp(pExp->value.bin.pExp2, pEvents, pShared);
			break;
    case(EXPND_CALL): 
      goFunctionCallNode((ndFunctionCall*) pExp->value.pNode, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onExp);
}

void goNewNode(ndNew *pNew, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onNew);
	CALL_EVENT(onNew, pNew);
  goExp(pNew->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onNew);
}

void goFunctionCallNode(ndFunctionCall *pFunctionCall, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onFunctionCall);
	CALL_EVENT(onFunctionCall, pFunctionCall);
  
  goExpListNode(pFunctionCall->pExpList, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onFunctionCall);
}

void goExpListNode(ndExpList *pExpList, TRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pExpList == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onExpList);
	CALL_EVENT(onExpList, pExpList);

  pList = pExpList->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
    goExp(pStat, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onExpList);
}

void goIfNode(ndIfElse *pIfElse, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onIf);
	CALL_EVENT(onIf, pIfElse);
  goExp(pIfElse->nExpIf, pEvents, pShared);
  
  goStatement(pIfElse->nStatementIf, pEvents, pShared);
	goElseNode(pIfElse, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onIf);
}

void goElseNode(ndIfElse *pIfElse, TRA_tpEvents *pEvents, void *pShared) {
  if (pIfElse->nStatementElse == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onElse);
	CALL_EVENT(onElse, pIfElse);
	goStatement(pIfElse->nStatementElse, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onElse);
}

void goWhileNode(ndWhile *pWhile, TRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onWhile);
	CALL_EVENT(onWhile, pWhile);
  goExp(pWhile->pExp, pEvents, pShared);
  goStatement(pWhile->pStat, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onWhile);
}
