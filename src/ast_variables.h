#ifndef AST_TREE_VAR
#define AST_TREE_VAR
#include "ast_tree.h"

enum VAR_enReportTag { VAR_RUNNING, VAR_ALL_REFERENCED, VAR_UNDEFINED };

typedef struct VAR_stReport {
	enum VAR_enReportTag tag;
	ndVar *pVar;
} VAR_tpReport;

VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations);

#endif
