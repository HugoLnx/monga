#include "ast_traversing.h"
#include "ast_code.h"
#include "assembly_writer.h"
#include "utils.h"
#include "label_generator.h"
#include "list.h"
#include "y.tab.h"
#include <string.h>

#define STATE(varName) ((tpState*) varName)

typedef struct stState {
	int lastVarPadding;
	int endIfLabel;
	int jmpLoopWhileLabel;
	int jmpEndWhileLabel;
} tpState;

void codeForReturn(ndReturn *pReturn, void *pShared);
void codeForVarDeclaration(ndVariable *pVar, void *pShared);
void codeForParameters(ndParameters *pParams, void *pShared);
void codeForFunction(ndFunction *pFunc, void *pShared);
void codeForVar(ndVar *pVar, void *pShared);
void codeForExpBin(ndExpression *pExp, void *pShared);
void codeForFunctionCall(ndFunctionCall *pCall, void *pShared);
void codeForStatement(ndStatement *pStat, void *pShared);
void codeForAttribution(ndAttribution *pAttr, void *pShared);
void codeForExp(ndExpression *pExp, void *pShared);
void codeForIfElse(ndIfElse *pNode, void *pShared);
void generateNumbersExpBin(char *command, ndExpression *pExp, void *pShared);
void afterEvent(char *evtName, void *pShared);


void afterEvent(char *evtName, void *pShared) {
	if (strcmp(evtName, "onFunction") == 0) {
		ASY_functionEnding();
	}
	else if (strcmp(evtName, "onIf") == 0) {
		ASY_raw("%s: \n", LBL_generate(((tpState*) pShared)->endIfLabel));
	}
	else if (strcmp(evtName, "onWhile") == 0) {
		ASY_raw("jmp %s\n", LBL_generate(((tpState*) pShared)->jmpLoopWhileLabel));
		ASY_raw("%s:\n", LBL_generate(((tpState*) pShared)->jmpEndWhileLabel));	
	}

}

int isIntExpression(ndExpression *pExp){
	ndVar *ndVariable;
	if (pExp->expType == EXPND_MINUS){
		return isIntExpression((ndExpression*) pExp->value.pNode);
	}
	if (pExp->expType == EXPND_VAR){
		ndVariable = (ndVar *) pExp->value.pNode;
		return (ndVariable->varType == EXP_NUMBER) || (ndVariable->varType == EXP_HEXADECIMAL) || (ndVariable->varType == EXP_CHAR);
	}
	return (pExp->expType == EXP_NUMBER) || (pExp->expType == EXP_HEXADECIMAL) || (pExp->expType == EXP_CHAR);
}

void codeForExp(ndExpression *pExp, void *pShared) {
	char *label;
  ndNew *pNew;
	switch(pExp->expType) {
		case(EXPND_VAR):
			codeForVar(pExp->value.pNode, pShared);
			ASY_raw("movl (%%eax), %%eax\n");
			break;
		case(EXPND_NEW): 
			pNew = (ndNew*) pExp->value.pNode;
			codeForExp(pNew->pExp, pShared);
			ASY_raw("imull $4,%%eax\n");
			ASY_malloc("%eax");
			break;
		case(EXP_TEXT):
			label = LBL_generate(LBL_next());
			ASY_raw(".data\n");
			ASY_raw("%s: .string \"%s\"\n", label, pExp->value.text);
			ASY_raw(".text\n");
			ASY_raw("movl $%s, %%eax\n", label);
			break;
		case(EXP_FLOAT):
			// TODO
			label = LBL_generate(LBL_next());
			ASY_raw(".data\n");
			ASY_raw("%s: .float %f\n", label, pExp->value.fval);
			ASY_raw(".text\n");
			ASY_raw("flds %s\n", label);
			ASY_raw("fstps (%%ecx)\n");
			ASY_raw("movl %%ecx, %%eax\n", label);
			break;
		case(EXP_HEXADECIMAL):
		case(EXP_NUMBER):
		case(EXP_CHAR):
			ASY_raw("movl $%lld, %%eax\n", pExp->value.ival);
			break;
		case(EXPND_MINUS):
			codeForExp((ndExpression*) pExp->value.pNode, pShared);
			ASY_raw("imull $-1, %%eax\n");
			// TODO IF FLOAT
			break;
		case(EXPND_EXCLAMATION): 
			// TODO
			break;
		case(EXPND_BIN): 
			codeForExpBin(pExp, pShared);
			break;
		case(EXPND_CALL): 
			codeForFunctionCall((ndFunctionCall*) (pExp->value.pNode), pShared);
			break;
	}
}

void codeForFunction(ndFunction *pFunc, void *pShared) {
	STATE(pShared)->lastVarPadding = -4;
	ASY_raw(".text\n");
	ASY_function(pFunc->name);
	ASY_functionBeginning();
	ASY_raw("subl $%d, %%esp\n", pFunc->varsStackSize+4);
}

void codeForVarDeclaration(ndVariable *pVar, void *pShared) {
	if(pVar->isGlobal) {
		ASY_globalVar(pVar->name, "int", "0");
	} else {
		pVar->stackPadding = STATE(pShared)->lastVarPadding;
		STATE(pShared)->lastVarPadding -= 4;
	}
}

