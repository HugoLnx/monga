#include "ast_tree.h"

typedef void (*DFS_fcOnDeclarations)(ndDeclarations *pDecs            , void *pShared);
typedef void (*DFS_fcOnDeclaration)(ndDeclaration *pDec               , void *pShared);
typedef void (*DFS_fcOnFunction)(ndFunction* pFunc                    , void *pShared);
typedef void (*DFS_fcOnBlock)(ndBlock *pBlock                         , void *pShared);
typedef void (*DFS_fcOnParameters)(ndParameters *pParameters          , void *pShared);
typedef void (*DFS_fcOnParameter)(ndVariable *pVar                    , void *pShared);
typedef void (*DFS_fcOnVariable)(ndVariable *pVar                     , void *pShared);
typedef void (*DFS_fcOnVariables)(ndVariables *pVariables             , void *pShared);
typedef void (*DFS_fcOnVarDeclarations)(ndVarDeclarations *pVarDecs   , void *pShared);
typedef void (*DFS_fcOnStatements)(ndStatements *pStats               , void *pShared);
typedef void (*DFS_fcOnStatement)(ndStatement *pStat                  , void *pShared);
typedef void (*DFS_fcOnReturn)(ndReturn *pReturn                      , void *pShared);
typedef void (*DFS_fcOnAttribution)(ndAttribution *pAttribution       , void *pShared);
typedef void (*DFS_fcOnVar)(ndVar *pVar                               , void *pShared);
typedef void (*DFS_fcOnExp)(ndExpression *pExp                        , void *pShared);
typedef void (*DFS_fcOnNew)(ndNew *pNew                               , void *pShared);
typedef void (*DFS_fcOnFunctionCall)(ndFunctionCall *pfunctionCall    , void *pShared);
typedef void (*DFS_fcOnExpList)(ndExpList *pExpList                   , void *pShared);
typedef void (*DFS_fcOnIf)(ndIfElse *pNode                            , void *pShared);
typedef void (*DFS_fcOnElse)(ndIfElse *pNode                          , void *pShared);
typedef void (*DFS_fcOnWhile)(ndWhile *pWhile                         , void *pShared);
typedef void (*DFS_fcOnNewLevel)(void *pShared);
typedef void (*DFS_fcOnBackLevel)(void *pShared);

typedef struct DFS_stEvents {
	DFS_fcOnDeclarations onDeclarations;
	DFS_fcOnDeclaration onDeclaration;
  DFS_fcOnFunction onFunction;
  DFS_fcOnBlock onBlock;
  DFS_fcOnParameters onParameters;
  DFS_fcOnParameter onParameter;
  DFS_fcOnVariable onVariable;
  DFS_fcOnVariables onVariables;
  DFS_fcOnVarDeclarations onVarDeclarations;
  DFS_fcOnStatements onStatements;
  DFS_fcOnStatement onStatement;
  DFS_fcOnReturn onReturn;
  DFS_fcOnAttribution onAttribution;
  DFS_fcOnVar onVar;
  DFS_fcOnExp onExp;
  DFS_fcOnNew onNew;
  DFS_fcOnFunctionCall onFunctionCall;
  DFS_fcOnExpList onExpList;
  DFS_fcOnIf onIf;
  DFS_fcOnElse onElse;
  DFS_fcOnWhile onWhile;
  DFS_fcOnNewLevel onNewLevel;
  DFS_fcOnBackLevel onBackLevel;
} DFS_tpEvents;

void DFS_execute(ndDeclarations* pDeclarations, DFS_tpEvents *pEvents, void *pShared);
