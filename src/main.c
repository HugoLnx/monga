#include <stdio.h>
#include "label_generator.h"

int main()
{
	printf(LBL_generate(LBL_next()));
  return 0;
}

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

