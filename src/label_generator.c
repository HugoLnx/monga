#include "label_generator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int nextLabelNumber = 0;

unsigned int LBL_next()
{
  return nextLabelNumber++;
}

char *LBL_generate(unsigned int labelNumber) {
	char *label = (char*) malloc(sizeof(char)*12);
	memset(label, '\0', 12);
	sprintf(label, "l%u", labelNumber);
	return label;
}
