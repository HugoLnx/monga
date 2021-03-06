#include "ast_pre_traversing.h"
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
	char *ifEndLabel;
	char *elseLabel;
	char *jmpLoopWhileLabel;
	char *jmpEndWhileLabel;
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
void codeForWhile(ndWhile *pWhile, void *pShared);
void codeForIf(ndIfElse *pNode, void *pShared);
void codeForElse(ndIfElse *pNode, void *pShared);
void generateNumbersExpBin(char *command, ndExpression *pExp, void *pShared);
void afterEvent(char *evtName, void *pShared);
int isCharType(tpType *pType);

void COD_codeForTree(ndDeclarations *pDeclarations) {
	PRETRA_tpEvents *pEvents = NEW(PRETRA_tpEvents);

	pEvents->onFunction = codeForFunction;
	pEvents->onVariable = codeForVarDeclaration;
	pEvents->onAttribution = codeForAttribution;
	pEvents->onParameters = codeForParameters;
	pEvents->onReturn = codeForReturn;
	pEvents->onStatement = codeForStatement;
	pEvents->onIf = codeForIf;
	pEvents->onElse = codeForElse;
	pEvents->onWhile = codeForWhile;

	pEvents->onBackLevel = afterEvent;

	tpState *pState = NEW(tpState);

	ASY_raw(".data\n");
	PRETRA_execute(pDeclarations, pEvents, (void*)pState);
	ASY_raw(".end\n\n");
}

/*
 * EXPRESSIONS
 */
void codeForExp(ndExpression *pExp, void *pShared) {
	char *label, *label2;
	ndNew *pNew;
	switch(pExp->expTag) {
		case(EXPND_VAR):
			codeForVar(pExp->value.pNode, pShared);
			if (isCharType(pExp->pType)) {
				ASY_raw("movl (%%eax), %%ecx\n");
				ASY_raw("movb %%cl, %%al\n");
			} else {
				ASY_raw("movl (%%eax), %%eax\n");
			}
			break;
		case(EXPND_NEW): 
			pNew = (ndNew*) pExp->value.pNode;
			codeForExp(pNew->pExp, pShared);
			if(!isCharType(pNew->pType)) {
			  ASY_raw("imull $4,%%eax\n");
			}
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
			printf("TODO: Float isn't supported");
			break;
		case(EXP_HEXADECIMAL):
		case(EXP_NUMBER):
			ASY_raw("movl $%lld, %%eax\n", pExp->value.ival);
			break;
		case(EXP_CHAR):
			ASY_raw("movb $%d, %%al\n", pExp->value.ival);
			break;
		case(EXPND_MINUS):
			codeForExp((ndExpression*) pExp->value.pNode, pShared);
			ASY_raw("imull $-1, %%eax\n");
			break;
		case(EXPND_EXCLAMATION): 
			label = LBL_generate(LBL_next());
			label2 = LBL_generate(LBL_next());
			codeForExp((ndExpression*) pExp->value.pNode, pShared);
			ASY_raw("cmpl $0, %%eax\n");
			ASY_raw("je %s\n", label);
			ASY_raw("movl $0, %%eax\n");
			ASY_raw("jmp %s\n", label2);
			ASY_label(label);
			ASY_raw("movl $1, %%eax\n", label);
			ASY_label(label2);
			break;
		case(EXPND_BIN): 
			codeForExpBin(pExp, pShared);
			break;
		case(EXPND_CALL): 
			codeForFunctionCall((ndFunctionCall*) (pExp->value.pNode), pShared);
			break;
	}
}

