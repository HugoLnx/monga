#include "ast_tree.h"

typedef void (*TRA_fcOnDeclarations)(ndDeclarations *pDecs            , void *pShared);
typedef void (*TRA_fcOnDeclaration)(ndDeclaration *pDec               , void *pShared);
typedef void (*TRA_fcOnFunction)(ndFunction* pFunc                    , void *pShared);
typedef void (*TRA_fcOnBlock)(ndBlock *pBlock                         , void *pShared);
typedef void (*TRA_fcOnParameters)(ndParameters *pParameters          , void *pShared);
typedef void (*TRA_fcOnParameter)(ndVariable *pVar                    , void *pShared);
typedef void (*TRA_fcOnVariable)(ndVariable *pVar                     , void *pShared);
typedef void (*TRA_fcOnVariables)(ndVariables *pVariables             , void *pShared);
typedef void (*TRA_fcOnVarDeclarations)(ndVarDeclarations *pVarDecs   , void *pShared);
typedef void (*TRA_fcOnStatements)(ndStatements *pStats               , void *pShared);
typedef void (*TRA_fcOnStatement)(ndStatement *pStat                  , void *pShared);
typedef void (*TRA_fcOnReturn)(ndReturn *pReturn                      , void *pShared);
typedef void (*TRA_fcOnAttribution)(ndAttribution *pAttribution       , void *pShared);
typedef void (*TRA_fcOnVar)(ndVar *pVar                               , void *pShared);
typedef void (*TRA_fcOnExp)(ndExpression *pExp                        , void *pShared);
typedef void (*TRA_fcOnNew)(ndNew *pNew                               , void *pShared);
typedef void (*TRA_fcOnFunctionCall)(ndFunctionCall *pfunctionCall    , void *pShared);
typedef void (*TRA_fcOnExpList)(ndExpList *pExpList                   , void *pShared);
typedef void (*TRA_fcOnIf)(ndIfElse *pNode                            , void *pShared);
typedef void (*TRA_fcOnElse)(ndIfElse *pNode                          , void *pShared);
typedef void (*TRA_fcOnWhile)(ndWhile *pWhile                         , void *pShared);
typedef void (*TRA_fcOnNewLevel)(char *evtName, void *pShared);
typedef void (*TRA_fcOnBackLevel)(char *evtName, void *pShared);

typedef struct TRA_stEvents {
	TRA_fcOnDeclarations onDeclarations;
	TRA_fcOnDeclaration onDeclaration;
  TRA_fcOnFunction onFunction;
  TRA_fcOnBlock onBlock;
  TRA_fcOnParameters onParameters;
  TRA_fcOnParameter onParameter;
  TRA_fcOnVariable onVariable;
  TRA_fcOnVariables onVariables;
  TRA_fcOnVarDeclarations onVarDeclarations;
  TRA_fcOnStatements onStatements;
  TRA_fcOnStatement onStatement;
  TRA_fcOnReturn onReturn;
  TRA_fcOnAttribution onAttribution;
  TRA_fcOnVar onVar;
  TRA_fcOnExp onExp;
  TRA_fcOnNew onNew;
  TRA_fcOnFunctionCall onFunctionCall;
  TRA_fcOnExpList onExpList;
  TRA_fcOnIf onIf;
  TRA_fcOnElse onElse;
  TRA_fcOnWhile onWhile;
  TRA_fcOnNewLevel onNewLevel;
  TRA_fcOnBackLevel onBackLevel;
} TRA_tpEvents;

void TRA_execute(ndDeclarations* pDeclarations, TRA_tpEvents *pEvents, void *pShared);
