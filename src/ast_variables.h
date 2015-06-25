#ifndef AST_TREE_VAR
#define AST_TREE_VAR
#include "ast_tree.h"

enum VAR_enReportTag { VAR_RUNNING, VAR_ALL_REFERENCED, VAR_UNDEFINED, VAR_UNDEFINED_FUNCTION, VAR_FUNCTION_OVERRIDING, VAR_SCOPE_VARIABLE_OVERRIDING };

typedef struct VAR_stReport {
	enum VAR_enReportTag tag;
	union {
		ndVar *pVar;
		ndVariable *pVarDec;
		ndFunctionCall *pFunctionCall;
		ndFunction *pFunction;
	} errorSource;
} VAR_tpReport;

VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations);

#endif