void codeForParameters(ndParameters *pParams, void *pShared) {
	tpList *pList;
	int paramPadding = 8;
	if (pParams == NULL) return;

  pList = pParams->pList;
  resetList(pList);
  while(goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) getCurrentValue(pList);
		pVar->stackPadding = paramPadding;
		paramPadding += 4;
  }
}

void codeForReturn(ndReturn *pReturn, void *pShared) {
	if(pReturn->pExp == NULL) {
		ASY_raw("movl $0, %%eax\n");
	} else {
		codeForExp(pReturn->pExp, pShared);
	}
}

void codeForVar(ndVar *pVar, void *pShared) {
	ndVariable *pDec = pVar->pBackDeclaration->pVarDec;
	if(pVar->varType == VAR_ID) {
		if(pDec->isGlobal) {
			ASY_raw("movl $%s, %%eax\n", pDec->name);
		} else {
			int padding = pDec->stackPadding;
			ASY_raw("movl %%ebp, %%eax\n");
			ASY_raw("addl $%d, %%eax\n", padding);
		}
	} else {
		codeForExp(pVar->value.address.pPointerExp, pShared);
		ASY_raw("pushl %%eax\n");
		codeForExp(pVar->value.address.pInxExp, pShared);
		ASY_raw("imull $4,%%eax\n");
		ASY_raw("movl %%eax,%%ecx\n");
		ASY_raw("popl %%eax\n");
		ASY_raw("addl %%ecx,%%eax\n");
	}
}

void generateNumbersExpBin(char *command, ndExpression *pExp, void *pShared) {
	char *labelEnd = LBL_generate(LBL_next());
	char *labelTrue = LBL_generate(LBL_next());
	char *labelFalse = LBL_generate(LBL_next());
	if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
		// TODO: float treat
	}
	else {
		codeForExp(pExp->value.bin.pExp1, pShared);
		ASY_raw("pushl %%eax\n");
		codeForExp(pExp->value.bin.pExp2, pShared);
		ASY_raw("popl %%ecx\n");
		ASY_raw("cmp %%eax, %%ecx\n");
		ASY_raw("%s %s\n", command, labelTrue);
		ASY_raw("movl $0, %%eax\n");
		ASY_raw("jmp %s\n", labelEnd);
		ASY_raw("%s: movl $1, %%eax\n", labelTrue);
		ASY_raw("jmp %s\n", labelEnd);
	}
	ASY_raw("%s:\n", labelEnd);
}

void codeForExpBin(ndExpression *pExp, void *pShared) {
	char *labelTrue, *labelMiddle, *labelFalse, *labelEnd;
	switch(pExp->value.bin.expType) {
		case(EXPBIN_PLUS):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				// TODO: float treat
			}
			else {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("popl %%ecx\n");
				ASY_raw("addl %%ecx, %%eax\n");
			}
			break;
		case(EXPBIN_SLASH):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				// TODO: float treat
			}
			else {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("movl %%eax, %%ecx\n");
				ASY_raw("popl %%eax\n");
				ASY_raw("movl $0,%%edx\n");
				ASY_raw("divl %%ecx\n");
			}
			break;
		case(EXPBIN_MINUS):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				// TODO: float treat
			}
			else {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("movl %%eax, %%ecx\n");
				ASY_raw("popl %%eax\n");
				ASY_raw("subl %%ecx, %%eax\n");
			}
			break;
		case(EXPBIN_ASTERISK):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				// TODO: float treat
			}
			else {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("popl %%ecx\n");
				ASY_raw("imull %%ecx, %%eax\n");
			}
			break;
		case(EXPBIN_AND):
			labelFalse = LBL_generate(LBL_next());
			labelEnd = LBL_generate(LBL_next());
			codeForExp(pExp->value.bin.pExp1, pShared);
			ASY_raw("pushl %%eax\n");
			codeForExp(pExp->value.bin.pExp2, pShared);
			ASY_raw("popl %%ecx\n");
			ASY_raw("cmp $0, %%ecx\n");
			ASY_raw("je %s\n", labelFalse);
			ASY_raw("cmp $0, %%eax\n");
			ASY_raw("je %s\n", labelFalse);
			ASY_raw("movl $1, %%eax\n");
			ASY_raw("jmp %s\n", labelEnd);
			ASY_raw("%s: movl $0, %%eax\n", labelFalse);
			ASY_raw("jmp %s\n", labelEnd);
			ASY_raw("%s:\n", labelEnd);
			break;
		case(EXPBIN_OR):
			labelTrue = LBL_generate(LBL_next());
			labelEnd = LBL_generate(LBL_next());
			codeForExp(pExp->value.bin.pExp1, pShared);
			ASY_raw("pushl %%eax\n");
			codeForExp(pExp->value.bin.pExp2, pShared);
			ASY_raw("popl %%ecx\n");
			ASY_raw("cmp $0, %%ecx\n");
			ASY_raw("jne %s\n", labelTrue);
			ASY_raw("cmp $0, %%eax\n");
			ASY_raw("jne %s\n", labelTrue);
			ASY_raw("movl $0, %%eax\n");
			ASY_raw("jmp %s\n", labelEnd);
			ASY_raw("%s: movl $1, %%eax\n", labelTrue);
			ASY_raw("jmp %s\n", labelEnd);
			ASY_raw("%s:\n", labelEnd);
			break;
		case(EXPBIN_DOUBLE_EQUAL):
			generateNumbersExpBin("je", pExp, pShared);
			break;
		case(EXPBIN_BANG_EQUAL):
			generateNumbersExpBin("jne", pExp, pShared);
			break;
		case(EXPBIN_LESS_EQUAL):
			generateNumbersExpBin("jle", pExp, pShared);
			break;
		case(EXPBIN_GREATER_EQUAL):
			generateNumbersExpBin("jge", pExp, pShared);
			break;
		case(EXPBIN_LESS):
			generateNumbersExpBin("jl", pExp, pShared);
			break;
		case(EXPBIN_GREATER):
			generateNumbersExpBin("jg", pExp, pShared);
			break;
	}
}