void codeForExpBin(ndExpression *pExp, void *pShared) {
	char *labelTrue, *labelMiddle, *labelFalse, *labelEnd;
	codeForExp(pExp->value.bin.pExp1, pShared);
	ASY_raw("pushl %%eax\n");
	codeForExp(pExp->value.bin.pExp2, pShared);
	ASY_raw("popl %%ecx\n");
	if (isCharType(pExp->value.bin.pExp1->pType)) {
		ASY_raw("movsbl %%cl, %%ecx\n");
	}
	if (isCharType(pExp->value.bin.pExp2->pType)) {
		ASY_raw("movsbl %%al, %%eax\n");
	}
	switch(pExp->value.bin.expTag) {
		case(EXPBIN_PLUS):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				printf("TODO: Float isn't supported");
			}
			else {
				ASY_raw("addl %%ecx, %%eax\n");
			}
			break;
		case(EXPBIN_SLASH):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				printf("TODO: Float isn't supported");
			}
			else {
				ASY_raw("movl %%eax, %%edx\n");
				ASY_raw("movl %%ecx, %%eax\n");
				ASY_raw("movl %%edx, %%ecx\n");

				ASY_raw("movl $0,%%edx\n");
				ASY_raw("divl %%ecx\n");
			}
			break;
		case(EXPBIN_MINUS):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				printf("TODO: Float isn't supported");
			}
			else {
				ASY_raw("subl %%eax, %%ecx\n");
				ASY_raw("movl %%ecx, %%eax\n");
			}
			break;
		case(EXPBIN_ASTERISK):
			if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
				printf("TODO: Float isn't supported");
			}
			else {
				ASY_raw("imull %%ecx, %%eax\n");
			}
			break;
		case(EXPBIN_AND):
			labelFalse = LBL_generate(LBL_next());
			labelEnd = LBL_generate(LBL_next());
			ASY_raw("cmp $0, %%ecx\n");
			ASY_raw("je %s\n", labelFalse);
			ASY_raw("cmp $0, %%eax\n");
			ASY_raw("je %s\n", labelFalse);
			ASY_raw("movl $1, %%eax\n");
			ASY_raw("jmp %s\n", labelEnd);
			ASY_label(labelFalse);
			ASY_raw("movl $0, %%eax\n");
			ASY_label(labelEnd);
			break;
		case(EXPBIN_OR):
			labelTrue = LBL_generate(LBL_next());
			labelEnd = LBL_generate(LBL_next());
			ASY_raw("cmp $0, %%ecx\n");
			ASY_raw("jne %s\n", labelTrue);
			ASY_raw("cmp $0, %%eax\n");
			ASY_raw("jne %s\n", labelTrue);
			ASY_raw("movl $0, %%eax\n");
			ASY_raw("jmp %s\n", labelEnd);
			ASY_label(labelTrue);
			ASY_raw("movl $1, %%eax\n");
			ASY_label(labelEnd);
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


void generateNumbersExpBin(char *command, ndExpression *pExp, void *pShared) {
	char *labelEnd = LBL_generate(LBL_next());
	char *labelTrue = LBL_generate(LBL_next());
	if (isFloatOperation(pExp->value.bin.pExp1, pExp->value.bin.pExp2)) {
		printf("TODO: Float isn't supported");
	}
	else {
		ASY_raw("cmp %%eax, %%ecx\n");
		ASY_raw("%s %s\n", command, labelTrue);
		ASY_raw("movl $0, %%eax\n");
		ASY_raw("jmp %s\n", labelEnd);
		ASY_label(labelTrue);
		ASY_raw("movl $1, %%eax\n");
	}
	ASY_label(labelEnd);
}
/* END EXPRESSIONS */



/*
 * DECLARATIONS
 */
void codeForFunction(ndFunction *pFunc, void *pShared) {
	int totalSpace = pFunc->varsStackSize;
	STATE(pShared)->lastVarPadding = -totalSpace;
	ASY_raw(".text\n");
	ASY_function(pFunc->name);
	ASY_functionBeginning();
	ASY_raw("subl $%d, %%esp\n", totalSpace);
}

void codeForParameters(ndParameters *pParams, void *pShared) {
	LIS_tpList *pList;
	int paramPadding = 8;
	if (pParams == NULL) return;

  pList = pParams->pList;
  LIS_reset(pList);
  while(LIS_goPrevious(pList)) {
    ndVariable *pVar = (ndVariable*) LIS_getCurrentValue(pList);
		pVar->stackPadding = paramPadding;
		paramPadding += 4;
  }
}

void codeForVarDeclaration(ndVariable *pVar, void *pShared) {
	if(pVar->isGlobal) {
		if (isCharType(pVar->pType)) {
			ASY_globalVar(pVar->name, "byte", "0");
		}
		else {
			ASY_globalVar(pVar->name, "int", "0");
		}
	} else {
		pVar->stackPadding = STATE(pShared)->lastVarPadding;
		if (isCharType(pVar->pType)) {
			STATE(pShared)->lastVarPadding += 1;
		}
		else {
			STATE(pShared)->lastVarPadding += 4;
		}
	}
}
/* END DECLARATIONS */


/*
 * STATEMENTS
 */
