#ifndef AST_TREE_VAR
#define AST_TREE_VAR
#include "ast_tree.h"

enum IDS_enReportTag { IDS_RUNNING, IDS_ALL_REFERENCED, IDS_UNDEFINED, IDS_UNDEFINED_FUNCTION, IDS_FUNCTION_OVERRIDING, IDS_SCOPE_VARIABLE_OVERRIDING };

typedef struct IDS_stReport {
	enum IDS_enReportTag tag;
	union {
		ndVar *pVar;
		ndVariable *pVarDec;
		ndFunctionCall *pFunctionCall;
		ndFunction *pFunction;
	} errorSource;
} IDS_tpReport;

IDS_tpReport *IDS_checkVariablesScopes(ndDeclarations *pDeclarations);

#endif
