typedef enum {END,NUMBER} token;

union {
	int ival;
	char *text;
} yylval;
