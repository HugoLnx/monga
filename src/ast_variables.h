#ifndef AST_TREE_VAR
#define AST_TREE_VAR
#include "ast_tree.h"

enum VAR_enReportType { VAR_RUNNING, VAR_ALL_REFERENCED, VAR_UNDEFINED };

typedef struct VAR_stVarResume {
  int depth;
  char *name;
  ndVar *pVarTop;
} VAR_tpVarResume;

typedef struct VAR_stExpResume {
	tpType *pType;	
  ndExpression *pExpTop;
} VAR_tpExpResume;

typedef struct VAR_stReport {
	enum VAR_enReportType type;
	VAR_tpVarResume *pVarResume;
	VAR_tpExpResume *pExpResume;
} VAR_tpReport;

VAR_tpReport *VAR_checkVariablesScopes(ndDeclarations *pDeclarations);

#endif
