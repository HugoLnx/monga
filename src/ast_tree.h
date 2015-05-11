#ifndef AST_TREE_H
#define AST_TREE_H
enum enDeclaration { DEC_FUNCTION, DEC_VARIABLE };
enum enStatType { STAT_BLOCK, STAT_ATTRIBUTION, STAT_FUNCTION_CALL, STAT_RETURN_CALL, STAT_WHILE, STAT_IF };
enum enExpType { EXP_NUMBER, EXP_HEXADECIMAL, EXP_CHAR };
enum enVarType { VAR_ID, VAR_ARRAY };

typedef struct stDeclaration tpDeclaration;
typedef struct stType tpType;
typedef struct stProgramNode ndProgram;
typedef struct stBlockNode ndBlock;
typedef struct stFunctionNode ndFunction;
typedef struct stParametersNode ndParameters;
typedef struct stParameterNode ndParameter;
typedef struct stVariablesNode ndVariables;
typedef struct stVariableNode ndVariable;
typedef struct stVarDeclarationsNode ndVarDeclarations;
typedef struct stStatementsNode ndStatements;
typedef struct stStatementNode ndStatement;
typedef struct stReturnNode ndReturn;
typedef struct stExpressionNode ndExpression;
typedef struct stVarNode ndVar;
typedef struct stAttributionNode ndAttribution;

ndProgram *createProgramNode(tpType *pType, tpDeclaration *pDec);
ndFunction *createFunctionNode(char *name, ndParameters *pParams, ndBlock *pBlock);
ndBlock *createBlockNode(ndVarDeclarations *pVarDecs, ndStatements *pStats);
tpType *newType(int token, int depth);
tpDeclaration *newDeclaration(void *node, enum enDeclaration decType);
ndParameters* createParametersNode(ndParameter* pParam);
ndParameter* createParameterNode(tpType *pType, char *name);
void addParam(ndParameters *pParams, ndParameter *pParam);

ndVariables *createVariablesNode(char *name);
void addVariable(ndVariables *pVars, char *name);

ndVarDeclarations *createVarDeclarations(tpType *pType, ndVariables *pVars);
void addVarDeclaration(ndVarDeclarations *pVarDecs, tpType *pType, ndVariables *pVars);
ndStatements *createStatements(ndStatement *pStat);
void addStatement(ndStatements *pStats, ndStatement *pStat);
ndStatement *createStatement(void *pNode, enum enStatType);

ndReturn *createReturnNode(ndExpression *pExp);
ndAttribution *createAttributionNode(ndVar *pVar, ndExpression *pExp);
ndVar *createVarNode(char *name);
ndVar *createArrayVarNode(ndExpression *ndExp, ndExpression *ndInxExp);
ndExpression *createExpressionIntegerNode(int val, enum enExpType expType);

char *strDup(char *str);
void incrDepth(tpType *pType);



void printProgram();
void printFunction(ndFunction* pFunc, char *ident);
void printBlock(ndBlock *pBlock, char *ident);
void printParameters(ndParameters *pParameters, char *ident);
void printParameter(ndParameter *pParameter, char *ident);
void printVariable(ndVariable *pVar, char *ident);
void printVariables(ndVariables *pVariables, char *ident);
void printVarDeclarations(ndVarDeclarations *pVarDecs, char *ident);
void printStatements(ndStatements *pStats, char *ident);
void printStatement(ndStatement *pStat, char *ident);
void printReturn(ndReturn *pReturn, char *ident);
void printAttribution(ndAttribution *pAttribution, char *ident);
void printVar(ndVar *pVar, char *ident);
void printExp(ndExpression *pExp, char *ident);
char *addIdent(char *ident);
#endif
