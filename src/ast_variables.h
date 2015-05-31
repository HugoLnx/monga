#ifndef AST_TREE_VAR
#define AST_TREE_VAR
#include "ast_tree.h"

enum VAR_enReportType { RUNNING, CORRECT, UNMATCH_TYPE, UNDEFINED_VARIABLE };

typedef struct VAR_stVarResume {
  int depth;
  char *name;
  ndVar *pVarTop;
} VAR_tpVarResume;

typedef struct VAR_stReport {
	enum VAR_enReportType type;
	VAR_tpVarResume *pVarResume;
	ndVariable *pVariable;
} VAR_tpReport;

VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations);

#endif
