#include "ast_tree.h"

typedef void (*PRETRA_fcOnDeclarations)(ndDeclarations *pDecs            , void *pShared);
typedef void (*PRETRA_fcOnDeclaration)(ndDeclaration *pDec               , void *pShared);
typedef void (*PRETRA_fcOnFunction)(ndFunction* pFunc                    , void *pShared);
typedef void (*PRETRA_fcOnBlock)(ndBlock *pBlock                         , void *pShared);
typedef void (*PRETRA_fcOnParameters)(ndParameters *pParameters          , void *pShared);
typedef void (*PRETRA_fcOnParameter)(ndVariable *pVar                    , void *pShared);
typedef void (*PRETRA_fcOnVariable)(ndVariable *pVar                     , void *pShared);
typedef void (*PRETRA_fcOnVariables)(ndVariables *pVariables             , void *pShared);
typedef void (*PRETRA_fcOnVarDeclarations)(ndVarDeclarations *pVarDecs   , void *pShared);
typedef void (*PRETRA_fcOnStatements)(ndStatements *pStats               , void *pShared);
typedef void (*PRETRA_fcOnStatement)(ndStatement *pStat                  , void *pShared);
typedef void (*PRETRA_fcOnReturn)(ndReturn *pReturn                      , void *pShared);
typedef void (*PRETRA_fcOnAttribution)(ndAttribution *pAttribution       , void *pShared);
typedef void (*PRETRA_fcOnVar)(ndVar *pVar                               , void *pShared);
typedef void (*PRETRA_fcOnExp)(ndExpression *pExp                        , void *pShared);
typedef void (*PRETRA_fcOnNew)(ndNew *pNew                               , void *pShared);
typedef void (*PRETRA_fcOnFunctionCall)(ndFunctionCall *pfunctionCall    , void *pShared);
typedef void (*PRETRA_fcOnExpList)(ndExpList *pExpList                   , void *pShared);
typedef void (*PRETRA_fcOnIf)(ndIfElse *pNode                            , void *pShared);
typedef void (*PRETRA_fcOnElse)(ndIfElse *pNode                          , void *pShared);
typedef void (*PRETRA_fcOnWhile)(ndWhile *pWhile                         , void *pShared);
typedef void (*PRETRA_fcOnNewLevel)(char *evtName, void *pShared);
typedef void (*PRETRA_fcOnBackLevel)(char *evtName, void *pShared);

typedef struct PRETRA_stEvents {
	PRETRA_fcOnDeclarations onDeclarations;
	PRETRA_fcOnDeclaration onDeclaration;
  PRETRA_fcOnFunction onFunction;
  PRETRA_fcOnBlock onBlock;
  PRETRA_fcOnParameters onParameters;
  PRETRA_fcOnParameter onParameter;
  PRETRA_fcOnVariable onVariable;
  PRETRA_fcOnVariables onVariables;
  PRETRA_fcOnVarDeclarations onVarDeclarations;
  PRETRA_fcOnStatements onStatements;
  PRETRA_fcOnStatement onStatement;
  PRETRA_fcOnReturn onReturn;
  PRETRA_fcOnAttribution onAttribution;
  PRETRA_fcOnVar onVar;
  PRETRA_fcOnExp onExp;
  PRETRA_fcOnNew onNew;
  PRETRA_fcOnFunctionCall onFunctionCall;
  PRETRA_fcOnExpList onExpList;
  PRETRA_fcOnIf onIf;
  PRETRA_fcOnElse onElse;
  PRETRA_fcOnWhile onWhile;
  PRETRA_fcOnNewLevel onNewLevel;
  PRETRA_fcOnBackLevel onBackLevel;
} PRETRA_tpEvents;

void PRETRA_execute(ndDeclarations* pDeclarations, PRETRA_tpEvents *pEvents, void *pShared);
