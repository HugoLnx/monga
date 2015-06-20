#include <stdio.h>
#include "label_generator.h"
#include "assembly_writer.h"

int main()
{
	ASY_raw(".data\n");
	ASY_globalVar("msg", "string", "hello world!");
	ASY_raw(".text\n");
	ASY_function("main");
	ASY_functionBeginning();
	ASY_functionCall("printf", 1, "$msg");
	ASY_functionEnding();
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

