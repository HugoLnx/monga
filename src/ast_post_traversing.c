#include "ast_post_traversing.h"
#define CALL_EVENT(evtName, evtVar) if (pEvents->evtName != NULL) pEvents->evtName(evtVar, pShared)
#define CALL_LEVEL_EVENT(evtName,relatedEvt) if (pEvents->evtName != NULL) pEvents->evtName(#relatedEvt, pShared)

static void goDeclarations(ndDeclarations *pDecs             ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goDeclaration(ndDeclaration *pDec                ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goFunction(ndFunction* pFunc                     ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goBlock(ndBlock *pBlock                          ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goParameters(ndParameters *pParameters           ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goParameter(ndVariable *pVar                     ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goVariable(ndVariable *pVar                      ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goVariables(ndVariables *pVariables              ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goVarDeclarations(ndVarDeclarations *pVarDecs    ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goStatements(ndStatements *pStats                ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goStatement(ndStatement *pStat                   ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goReturn(ndReturn *pReturn                       ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goAttribution(ndAttribution *pAttribution        ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goVar(ndVar *pVar                                ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goExp(ndExpression *pExp                         ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goNewNode(ndNew *pNew                            ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goFunctionCallNode(ndFunctionCall *pfunctionCall ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goExpListNode(ndExpList *pExpList                ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goIfNode(ndIfElse *pNode                         ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goElseNode(ndIfElse *pNode                       ,POSTTRA_tpEvents *pEvents, void *pShared);
static void goWhileNode(ndWhile *pWhile                      ,POSTTRA_tpEvents *pEvents, void *pShared);


void POSTTRA_execute(ndDeclarations* pDeclarations, POSTTRA_tpEvents *pEvents, void *pShared) {
	goDeclarations(pDeclarations, pEvents, pShared);
}

static void goDeclarations(ndDeclarations *pDeclarations, POSTTRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pDeclarations == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onDeclarations);
  pList = pDeclarations->pList;
  resetList(pList);

  while(goPrevious(pList)) {
    ndDeclaration *pDec = (ndDeclaration*) getCurrentValue(pList);
    goDeclaration(pDec, pEvents, pShared);
  }
	CALL_EVENT(onDeclarations, pDeclarations);
	CALL_LEVEL_EVENT(onBackLevel,onDeclarations);
}

static void goDeclaration(ndDeclaration *pDec, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onDeclaration);
	if(pDec->decTag == DEC_FUNCTION) {
		goFunction((ndFunction*) pDec->pNode, pEvents, pShared);
	} else {
		goVariables((ndVariables*) pDec->pNode, pEvents, pShared);
  }
	CALL_EVENT(onDeclaration, pDec);
	CALL_LEVEL_EVENT(onBackLevel,onDeclaration);
}

static void goFunction(ndFunction* pFunc, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onFunction);

	goParameters(pFunc->pParameters, pEvents, pShared);
	goBlock(pFunc->pBlock, pEvents, pShared);
	CALL_EVENT(onFunction, pFunc);
	CALL_LEVEL_EVENT(onBackLevel,onFunction);
}

static void goParameters(ndParameters *pParameters, POSTTRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pParameters == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onParameters);
  pList = pParameters->pList;

  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    goParameter(pVar, pEvents, pShared);
  }
	CALL_EVENT(onParameters, pParameters);
	CALL_LEVEL_EVENT(onBackLevel,onParameters);
}

static void goParameter(ndVariable *pVar, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onParameter);
	CALL_EVENT(onParameter, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onParameter);
}

static void goVariables(ndVariables *pVariables, POSTTRA_tpEvents *pEvents, void *pShared) {
  tpList *pList = pVariables->pList;
	CALL_LEVEL_EVENT(onNewLevel,onVariables);

  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
    goVariable(pVar, pEvents, pShared);
  }
	CALL_EVENT(onVariables, pVariables);
	CALL_LEVEL_EVENT(onBackLevel,onVariables);
}

static void goVariable(ndVariable *pVar, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onVariable);
	CALL_EVENT(onVariable, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onVariable);
}

static void goBlock(ndBlock *pBlock, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onBlock);
  goVarDeclarations(pBlock->pVarDecs, pEvents, pShared);
  goStatements(pBlock->pStats, pEvents, pShared);
	CALL_EVENT(onBlock, pBlock);
	CALL_LEVEL_EVENT(onBackLevel,onBlock);
}

static void goVarDeclarations(ndVarDeclarations *pVarDecs, POSTTRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pVarDecs == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onVarDeclarations);

  pList = pVarDecs->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariables *pVars = (ndVariables*) getCurrentValue(pList);
    goVariables(pVars, pEvents, pShared);
  }
	CALL_EVENT(onVarDeclarations, pVarDecs);
	CALL_LEVEL_EVENT(onBackLevel,onVarDeclarations);
}

static void goStatements(ndStatements *pStats, POSTTRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pStats == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onStatements);

  pList = pStats->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndStatement *pStat = (ndStatement*) getCurrentValue(pList);
    goStatement(pStat, pEvents, pShared);
  }
	CALL_EVENT(onStatements, pStats);
	CALL_LEVEL_EVENT(onBackLevel,onStatements);
}

static void goStatement(ndStatement *pStat, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onStatement);
  switch(pStat->statTag){
    case(STAT_BLOCK): goBlock((ndBlock*) pStat->pNode, pEvents, pShared); break;
    case(STAT_ATTRIBUTION): goAttribution((ndAttribution*) pStat->pNode, pEvents, pShared);break;
    case(STAT_RETURN_CALL): goReturn((ndReturn*) pStat->pNode, pEvents, pShared);break;
    case(STAT_FUNCTION_CALL): goFunctionCallNode((ndFunctionCall*) pStat->pNode, pEvents, pShared);break;
    case(STAT_IF): goIfNode((ndIfElse*) pStat->pNode, pEvents, pShared);break;
    case(STAT_WHILE): goWhileNode((ndWhile*) pStat->pNode, pEvents, pShared);break;
  }
	CALL_EVENT(onStatement, pStat);
	CALL_LEVEL_EVENT(onBackLevel,onStatement);
}

static void goReturn(ndReturn *pReturn, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onReturn);
  goExp(pReturn->pExp, pEvents, pShared);
	CALL_EVENT(onReturn, pReturn);
	CALL_LEVEL_EVENT(onBackLevel,onReturn);
}

static void goAttribution(ndAttribution *pAttribution, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onAttribution);

  goVar(pAttribution->pVar, pEvents, pShared);
  goExp(pAttribution->pExp, pEvents, pShared);
	CALL_EVENT(onAttribution, pAttribution);
	CALL_LEVEL_EVENT(onBackLevel,onAttribution);
}

static void goVar(ndVar *pVar, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onVar);
  if(pVar->varTag == VAR_ARRAY) {
    goExp(pVar->value.address.pPointerExp, pEvents, pShared);
    goExp(pVar->value.address.pInxExp, pEvents, pShared);
  }
	CALL_EVENT(onVar, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onVar);
}

static void goExp(ndExpression *pExp, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onExp);
  switch(pExp->expTag) {
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
	CALL_EVENT(onExp, pExp);
	CALL_LEVEL_EVENT(onBackLevel,onExp);
}

static void goNewNode(ndNew *pNew, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onNew);
  goExp(pNew->pExp, pEvents, pShared);
	CALL_EVENT(onNew, pNew);
	CALL_LEVEL_EVENT(onBackLevel,onNew);
}

static void goFunctionCallNode(ndFunctionCall *pFunctionCall, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onFunctionCall);
  
  goExpListNode(pFunctionCall->pExpList, pEvents, pShared);
	CALL_EVENT(onFunctionCall, pFunctionCall);
	CALL_LEVEL_EVENT(onBackLevel,onFunctionCall);
}

