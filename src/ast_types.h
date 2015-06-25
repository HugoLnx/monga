#ifndef AST_TREE_TYP
#define AST_TREE_TYP
#include "ast_tree.h"

enum TYP_enReportTag { TYP_RUNNING, TYP_WELL_TYPED, TYP_UNMATCH, TYP_NO_ARITHMETIC_TYPE, TYP_NO_VALID_ARRAY_INDEX };

typedef struct TYP_stReport {
	enum TYP_enReportTag tag;
	tpVarBackDeclaration *pVarBack;
	ndExpression *pExp;
} TYP_tpReport;

TYP_tpReport *TYP_checkMatchingTypes(ndDeclarations *pDeclarations);

#endif
