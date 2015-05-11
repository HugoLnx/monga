#ifndef AST_TREE
#define AST_TREE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NEW(type) (type*) malloc(sizeof(type))

enum enDeclaration { DEC_FUNCTION, DEC_VARIABLE };

typedef struct stDeclaration {
	enum enDeclaration decType;
	void *pNode;
} tpDeclaration;

typedef struct stType {
	int token;
	int depth;
} tpType;

typedef struct stProgramNode {
	tpDeclaration *pDec;
} ndProgram;

typedef struct stBlockNode {
} ndBlock;

typedef struct stFunctionNode {
	char *name;
	ndBlock *pBlock;
	tpType *pReturnType;
} ndFunction;

ndProgram *createProgramNode(tpType *pType, tpDeclaration *pDec);
ndFunction *createFunctionNode(char *name, ndBlock *pBlock);
ndBlock *createBlockNode();
tpType *newType(int token, int depth);
tpDeclaration *newDeclaration(void *node, enum enDeclaration decType);
char *strDup(char *str);

ndProgram *pProgram = NULL;
 
char *strDup(char *str) {
	char* dup = (char*) malloc(sizeof(char)*(strlen(str)+1));
	strcpy(dup, str);
	return dup;
}

ndProgram *createProgramNode(tpType *pType, tpDeclaration *pDec) {
	pProgram = NEW(ndProgram);
	if (pDec->decType == DEC_FUNCTION) {
		ndFunction *pFunc = (ndFunction*) pDec->pNode;
		pFunc->pReturnType = pType;
	}

	pProgram->pDec = pDec;
	return pProgram;
}

ndFunction *createFunctionNode(char *name, ndBlock *pBlock) {
	ndFunction *pFunc = NEW(ndFunction);
	pFunc->name = strDup(name);
	pFunc->pBlock = pBlock;
	return pFunc;
}

ndBlock *createBlockNode() {
	return NEW(ndBlock);
}

tpType *newType(int token, int depth) {
	tpType *pType = NEW(tpType);
	pType->token = token;
	pType->depth = depth;
	return pType;
}

tpDeclaration *newDeclaration(void *node, enum enDeclaration decType) {
	tpDeclaration *pDec = NEW(tpDeclaration);
	pDec->decType = decType;
	pDec->pNode = node;
	return pDec;
}

void printProgram();
void printFunction(ndFunction* pFunc, char *ident);
void printBlock(ndBlock *pBlock, char *ident);
char *addIdent(char *ident);

void printProgram() {
	tpDeclaration* pDec = pProgram->pDec;
	printf("program:\n");
	if(pDec->decType == DEC_FUNCTION) {
		printFunction((ndFunction*) pDec->pNode, strdup("  "));
	}
}

void printFunction(ndFunction* pFunc, char *ident) {
	tpType* pReturn = pFunc->pReturnType;
	printf("%sfunction:%s,%d,%d\n", ident, pFunc->name,pReturn->token,pReturn->depth);
	printBlock(pFunc->pBlock, addIdent(ident));
}

void printBlock(ndBlock *pBlock, char *ident) {
	printf("%sblock:\n", ident);
}

char *addIdent(char *ident) {
	char *newIdent = (char*) malloc(sizeof(char)*(strlen(ident)+3));
	memset(newIdent, ' ', strlen(ident)+2);
	newIdent[strlen(ident)+3] = '\0';
	return newIdent;
}
#endif
