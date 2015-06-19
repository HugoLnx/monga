#include <string.h>
#include <malloc.h>

char *strDup(char *str) {
	char* dup = (char*) malloc(sizeof(char)*(strlen(str)+1));
	strcpy(dup, str);
	return dup;
}

char *toRawStr(char *str) {
	int index = 0, counterPointerMove = 0;
	int size = strlen(str);
	char *outStr;
	outStr = (char *) malloc(sizeof(char)*size);

	while(index < size) {
		if(str[index] == '\\') {
			switch(str[index+1]) {
				case 'n':
					*outStr++ = '\n';
					counterPointerMove++;
					break;
				case 't':
					*outStr++ = '\t';
					counterPointerMove++;
					break;
				default:
					// erro
					break;
			}
			index++;
		}
		else{
			*outStr++ = str[index];
			counterPointerMove++;
		}
		index++;
	}
	
	while(counterPointerMove > 0){
		counterPointerMove--;
		*outStr--;
	}
	
	return outStr;
}