static void goExpListNode(ndExpList *pExpList, POSTTRA_tpEvents *pEvents, void *pShared) {
  tpList *pList;
  if(pExpList == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onExpList);

  pList = pExpList->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
    goExp(pStat, pEvents, pShared);
  }
	CALL_EVENT(onExpList, pExpList);
	CALL_LEVEL_EVENT(onBackLevel,onExpList);
}

static void goIfNode(ndIfElse *pIfElse, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onIf);
  goExp(pIfElse->nExpIf, pEvents, pShared);
  
  goStatement(pIfElse->nStatementIf, pEvents, pShared);
	goElseNode(pIfElse, pEvents, pShared);
	CALL_EVENT(onIf, pIfElse);
	CALL_LEVEL_EVENT(onBackLevel,onIf);
}

static void goElseNode(ndIfElse *pIfElse, POSTTRA_tpEvents *pEvents, void *pShared) {
  if (pIfElse->nStatementElse == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onElse);
	goStatement(pIfElse->nStatementElse, pEvents, pShared);
	CALL_EVENT(onElse, pIfElse);
	CALL_LEVEL_EVENT(onBackLevel,onElse);
}

static void goWhileNode(ndWhile *pWhile, POSTTRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onWhile);
  goExp(pWhile->pExp, pEvents, pShared);
  goStatement(pWhile->pStat, pEvents, pShared);
	CALL_EVENT(onWhile, pWhile);
	CALL_LEVEL_EVENT(onBackLevel,onWhile);
}