void codeForStatement(ndStatement *pStat, void *pShared) {
	if(pStat->statTag == STAT_FUNCTION_CALL) {
		codeForFunctionCall((ndFunctionCall*) pStat->pNode, pShared);
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
	if(pVar->varTag == IDS_ID) {
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
		if (!isCharType(pVar->pBackDeclaration->pVarDec->pType)) {
			ASY_raw("imull $4,%%eax\n");
	  }
		ASY_raw("movl %%eax,%%ecx\n");
		ASY_raw("popl %%eax\n");
		ASY_raw("addl %%ecx,%%eax\n");
	}
}

void codeForFunctionCall(ndFunctionCall *pCall, void *pShared) {
	int paramsStackSize = 0;
	LIS_tpList *pList;

	if(pCall->pExpList != NULL) {
    pList = pCall->pExpList->pList;
		LIS_reset(pList);
		while(LIS_goNext(pList)) {
			ndExpression *pStat = (ndExpression*) LIS_getCurrentValue(pList);
			codeForExp(pStat, pShared);
			if (isCharType(pStat->pType)) {
				ASY_raw("movzbl %%al, %%eax\n");
				ASY_raw("pushl %%eax\n");
			}
			else {
				ASY_raw("pushl %%eax\n");
			}
			paramsStackSize += 4;
		}
	}

	ASY_functionCall(pCall->functionName, paramsStackSize);
}

void codeForWhile(ndWhile *pWhile, void *pShared) {
	char *endLabel = LBL_generate(LBL_next());
	char *loopLabel = LBL_generate(LBL_next());
	ASY_label(loopLabel);
	codeForExp(pWhile->pExp, pShared);
	ASY_raw("cmp $0, %%eax\n");
	ASY_raw("je %s\n", endLabel);

	STATE(pShared)->jmpLoopWhileLabel = loopLabel;
	STATE(pShared)->jmpEndWhileLabel = endLabel;
}

void codeForIf(ndIfElse *pNode, void *pShared) {
	char *labelElse = LBL_generate(LBL_next());
	char *labelEnd = LBL_generate(LBL_next());
	codeForExp(pNode->nExpIf, pShared);
	ASY_raw("cmp $0, %%eax\n");
	
	if (pNode->nStatementElse == NULL) {
		ASY_raw("je %s\n", labelEnd);
	}
	else {
		ASY_raw("je %s\n", labelElse);
	}

	STATE(pShared)->ifEndLabel = labelEnd;
	STATE(pShared)->elseLabel = labelElse;
}

void codeForElse(ndIfElse *pNode, void *pShared) {
	ASY_raw("jmp %s\n", STATE(pShared)->ifEndLabel);
	ASY_label(STATE(pShared)->elseLabel);
}

void codeForAttribution(ndAttribution *pAttr, void *pShared) {
	codeForVar(pAttr->pVar, pShared);
	ASY_raw("pushl %%eax\n");
	codeForExp(pAttr->pExp, pShared);
	ASY_raw("popl %%ecx\n");	
	if (isCharType(pAttr->pVar->pBackDeclaration->pVarDec->pType)) {
		ASY_raw("movb %%al, (%%ecx)\n");
	}
	else {
		ASY_raw("movl %%eax, (%%ecx)\n");
	}
}
/* END STATEMENTS */



/*
 * AFTER EVENT
 */
void afterEvent(char *evtName, void *pShared) {
	if (strcmp(evtName, "onFunction") == 0) {
		ASY_functionEnding();
	}
	else if (strcmp(evtName, "onIf") == 0) {
		ASY_label(STATE(pShared)->ifEndLabel);
	}
	else if (strcmp(evtName, "onWhile") == 0) {
		ASY_raw("jmp %s\n", STATE(pShared)->jmpLoopWhileLabel);
		ASY_raw("%s:\n", STATE(pShared)->jmpEndWhileLabel);
	}

}
/* END AFTER EVENT */



/*
 * TYPE CHECKING
 */

int isFloatExpression(ndExpression *pExp){
	return pExp->pType != NULL
		&& pExp->pType->token == TK_FLOAT
		&& pExp->pType->depth == 0;
}

int isFloatOperation(ndExpression *pExp1, ndExpression *pExp2){
	return isFloatExpression(pExp1) || isFloatExpression(pExp2);
}

int isCharType(tpType *pType) {
	return pType->token == TK_CHAR && pType->depth == 0;
}
/* END TYPE CHECKING */
