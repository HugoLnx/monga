#include "ast_dfs.h"
#define CALL_EVENT(evtName, evtVar) if (pEvents->evtName != NULL) pEvents->evtName(evtVar, pShared)
#define CALL_LEVEL_EVENT(evtName) if (pEvents->evtName != NULL) pEvents->evtName(pShared)

void goDeclarations(ndDeclarations *pDecs             ,DFS_tpEvents *pEvents, void *pShared);
void goDeclaration(ndDeclaration *pDec                ,DFS_tpEvents *pEvents, void *pShared);
void goFunction(ndFunction* pFunc                     ,DFS_tpEvents *pEvents, void *pShared);
void goBlock(ndBlock *pBlock                          ,DFS_tpEvents *pEvents, void *pShared);
void goParameters(ndParameters *pParameters           ,DFS_tpEvents *pEvents, void *pShared);
void goParameter(ndVariable *pVar                     ,DFS_tpEvents *pEvents, void *pShared);
void goVariable(ndVariable *pVar                      ,DFS_tpEvents *pEvents, void *pShared);
void goVariables(ndVariables *pVariables              ,DFS_tpEvents *pEvents, void *pShared);
void goVarDeclarations(ndVarDeclarations *pVarDecs    ,DFS_tpEvents *pEvents, void *pShared);
void goStatements(ndStatements *pStats                ,DFS_tpEvents *pEvents, void *pShared);
void goStatement(ndStatement *pStat                   ,DFS_tpEvents *pEvents, void *pShared);
void goReturn(ndReturn *pReturn                       ,DFS_tpEvents *pEvents, void *pShared);
void goAttribution(ndAttribution *pAttribution        ,DFS_tpEvents *pEvents, void *pShared);
void goVar(ndVar *pVar                                ,DFS_tpEvents *pEvents, void *pShared);
void goExp(ndExpression *pExp                         ,DFS_tpEvents *pEvents, void *pShared);
void goNewNode(ndNew *pNew                            ,DFS_tpEvents *pEvents, void *pShared);
void goFunctionCallNode(ndFunctionCall *pfunctionCall ,DFS_tpEvents *pEvents, void *pShared);
void goExpListNode(ndExpList *pExpList                ,DFS_tpEvents *pEvents, void *pShared);
void goIfNode(ndIfElse *pNode                         ,DFS_tpEvents *pEvents, void *pShared);
void goElseNode(ndIfElse *pNode                       ,DFS_tpEvents *pEvents, void *pShared);
void goWhileNode(ndWhile *pWhile                      ,DFS_tpEvents *pEvents, void *pShared);


void DFS_execute(ndDeclarations* pDeclarations, DFS_tpEvents *pEvents, void *pShared) {
	goDeclarations(pDeclarations, pEvents, pShared);
}

void goDeclarations(ndDeclarations *pDeclarations ,DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
  tpList *pList;
  if(pDeclarations == NULL) return;
	CALL_EVENT(onDeclarations, pDeclarations);
  pList = pDeclarations->pList;
  resetList(pList);

  while(goPrevious(pList)) {
    ndDeclaration *pDec = (ndDeclaration*) getCurrentValue(pList);
    goDeclaration(pDec, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goDeclaration(ndDeclaration *pDec, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onDeclaration, pDec);
	if(pDec->decType == DEC_FUNCTION) {
		goFunction((ndFunction*) pDec->pNode, pEvents, pShared);
	} else {
		goVariables((ndVariables*) pDec->pNode, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goFunction(ndFunction* pFunc, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onFunction, pFunc);

	goParameters(pFunc->pParameters, pEvents, pShared);
	goBlock(pFunc->pBlock, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goParameters(ndParameters *pParameters, DFS_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pParameters == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onParameters, pParameters);
  pList = pParameters->pList;

  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    goParameter(pVar, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goParameter(ndVariable *pVar, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onParameter, pVar);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goVariables(ndVariables *pVariables, DFS_tpEvents *pEvents, void *pShared) {
  tpList *pList = pVariables->pList;
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onVariables, pVariables);

  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    goVariable(pVar, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goVariable(ndVariable *pVar, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onVariable, pVar);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goBlock(ndBlock *pBlock ,DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onBlock, pBlock);
  goVarDeclarations(pBlock->pVarDecs, pEvents, pShared);
  goStatements(pBlock->pStats, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goVarDeclarations(ndVarDeclarations *pVarDecs, DFS_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pVarDecs == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onVarDeclarations, pVarDecs);

  pList = pVarDecs->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariables *pVars = (ndVariables*) getCurrentValue(pList);
    goVariables(pVars, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goStatements(ndStatements *pStats, DFS_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pStats == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onStatements, pStats);

  pList = pStats->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndStatement *pStat = (ndStatement*) getCurrentValue(pList);
    goStatement(pStat, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goStatement(ndStatement *pStat, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onStatement, pStat);
  switch(pStat->statType){
    case(STAT_BLOCK): goBlock((ndBlock*) pStat->pNode, pEvents, pShared); break;
    case(STAT_ATTRIBUTION): goAttribution((ndAttribution*) pStat->pNode, pEvents, pShared);break;
    case(STAT_RETURN_CALL): goReturn((ndReturn*) pStat->pNode, pEvents, pShared);break;
    case(STAT_FUNCTION_CALL): goFunctionCallNode((ndFunctionCall*) pStat->pNode, pEvents, pShared);break;
    case(STAT_IF): goIfNode((ndIfElse*) pStat->pNode, pEvents, pShared);break;
    case(STAT_WHILE): goWhileNode((ndWhile*) pStat->pNode, pEvents, pShared);break;
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goReturn(ndReturn *pReturn, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onReturn, pReturn);
  goExp(pReturn->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goAttribution(ndAttribution *pAttribution, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onAttribution, pAttribution);

  goVar(pAttribution->pVar, pEvents, pShared);
  goExp(pAttribution->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goVar(ndVar *pVar, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onVar, pVar);
  if(pVar->varType == VAR_ARRAY) {
    goExp(pVar->value.address.pPointerExp, pEvents, pShared);
    goExp(pVar->value.address.pInxExp, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goExp(ndExpression *pExp, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
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
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goNewNode(ndNew *pNew, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onNew, pNew);
  goExp(pNew->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goFunctionCallNode(ndFunctionCall *pFunctionCall, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onFunctionCall, pFunctionCall);
  
  goExpListNode(pFunctionCall->pExpList, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goExpListNode(ndExpList *pExpList, DFS_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pExpList == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onExpList, pExpList);

  pList = pExpList->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
    goExp(pStat, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel);
}

void goIfNode(ndIfElse *pIfElse, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onIf, pIfElse);
  goExp(pIfElse->nExpIf, pEvents, pShared);
  
  goStatement(pIfElse->nStatementIf, pEvents, pShared);
	goElseNode(pIfElse, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goElseNode(ndIfElse *pIfElse, DFS_tpEvents *pEvents, void *pShared) {
  if (pIfElse->nStatementElse == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onElse, pIfElse);
	goStatement(pIfElse->nStatementElse, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}

void goWhileNode(ndWhile *pWhile, DFS_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel);
	CALL_EVENT(onWhile, pWhile);
  goExp(pWhile->pExp, pEvents, pShared);
  goStatement(pWhile->pStat, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel);
}