void codeForWhile(ndWhile *pWhile, void *pShared) {
	int endInt = LBL_next();
	int loopInt = LBL_next();
	char *endLabel = LBL_generate(endInt);
	char *loopLabel = LBL_generate(loopInt);
	ASY_raw("%s:\n", loopLabel);
	codeForExp(pWhile->pExp, pShared);
	ASY_raw("cmp $0, %%eax\n");
	ASY_raw("je %s\n", endLabel);
	codeForStatement(pWhile->pStat, pShared);

	((tpState*) pShared)->jmpLoopWhileLabel = loopInt;
	((tpState*) pShared)->jmpEndWhileLabel = endInt;

}

void codeForFunctionCall(ndFunctionCall *pCall, void *pShared) {
	int qntParams = 0;
	tpList *pList;
	ASY_functionCallHeader();

	if(pCall->pExpList != NULL) {
    pList = pCall->pExpList->pList;
		resetList(pList);
		while(goNext(pList)) {
			ndExpression *pStat = (ndExpression*) getCurrentValue(pList);
			codeForExp(pStat, pShared);
			ASY_raw("pushl %%eax\n");
			qntParams++;
		}
	}

	ASY_functionCall(pCall->functionName, qntParams);
}

void codeForStatement(ndStatement *pStat, void *pShared) {
	if(pStat->statType == STAT_FUNCTION_CALL) {
		codeForFunctionCall((ndFunctionCall*) pStat->pNode, pShared);
	}
}

void codeForIfElse(ndIfElse *pNode, void *pShared) {
	char *labelElse = LBL_generate(LBL_next());
	int numLabelEnd = LBL_next();
	char *labelEnd = LBL_generate(numLabelEnd);
	codeForExp(pNode->nExpIf, pShared);
	ASY_raw("cmp $0, %%eax\n");
	
	if (pNode->nStatementElse == NULL){
		ASY_raw("je %s\n", labelEnd);
		codeForStatement(pNode->nStatementIf, pShared);
	}
	else{
		ASY_raw("je %s\n", labelElse);
		codeForStatement(pNode->nStatementIf, pShared);
		ASY_raw("jmp %s\n", labelEnd);
		ASY_raw("%s: \n", labelElse);
		codeForStatement(pNode->nStatementElse, pShared);
	}

	((tpState*) pShared)->endIfLabel = numLabelEnd;
}

void initVarDeclarationsState(ndVarDeclarations *pVars, void *pShared) {
	//STATE(pShared)->lastVarPadding = 0;
}

void codeForAttribution(ndAttribution *pAttr, void *pShared) {
	codeForVar(pAttr->pVar, pShared);
	ASY_raw("pushl %%eax\n");
	codeForExp(pAttr->pExp, pShared);
	ASY_raw("popl %%ecx\n");
	ASY_raw("movl %%eax, (%%ecx)\n");
}

void COD_codeForTree(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);

	pEvents->onFunction = codeForFunction;
	pEvents->onVariable = codeForVarDeclaration;
	pEvents->onVarDeclarations = initVarDeclarationsState;
	pEvents->onAttribution = codeForAttribution;
	pEvents->onParameters = codeForParameters;
	pEvents->onReturn = codeForReturn;
	pEvents->onStatement = codeForStatement;
	pEvents->onIf = codeForIfElse;
	pEvents->onWhile = codeForWhile;

	pEvents->onBackLevel = afterEvent;

	tpState *pState = NEW(tpState);

	ASY_raw(".data\n");
	TRA_execute(pDeclarations, pEvents, (void*)pState);
	ASY_raw(".end\n\n");
}

int isFloatExpression(ndExpression *pExp){
	return pExp->pType != NULL
		&& pExp->pType->token == TK_FLOAT
		&& pExp->pType->depth == 0;
}

int isFloatOperation(ndExpression *pExp1, ndExpression *pExp2){
	return isFloatExpression(pExp1) || isFloatExpression(pExp2);
}