#include <stdio.h>
#include "ast_tree.h"

typedef struct STK_stScopeStack STK_tpScopeStack;

STK_tpScopeStack *STK_create();
void STK_pushNewScope(STK_tpScopeStack *pStack);
void STK_popScope(STK_tpScopeStack *pStack);
void STK_addToCurrentScope(STK_tpScopeStack *pStack, ndVariable *pVar);
ndVariable *STK_getCurrentReferenceTo(STK_tpScopeStack *pStack, char *name);
