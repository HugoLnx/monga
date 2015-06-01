#ifndef AST_TREE_TYP
#define AST_TREE_TYP
#include "ast_tree.h"

enum TYP_enReportType { RUNNING, WELL_TYPED, UNMATCH_TYPE };

typedef struct TYP_stExpResume {
	tpType *pType;	
  ndExpression *pExpTop;
} TYP_tpExpResume;

typedef struct TYP_stReport {
	enum TYP_enReportType type;
	tpVarBackDeclaration *pVarDec;
	TYP_tpExpResume *pExpResume;
} TYP_tpReport;

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations);

#endif
