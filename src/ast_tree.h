#ifndef AST_TREE_H
#define AST_TREE_H
enum enDeclaration { DEC_FUNCTION, DEC_VARIABLE };

typedef struct stDeclaration tpDeclaration;
typedef struct stType tpType;
typedef struct stProgramNode ndProgram;
typedef struct stBlockNode ndBlock;
typedef struct stFunctionNode ndFunction;
typedef struct stParametersNode ndParameters;
typedef struct stParameterNode ndParameter;
typedef struct stVariablesNode ndVariables;
typedef struct stVariableNode ndVariable;

ndProgram *createProgramNode(tpType *pType, tpDeclaration *pDec);
ndFunction *createFunctionNode(char *name, ndParameters *pParams, ndBlock *pBlock);
ndBlock *createBlockNode();
tpType *newType(int token, int depth);
tpDeclaration *newDeclaration(void *node, enum enDeclaration decType);
ndParameters* createParametersNode(ndParameter* pParam);
ndParameter* createParameterNode(tpType *pType, char *name);
void addParam(ndParameters *pParams, ndParameter *pParam);

ndVariables *createVariablesNode(char *name);
void addVariable(ndVariables *pVars, char *name);

char *strDup(char *str);
void incrDepth(tpType *pType);



void printProgram();
void printFunction(ndFunction* pFunc, char *ident);
void printBlock(ndBlock *pBlock, char *ident);
void printParameters(ndParameters *pParameters, char *ident);
void printParameter(ndParameter *pParameter, char *ident);
void printVariable(ndVariable *pVar, char *ident);
void printVariables(ndVariables *pVariables, char *ident);
char *addIdent(char *ident);
#endif
