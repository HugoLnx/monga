#include <stdio.h>
#include "ast_tree.h"
#include "ast_ids.h"
#include "ast_types.h"
#include "error_reporter.h"

extern ndDeclarations *pDeclarations;

int main()
{
	yyparse();
  IDS_tpReport *pVarReport = IDS_checkVariablesScopes(pDeclarations);
	if (ERR_printErrorsOnVarReport(pVarReport) == 0) {
		TYP_tpReport *pTypReport = TYP_checkMatchingTypes(pDeclarations);
		if (ERR_printErrorsOnTypesReport(pTypReport) == 0) {
			printTree(pDeclarations);
		}
	}
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

