#include <stdio.h>
#include "ast_tree.h"


struct stScopeStack {
  struct stNode *pStackVariables;
  struct stNode *pStackCountVariables;
};
typedef struct stScopeStack tpScopeStack;

tpScopeStack *createStack();
void pushNewScope(tpScopeStack *pStack);
void popScope(tpScopeStack *pStack);
void addToCurrentScope(tpScopeStack *pStack, ndVariable *pVar);
ndVariable *getCurrentReferenceTo(tpScopeStack *pStack, char *name);