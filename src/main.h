#ifndef MAIN_H
#define MAIN_H
#include "./ast_tree.h"

typedef union {
 	long long int ival;
 	char *text;
  double fval;
  ndProgram* pProgram;
  ndFunction* pFunction;
  ndBlock* pBlock;
  tpType* pType;
  tpDeclaration* pDec;
} YYSTYPE;

YYSTYPE yylval;
#endif
