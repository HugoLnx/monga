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
	void *pNode;
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
 
char *strDup(char *str) {
	char* dup = (char*) malloc(sizeof(char)*(strlen(str)+1));
	strcpy(dup, str);
	return dup;
}

ndProgram *createProgramNode(tpType *pType, tpDeclaration *pDec) {
	ndProgram *pProgram = NEW(ndProgram);
	if (pDec->decType == DEC_FUNCTION) {
		ndFunction *pFunc = (ndFunction*) pDec->pNode;
		pFunc->pReturnType = pType;
	}

	pProgram->pNode = pDec->pNode;
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
