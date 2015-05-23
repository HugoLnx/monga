#include <string.h>
#include <malloc.h>

char *strDup(char *str) {
	char* dup = (char*) malloc(sizeof(char)*(strlen(str)+1));
	strcpy(dup, str);
	return dup;
}

