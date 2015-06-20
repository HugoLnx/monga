#include "assembly_writer.h"
#include <string.h>
#include <stdarg.h>

void ASY_raw(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void ASY_globalVar(char *varName, char *varType, char *varValue) {
	ASY_raw("%s: .%s \"%s\"\n", varName, varType, varValue);
}

void ASY_function(char *funcName) {
	ASY_raw(".globl %s\n", funcName);
	ASY_label(funcName);
}

void ASY_label(char *labelName) {
	ASY_raw("%s:\n", labelName);
}

void ASY_functionBeginning() {
	ASY_raw("pushl %%ebp\n");
	ASY_raw("movl %%esp, %%ebp\n");
	ASY_raw("pushl %%ebx\n");
}

void ASY_functionCall(char *funcName, int qntParams, ...) {
	va_list params;
	int i;
	ASY_raw("pushl %%ecx\n");
	ASY_raw("pushl %%edx\n");
	ASY_raw("pushl %%eax\n");
	va_start(params, qntParams);
	for(i = 0; i < qntParams; i++) {
		char *arg = va_arg(params, char*);
		ASY_raw("pushl %s\n", arg);
	}
	va_end(params);
	ASY_raw("call %s\n", funcName);
	ASY_raw("addl $%d, %%esp\n", qntParams*4+4);
	ASY_raw("popl %%edx\n");
	ASY_raw("popl %%ecx\n");
}

void ASY_functionEnding() {
	ASY_raw("popl %%ebx\n");
	ASY_raw("movl %%ebp, %%esp\n");
	ASY_raw("popl %%ebp\n");
}