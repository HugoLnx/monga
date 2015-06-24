#include "ast_tree.h"

typedef void (*POSTTRA_fcOnDeclarations)(ndDeclarations *pDecs            , void *pShared);
typedef void (*POSTTRA_fcOnDeclaration)(ndDeclaration *pDec               , void *pShared);
typedef void (*POSTTRA_fcOnFunction)(ndFunction* pFunc                    , void *pShared);
typedef void (*POSTTRA_fcOnBlock)(ndBlock *pBlock                         , void *pShared);
typedef void (*POSTTRA_fcOnParameters)(ndParameters *pParameters          , void *pShared);
typedef void (*POSTTRA_fcOnParameter)(ndVariable *pVar                    , void *pShared);
typedef void (*POSTTRA_fcOnVariable)(ndVariable *pVar                     , void *pShared);
typedef void (*POSTTRA_fcOnVariables)(ndVariables *pVariables             , void *pShared);
typedef void (*POSTTRA_fcOnVarDeclarations)(ndVarDeclarations *pVarDecs   , void *pShared);
typedef void (*POSTTRA_fcOnStatements)(ndStatements *pStats               , void *pShared);
typedef void (*POSTTRA_fcOnStatement)(ndStatement *pStat                  , void *pShared);
typedef void (*POSTTRA_fcOnReturn)(ndReturn *pReturn                      , void *pShared);
typedef void (*POSTTRA_fcOnAttribution)(ndAttribution *pAttribution       , void *pShared);
typedef void (*POSTTRA_fcOnVar)(ndVar *pVar                               , void *pShared);
typedef void (*POSTTRA_fcOnExp)(ndExpression *pExp                        , void *pShared);
typedef void (*POSTTRA_fcOnNew)(ndNew *pNew                               , void *pShared);
typedef void (*POSTTRA_fcOnFunctionCall)(ndFunctionCall *pfunctionCall    , void *pShared);
typedef void (*POSTTRA_fcOnExpList)(ndExpList *pExpList                   , void *pShared);
typedef void (*POSTTRA_fcOnIf)(ndIfElse *pNode                            , void *pShared);
typedef void (*POSTTRA_fcOnElse)(ndIfElse *pNode                          , void *pShared);
typedef void (*POSTTRA_fcOnWhile)(ndWhile *pWhile                         , void *pShared);
typedef void (*POSTTRA_fcOnNewLevel)(char *evtName, void *pShared);
typedef void (*POSTTRA_fcOnBackLevel)(char *evtName, void *pShared);

typedef struct POSTTRA_stEvents {
	POSTTRA_fcOnDeclarations onDeclarations;
	POSTTRA_fcOnDeclaration onDeclaration;
  POSTTRA_fcOnFunction onFunction;
  POSTTRA_fcOnBlock onBlock;
  POSTTRA_fcOnParameters onParameters;
  POSTTRA_fcOnParameter onParameter;
  POSTTRA_fcOnVariable onVariable;
  POSTTRA_fcOnVariables onVariables;
  POSTTRA_fcOnVarDeclarations onVarDeclarations;
  POSTTRA_fcOnStatements onStatements;
  POSTTRA_fcOnStatement onStatement;
  POSTTRA_fcOnReturn onReturn;
  POSTTRA_fcOnAttribution onAttribution;
  POSTTRA_fcOnVar onVar;
  POSTTRA_fcOnExp onExp;
  POSTTRA_fcOnNew onNew;
  POSTTRA_fcOnFunctionCall onFunctionCall;
  POSTTRA_fcOnExpList onExpList;
  POSTTRA_fcOnIf onIf;
  POSTTRA_fcOnElse onElse;
  POSTTRA_fcOnWhile onWhile;
  POSTTRA_fcOnNewLevel onNewLevel;
  POSTTRA_fcOnBackLevel onBackLevel;
} POSTTRA_tpEvents;

void POSTTRA_execute(ndDeclarations* pDeclarations, POSTTRA_tpEvents *pEvents, void *pShared);
