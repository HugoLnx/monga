#include "assembly_writer.h"
#include <string.h>
#include <stdarg.h>

void ASY_raw(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void ASY_globalVar(char *varName, char *varTag, char *varValue) {
	ASY_raw("%s: .%s %s\n", varName, varTag, varValue);
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
}

void ASY_functionCall(char *funcName, int paramsStackSize) {
	ASY_raw("call %s\n", funcName);
	ASY_raw("addl $%d, %%esp\n", paramsStackSize);
}

void ASY_functionEnding() {
	ASY_raw("movl %%ebp, %%esp\n");
	ASY_raw("popl %%ebp\n");
	ASY_raw("ret\n");
}

void ASY_malloc(char *param) {
	ASY_raw("pushl %s\n", param);
	ASY_functionCall("malloc", 4);
}
