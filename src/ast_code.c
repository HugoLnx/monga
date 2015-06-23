#include "ast_traversing.h"
#include "ast_code.h"
#include "assembly_writer.h"
#include "utils.h"
#include "label_generator.h"
#include "list.h"
#include <string.h>

#define STATE(varName) ((tpState*) varName)

typedef struct stState {
	int lastVarPadding;
} tpState;

void codeForExp(ndExpression *pExp, void *pShared);

void beforeEvent(char *evtName, void *pShared) {
}

void afterEvent(char *evtName, void *pShared) {
	if (strcmp(evtName, "onFunction") == 0) {
		ASY_functionEnding();
	}
}

int isIntExpression(ndExpression *pExp){
	ndVar *ndVariable;
	if (pExp->expType == EXPND_VAR){
		ndVariable = (ndVar *) pExp->value.pNode;
		return (ndVariable->varType == EXP_NUMBER) || (ndVariable->varType == EXP_HEXADECIMAL) || (ndVariable->varType == EXP_CHAR);
	}
	return (pExp->expType == EXP_NUMBER) || (pExp->expType == EXP_HEXADECIMAL) || (pExp->expType == EXP_CHAR);
}

int isIntOperation(ndExpression *pExp1, ndExpression *pExp2){
	return isIntExpression(pExp1) && isIntExpression(pExp2);
}

void codeForHeader(ndDeclarations *pDeclarations, void *pShared) {
}

void codeForGlobalVariable(ndVariable *pVar, void *pShared) {
}

void codeForReturn(ndReturn *pReturn, void *pShared) {
	if(pReturn->pExp == NULL) {
		ASY_raw("movl $0, %%eax\n");
	} else {
		codeForExp(pReturn->pExp, pShared);
	}
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

void codeForFunction(ndFunction *pFunc, void *pShared) {
	STATE(pShared)->lastVarPadding = -4;
	ASY_raw(".text\n");
	ASY_function(pFunc->name);
	ASY_functionBeginning();
	ASY_raw("subl $%d, %%esp\n", pFunc->varsStackSize+4);
}

void codeForVar(ndVar *pVar, void *pShared) {
	ndVariable *pDec = pVar->pBackDeclaration->pVarDec;
	if(pDec->isGlobal) {
		ASY_raw("movl $%s, %%eax\n", pDec->name);
	} else {
		int padding = pDec->stackPadding;
		ASY_raw("movl %%ebp, %%eax\n");
		ASY_raw("addl $%d, %%eax\n", padding);
	}
}

void generateNumbersExpBin(char *command, ndExpression *pExp, void *pShared) {
	char *labelEnd = LBL_generate(LBL_next());
	char *labelTrue = LBL_generate(LBL_next());
	char *labelFalse = LBL_generate(LBL_next());
	if (isIntOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
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
	else {
		// TODO floats
	}
	ASY_raw("%s:\n", labelEnd);
}

void codeForExpBin(ndExpression *pExp, void *pShared) {
	char *labelTrue, *labelMiddle, *labelFalse, *labelEnd;
	switch(pExp->value.bin.expType) {
		case(EXPBIN_PLUS):
			if (isIntOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("popl %%ecx\n");
				ASY_raw("addl %%ecx, %%eax\n");
			}
			else {

			}
			break;
		case(EXPBIN_SLASH):
			if (isIntOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("movl %%eax, %%ecx\n");
				ASY_raw("popl %%eax\n");
				ASY_raw("movl $0,%%edx\n");
				ASY_raw("divl %%ecx\n");
			}
			else {

			}
			break;
		case(EXPBIN_MINUS):
			if (isIntOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("movl %%eax, %%ecx\n");
				ASY_raw("popl %%eax\n");
				ASY_raw("subl %%ecx, %%eax\n");
			}
			else {

			}
			break;
		case(EXPBIN_ASTERISK):
			if (isIntOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				codeForExp(pExp->value.bin.pExp1, pShared);
				ASY_raw("pushl %%eax\n");
				codeForExp(pExp->value.bin.pExp2, pShared);
				ASY_raw("popl %%ecx\n");
				ASY_raw("imull %%ecx, %%eax\n");
			}
			else {

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

void codeForExp(ndExpression *pExp, void *pShared) {
	char *label;
	switch(pExp->expType) {
		case(EXPND_VAR):
			codeForVar(pExp->value.pNode, pShared);
			ASY_raw("movl (%%eax), %%eax\n");
			break;
		case(EXPND_NEW): 
			// TODO
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

void COD_codeForTree(ndDeclarations *pDeclarations) {
	TRA_tpEvents *pEvents = NEW(TRA_tpEvents);

  pEvents->onDeclarations = codeForHeader;
  pEvents->onVariable = codeForGlobalVariable;
  pEvents->onFunction = codeForFunction;
  pEvents->onVariable = codeForVarDeclaration;
	pEvents->onVarDeclarations = initVarDeclarationsState;
	pEvents->onAttribution = codeForAttribution;
	pEvents->onParameters = codeForParameters;
	pEvents->onReturn = codeForReturn;
	pEvents->onStatement = codeForStatement;

  pEvents->onNewLevel = beforeEvent; 
  pEvents->onBackLevel = afterEvent; 

	tpState *pState = NEW(tpState);

	ASY_raw(".data\n");
  TRA_execute(pDeclarations, pEvents, (void*)pState);
	ASY_raw(".end\n\n");
}
