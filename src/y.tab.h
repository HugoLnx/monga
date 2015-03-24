#define FOREACH_TOKEN(TOKEN) \
	TOKEN(END) \
  TOKEN(NUMBER) \
  TOKEN(TEXT) \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum {
	FOREACH_TOKEN(GENERATE_ENUM)
} token;

static const char *TOKEN_STR[] = {
	FOREACH_TOKEN(GENERATE_STRING)
};

union {
	int ival;
	char *text;
} yylval;
