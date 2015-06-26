#include "ast_pre_traversing.h"
#define CALL_EVENT(evtName, evtVar) if (pEvents->evtName != NULL) pEvents->evtName(evtVar, pShared)
#define CALL_LEVEL_EVENT(evtName,relatedEvt) if (pEvents->evtName != NULL) pEvents->evtName(#relatedEvt, pShared)

static void goDeclarations(ndDeclarations *pDecs             ,PRETRA_tpEvents *pEvents, void *pShared);
static void goDeclaration(ndDeclaration *pDec                ,PRETRA_tpEvents *pEvents, void *pShared);
static void goFunction(ndFunction* pFunc                     ,PRETRA_tpEvents *pEvents, void *pShared);
static void goBlock(ndBlock *pBlock                          ,PRETRA_tpEvents *pEvents, void *pShared);
static void goParameters(ndParameters *pParameters           ,PRETRA_tpEvents *pEvents, void *pShared);
static void goParameter(ndVariable *pVar                     ,PRETRA_tpEvents *pEvents, void *pShared);
static void goVariable(ndVariable *pVar                      ,PRETRA_tpEvents *pEvents, void *pShared);
static void goVariables(ndVariables *pVariables              ,PRETRA_tpEvents *pEvents, void *pShared);
static void goVarDeclarations(ndVarDeclarations *pVarDecs    ,PRETRA_tpEvents *pEvents, void *pShared);
static void goStatements(ndStatements *pStats                ,PRETRA_tpEvents *pEvents, void *pShared);
static void goStatement(ndStatement *pStat                   ,PRETRA_tpEvents *pEvents, void *pShared);
static void goReturn(ndReturn *pReturn                       ,PRETRA_tpEvents *pEvents, void *pShared);
static void goAttribution(ndAttribution *pAttribution        ,PRETRA_tpEvents *pEvents, void *pShared);
static void goVar(ndVar *pVar                                ,PRETRA_tpEvents *pEvents, void *pShared);
static void goExp(ndExpression *pExp                         ,PRETRA_tpEvents *pEvents, void *pShared);
static void goNewNode(ndNew *pNew                            ,PRETRA_tpEvents *pEvents, void *pShared);
static void goFunctionCallNode(ndFunctionCall *pfunctionCall ,PRETRA_tpEvents *pEvents, void *pShared);
static void goExpListNode(ndExpList *pExpList                ,PRETRA_tpEvents *pEvents, void *pShared);
static void goIfNode(ndIfElse *pNode                         ,PRETRA_tpEvents *pEvents, void *pShared);
static void goElseNode(ndIfElse *pNode                       ,PRETRA_tpEvents *pEvents, void *pShared);
static void goWhileNode(ndWhile *pWhile                      ,PRETRA_tpEvents *pEvents, void *pShared);


void PRETRA_execute(ndDeclarations* pDeclarations, PRETRA_tpEvents *pEvents, void *pShared) {
	goDeclarations(pDeclarations, pEvents, pShared);
}

static void goDeclarations(ndDeclarations *pDeclarations ,PRETRA_tpEvents *pEvents, void *pShared) {
  LIS_tpList *pList;
  if(pDeclarations == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onDeclarations);
	CALL_EVENT(onDeclarations, pDeclarations);
  pList = pDeclarations->pList;
  LIS_reset(pList);

  while(LIS_goPrevious(pList)) {
    ndDeclaration *pDec = (ndDeclaration*) LIS_getCurrentValue(pList);
    goDeclaration(pDec, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onDeclarations);
}

static void goDeclaration(ndDeclaration *pDec, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onDeclaration);
	CALL_EVENT(onDeclaration, pDec);
	if(pDec->decTag == DEC_FUNCTION) {
		goFunction((ndFunction*) pDec->pNode, pEvents, pShared);
	} else {
		goVariables((ndVariables*) pDec->pNode, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onDeclaration);
}

static void goFunction(ndFunction* pFunc, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onFunction);
	CALL_EVENT(onFunction, pFunc);

	goParameters(pFunc->pParameters, pEvents, pShared);
	goBlock(pFunc->pBlock, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onFunction);
}

static void goParameters(ndParameters *pParameters, PRETRA_tpEvents *pEvents, void *pShared) {
  LIS_tpList *pList;
  if(pParameters == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onParameters);
	CALL_EVENT(onParameters, pParameters);
  pList = pParameters->pList;

  LIS_reset(pList);
  while(LIS_goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) LIS_getCurrentValue(pList);
    goParameter(pVar, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onParameters);
}

static void goParameter(ndVariable *pVar, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onParameter);
	CALL_EVENT(onParameter, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onParameter);
}

static void goVariables(ndVariables *pVariables, PRETRA_tpEvents *pEvents, void *pShared) {
  LIS_tpList *pList = pVariables->pList;
	CALL_LEVEL_EVENT(onNewLevel,onVariables);
	CALL_EVENT(onVariables, pVariables);

  LIS_reset(pList);
  while(LIS_goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) LIS_getCurrentValue(pList);
    goVariable(pVar, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onVariables);
}

static void goVariable(ndVariable *pVar, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onVariable);
	CALL_EVENT(onVariable, pVar);
	CALL_LEVEL_EVENT(onBackLevel,onVariable);
}

