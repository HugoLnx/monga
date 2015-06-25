#ifndef AST_TREE_H
#define AST_TREE_H
#include "list.h"
enum enDeclaration { DEC_FUNCTION, DEC_VARIABLE };
enum enStatTag { STAT_BLOCK, STAT_ATTRIBUTION, STAT_FUNCTION_CALL, STAT_RETURN_CALL, STAT_WHILE, STAT_IF };

enum enExpTag { EXP_NUMBER, EXP_HEXADECIMAL, EXP_CHAR, EXP_FLOAT, EXP_TEXT, EXPND_VAR, EXPND_NEW, EXPND_BIN, EXPND_EXCLAMATION, EXPND_MINUS, EXPND_CALL};

enum enExpBinTag { EXPBIN_PLUS, EXPBIN_MINUS, EXPBIN_ASTERISK, EXPBIN_SLASH, EXPBIN_AND, EXPBIN_DOUBLE_EQUAL, EXPBIN_BANG_EQUAL, EXPBIN_LESS_EQUAL, EXPBIN_GREATER_EQUAL, EXPBIN_LESS, EXPBIN_GREATER, EXPBIN_OR };

enum enVarTag { IDS_ID, IDS_ARRAY };

typedef struct stDeclaration ndDeclaration;
typedef struct stDeclarations ndDeclarations;
typedef struct stType tpType;
typedef struct stBlockNode ndBlock;
typedef struct stFunctionNode ndFunction;
typedef struct stParametersNode ndParameters;
typedef struct stVariablesNode ndVariables;
typedef struct stVariableNode ndVariable;
typedef struct stVarDeclarationsNode ndVarDeclarations;
typedef struct stStatementsNode ndStatements;
typedef struct stStatementNode ndStatement;
typedef struct stReturnNode ndReturn;
typedef struct stExpressionNode ndExpression;
typedef struct stVarNode ndVar;
typedef struct stAttributionNode ndAttribution;
typedef struct stNewNode ndNew;
typedef struct stExpListNode ndExpList;
typedef struct stFunctionCallNode ndFunctionCall;
typedef struct stIfElseNode ndIfElse;
typedef struct stWhileNode ndWhile;

typedef struct stDeclaration {
	enum enDeclaration decTag;
	void *pNode;
} ndDeclaration;

typedef struct stDeclarations {
	tpList *pList;
} ndDeclarations;

typedef struct stType {
	int token;
	int depth;
} tpType;

typedef struct stBlockNode {
  ndVarDeclarations *pVarDecs;
  ndStatements *pStats;
} ndBlock;

typedef struct stFunctionNode {
	char *name;
	ndBlock *pBlock;
	ndParameters *pParameters;
	tpType *pReturnType;
	int varsStackSize;
} ndFunction;

typedef struct stParametersNode {
	tpList *pList;
} ndParameters;

typedef struct stVariablesNode {
	tpList *pList;
} ndVariables;

typedef struct stVariableNode {
	tpType *pType;
	char *name;
  int stackPadding;
	int isGlobal;
} ndVariable;

typedef struct stVarDeclarationsNode {
	tpList *pList;
} ndVarDeclarations;

typedef struct stStatementsNode {
	tpList *pList;
} ndStatements;

typedef struct stStatementNode {
	void *pNode;
  enum enStatTag statTag;
} ndStatement;

typedef struct stReturnNode {
  ndExpression *pExp;
} ndReturn;

typedef struct stVarBackDeclaration {
	ndVariable *pVarDec;
	int usedDepth;
} tpVarBackDeclaration;

typedef struct stExpressionNode {
  enum enExpTag expTag;
	tpType *pType;
  union {
    long long int ival;
    char *text;
    double fval;
    void *pNode;
    struct {
      ndExpression *pExp1;
      ndExpression *pExp2;
      enum enExpBinTag expTag;
    } bin;
  } value;
} ndExpression;

typedef struct stVarNode {
  enum enVarTag varTag;
	tpVarBackDeclaration *pBackDeclaration;
	struct stVarNode *pBase;
  union {
    char *name;
    struct {
      ndExpression *pPointerExp;
      ndExpression *pInxExp;
    } address;
  } value;
} ndVar;

typedef struct stAttributionNode {
  ndVar *pVar;
  ndExpression *pExp;
} ndAttribution;

typedef struct stNewNode {
  tpType *pType;
  ndExpression *pExp;
} ndNew;

typedef struct stFunctionCallNode {
  char *functionName;
  ndExpList *pExpList;
	ndFunction *pDeclaration;
} ndFunctionCall;

typedef struct stExpListNode {
  tpList *pList;
} ndExpList;

typedef struct stIfElseNode {
  ndExpression *nExpIf;
  ndStatement *nStatementIf;
  ndStatement *nStatementElse;
} ndIfElse;

typedef struct stWhileNode {
  ndExpression *pExp;
  ndStatement *pStat;
} ndWhile;

ndDeclaration *finishDeclaration(tpType *pType, ndDeclaration *pDec);
ndDeclarations *createFullDeclarationsNode(ndDeclaration *pDec);
ndDeclarations *addFullDeclaration(ndDeclarations *pDecs, ndDeclaration *pDec);
ndFunction *createFunctionNode(char *name, ndParameters *pParams, ndBlock *pBlock);
ndBlock *createBlockNode(ndVarDeclarations *pVarDecs, ndStatements *pStats);
tpType *newType(int token, int depth);
ndDeclaration *createDeclarationNode(void *node, enum enDeclaration decTag);
ndParameters* createParametersNode(ndVariable* pParam);
ndVariable* createParameterNode(tpType *pType, char *name);
void addParam(ndParameters *pParams, ndVariable *pParam);
ndExpList* createExpListNode(ndExpression *pExp);
void addExpListNode(ndExpList *pExpList, ndExpression *pExp);
void addParam(ndParameters *pParams, ndVariable *pParam);

ndVariables *createVariablesNode(char *name);
void addVariable(ndVariables *pVars, char *name);

ndVarDeclarations *createVarDeclarations(tpType *pType, ndVariables *pVars);
void addVarDeclaration(ndVarDeclarations *pVarDecs, tpType *pType, ndVariables *pVars);
ndStatements *createStatements(ndStatement *pStat);
void addStatement(ndStatements *pStats, ndStatement *pStat);
ndStatement *createStatement(void *pNode, enum enStatTag);

ndReturn *createReturnNode(ndExpression *pExp);
ndAttribution *createAttributionNode(ndVar *pVar, ndExpression *pExp);
ndVar *createVarNode(char *name);
ndVar *createArrayVarNode(ndExpression *ndExp, ndExpression *ndInxExp);
ndExpression *createExpressionIntegerNode(int val, enum enExpTag expTag);
ndExpression *createExpressionFloatNode(double val);
ndExpression *createExpressionTextNode(char *val);
ndExpression *createExpressionGenericNode(void *pNode, enum enExpTag expTag);
ndExpression *createExpressionBinaryNode(ndExpression *pExp1, ndExpression *pExp2, enum enExpBinTag expTag);
ndNew *createNewNode(tpType *pType, ndExpression *pExp);
ndFunctionCall *createFunctionCall(char *functionName, ndExpList *pExpList);
ndIfElse *createIfElseNode(ndExpression *nExpIf, ndStatement *nStatementIf);
ndWhile *createWhileNode(ndExpression *pExp, ndStatement *pStat);
ndIfElse *addElseStatement(ndIfElse *pIfElse, ndStatement *nStatementElse);

void incrDepth(tpType *pType);


void printDeclarations();
#endif
