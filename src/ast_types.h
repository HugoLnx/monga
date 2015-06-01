#ifndef AST_TREE_TYP
#define AST_TREE_TYP
#include "ast_tree.h"

enum TYP_enReportType { TYP_RUNNING, TYP_WELL_TYPED, TYP_UNMATCH };

typedef struct TYP_stExpResume {
	tpType *pType;	
  ndExpression *pExpTop;
} TYP_tpExpResume;

typedef struct TYP_stReport {
	enum TYP_enReportType type;
	tpVarBackDeclaration *pVarBack;
	TYP_tpExpResume *pExpResume;
} TYP_tpReport;

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations);

#endif
