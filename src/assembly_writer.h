#include <stdio.h>
void ASY_globalVar(char *varName, char *varType, char *varValue);
void ASY_function(char *funcName);
void ASY_label(char *labelName);
void ASY_functionBeginning();
void ASY_functionCall(char *funcName, int qntParams, ...);
void ASY_functionEnding();
void ASY_raw(char *fmt, ...);
