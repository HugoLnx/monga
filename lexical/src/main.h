#define FOREACH_TOKEN(TOKEN) \
	TOKEN(END) \
  TOKEN(INVALID) \
  TOKEN(NUMBER) \
  TOKEN(HEXADECIMAL) \
  TOKEN(FLOAT) \
  TOKEN(TEXT) \
  TOKEN(CHAR) \
  TOKEN(COMMENT) \
  TOKEN(TK_ID) \
  TOKEN(TK_INT) \
  TOKEN(TK_CHAR) \
  TOKEN(TK_FLOAT) \
  TOKEN(TK_VOID) \
  TOKEN(TK_IF) \
  TOKEN(TK_ELSE) \
  TOKEN(TK_WHILE) \
  TOKEN(TK_NEW) \
  TOKEN(TK_RETURN) \
  TOKEN(TK_CURLY_BRACE_OPEN) \
  TOKEN(TK_CURLY_BRACE_CLOSE) \
  TOKEN(TK_SQUARE_BRACKET_OPEN) \
  TOKEN(TK_SQUARE_BRACKET_CLOSE) \
  TOKEN(TK_PARENTHESES_OPEN) \
  TOKEN(TK_PARENTHESES_CLOSE) \
  TOKEN(TK_SEMICOLON) \
  TOKEN(TK_PLUS) \
  TOKEN(TK_MINUS) \
  TOKEN(TK_ASTERISK) \
  TOKEN(TK_SLASH) \
  TOKEN(TK_EXCLAMATION_MARK) \
  TOKEN(TK_DOUBLE_EQUAL) \
  TOKEN(TK_ONE_EQUAL) \
  TOKEN(TK_BANG_EQUAL) \
  TOKEN(TK_LESS_EQUAL) \
  TOKEN(TK_GREATER_EQUAL) \
  TOKEN(TK_LESS) \
  TOKEN(TK_GREATER) \
  TOKEN(TK_OR) \
  TOKEN(TK_AND) \
  TOKEN(TK_COMMA) \


#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum {
	FOREACH_TOKEN(GENERATE_ENUM)
} token;

static const char *TOKEN_STR[] = {
	FOREACH_TOKEN(GENERATE_STRING)
};

union {
	long long int ival;
	char *text;
  double fval;
} yylval;