static void goBlock(ndBlock *pBlock ,PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onBlock);
	CALL_EVENT(onBlock, pBlock);
  goVarDeclarations(pBlock->pVarDecs, pEvents, pShared);
  goStatements(pBlock->pStats, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onBlock);
}

static void goVarDeclarations(ndVarDeclarations *pVarDecs, PRETRA_tpEvents *pEvents, void *pShared) {
  LIS_tpList *pList;
  if(pVarDecs == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onVarDeclarations);
	CALL_EVENT(onVarDeclarations, pVarDecs);

  pList = pVarDecs->pList;
  LIS_reset(pList);
  while(LIS_goPrevious(pList)) {
    ndVariables *pVars = (ndVariables*) LIS_getCurrentValue(pList);
    goVariables(pVars, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onVarDeclarations);
}

static void goStatements(ndStatements *pStats, PRETRA_tpEvents *pEvents, void *pShared) {
  LIS_tpList *pList;
  if(pStats == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onStatements);
	CALL_EVENT(onStatements, pStats);

  pList = pStats->pList;
  LIS_reset(pList);
  while(LIS_goPrevious(pList)) {
    ndStatement *pStat = (ndStatement*) LIS_getCurrentValue(pList);
    goStatement(pStat, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onStatements);
}

static void goStatement(ndStatement *pStat, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onStatement);
	CALL_EVENT(onStatement, pStat);
  switch(pStat->statTag){
    case(STAT_BLOCK): goBlock((ndBlock*) pStat->pNode, pEvents, pShared); break;
    case(STAT_ATTRIBUTION): goAttribution((ndAttribution*) pStat->pNode, pEvents, pShared);break;
    case(STAT_RETURN_CALL): goReturn((ndReturn*) pStat->pNode, pEvents, pShared);break;
    case(STAT_FUNCTION_CALL): goFunctionCallNode((ndFunctionCall*) pStat->pNode, pEvents, pShared);break;
    case(STAT_IF): goIfNode((ndIfElse*) pStat->pNode, pEvents, pShared);break;
    case(STAT_WHILE): goWhileNode((ndWhile*) pStat->pNode, pEvents, pShared);break;
  }
	CALL_LEVEL_EVENT(onBackLevel,onStatement);
}

static void goReturn(ndReturn *pReturn, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onReturn);
	CALL_EVENT(onReturn, pReturn);
  goExp(pReturn->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onReturn);
}

static void goAttribution(ndAttribution *pAttribution, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onAttribution);
	CALL_EVENT(onAttribution, pAttribution);

  goVar(pAttribution->pVar, pEvents, pShared);
  goExp(pAttribution->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onAttribution);
}

static void goVar(ndVar *pVar, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onVar);
	CALL_EVENT(onVar, pVar);
  if(pVar->varTag == IDS_ARRAY) {
    goExp(pVar->value.address.pPointerExp, pEvents, pShared);
    goExp(pVar->value.address.pInxExp, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onVar);
}

static void goExp(ndExpression *pExp, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onExp);
	CALL_EVENT(onExp, pExp);
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
	CALL_LEVEL_EVENT(onBackLevel,onExp);
}

static void goNewNode(ndNew *pNew, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onNew);
	CALL_EVENT(onNew, pNew);
  goExp(pNew->pExp, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onNew);
}

static void goFunctionCallNode(ndFunctionCall *pFunctionCall, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onFunctionCall);
	CALL_EVENT(onFunctionCall, pFunctionCall);
  
  goExpListNode(pFunctionCall->pExpList, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onFunctionCall);
}

static void goExpListNode(ndExpList *pExpList, PRETRA_tpEvents *pEvents, void *pShared) {
  LIS_tpList *pList;
  if(pExpList == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onExpList);
	CALL_EVENT(onExpList, pExpList);

  pList = pExpList->pList;
  LIS_reset(pList);
  while(LIS_goPrevious(pList)) {
    ndExpression *pStat = (ndExpression*) LIS_getCurrentValue(pList);
    goExp(pStat, pEvents, pShared);
  }
	CALL_LEVEL_EVENT(onBackLevel,onExpList);
}

static void goIfNode(ndIfElse *pIfElse, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onIf);
	CALL_EVENT(onIf, pIfElse);
  goExp(pIfElse->nExpIf, pEvents, pShared);
  
  goStatement(pIfElse->nStatementIf, pEvents, pShared);
	goElseNode(pIfElse, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onIf);
}

static void goElseNode(ndIfElse *pIfElse, PRETRA_tpEvents *pEvents, void *pShared) {
  if (pIfElse->nStatementElse == NULL) return;
	CALL_LEVEL_EVENT(onNewLevel,onElse);
	CALL_EVENT(onElse, pIfElse);
	goStatement(pIfElse->nStatementElse, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onElse);
}

static void goWhileNode(ndWhile *pWhile, PRETRA_tpEvents *pEvents, void *pShared) {
	CALL_LEVEL_EVENT(onNewLevel,onWhile);
	CALL_EVENT(onWhile, pWhile);
  goExp(pWhile->pExp, pEvents, pShared);
  goStatement(pWhile->pStat, pEvents, pShared);
	CALL_LEVEL_EVENT(onBackLevel,onWhile);
}
