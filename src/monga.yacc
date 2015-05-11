%{
#include <stdio.h>
#include <string.h>
#include "./ast_tree.h"
ndProgram *pProgram = NULL;
%}
%union {
 	long long int ival;
 	char *text;
  double fval;
	void* pNode;
}

%type <pNode> program dec_function block type declaration parameters parameter
	dec_variable names_list
	var_declarations statement_list statement
	attribution return_call exp var
%type <ival> base_type

%token
	END INVALID
	TK_INT TK_CHAR TK_FLOAT TK_VOID
	TK_IF TK_ELSE TK_WHILE TK_NEW TK_RETURN
	TK_CURLY_BRACE_OPEN  TK_SQUARE_BRACKET_OPEN  TK_PARENTHESES_OPEN
	TK_CURLY_BRACE_CLOSE TK_SQUARE_BRACKET_CLOSE TK_PARENTHESES_CLOSE
	TK_SEMICOLON TK_COMMA
	TK_PLUS TK_MINUS TK_ASTERISK TK_SLASH
	TK_EXCLAMATION_MARK
	TK_DOUBLE_EQUAL TK_ONE_EQUAL TK_BANG_EQUAL
	TK_LESS_EQUAL TK_GREATER_EQUAL TK_LESS TK_GREATER
  TK_OR TK_AND
%token <text>
  TEXT CHAR COMMENT TK_ID
%token <ival>
	NUMBER HEXADECIMAL
%token <fval>
  FLOAT

%left TK_AND TK_OR
%left TK_BANG_EQUAL TK_DOUBLE_EQUAL TK_LESS_EQUAL TK_LESS TK_GREATER_EQUAL TK_GREATER TK_EXCLAMATION_MARK
%left TK_MINUS TK_PLUS
%left TK_ASTERISK TK_SLASH
%nonassoc TK_SQUARE_BRACKET_OPEN
%nonassoc aux
%nonassoc TK_ELSE
%%
program : type declaration { $$ = createProgramNode((tpType*)$1, (tpDeclaration*)$2); }
        | TK_VOID dec_function { $$ = createProgramNode(NULL, NULL); }
				;

declaration : dec_variable { $$ = newDeclaration($1, DEC_VARIABLE); }
        | dec_function { $$ = newDeclaration($1, DEC_FUNCTION); }
				;

dec_variable : names_list TK_SEMICOLON { $$ = $1; }
             ;

names_list : TK_ID { $$ = createVariablesNode($1); }
           | TK_ID TK_COMMA names_list { $$ = $3; addVariable((ndVariables*)$$, $1); }
					 ;

type : base_type { $$ = newType($1, 0); }
     | type TK_SQUARE_BRACKET_OPEN TK_SQUARE_BRACKET_CLOSE { $$ = $1; incrDepth((tpType*)$$); }
		 ;
base_type : TK_INT { $$ = TK_INT; }
          | TK_CHAR { $$ = TK_CHAR; }
					| TK_FLOAT { $$ = TK_FLOAT; }
					;

dec_function : TK_ID TK_PARENTHESES_OPEN parameters TK_PARENTHESES_CLOSE block { $$ = createFunctionNode($1, (ndParameters*) $3, (ndBlock*)$5); }
						 | TK_ID TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE block { $$ = createFunctionNode($1, NULL, (ndBlock*)$4); }
						 ;
parameters : parameter { $$ = createParametersNode((ndParameter*) $1); }
           | parameter TK_COMMA parameters { $$ = $3; addParam((ndParameters*)$$, (ndParameter*)$1); }
					 ;
parameter : type TK_ID { $$ = createParameterNode((tpType*)$1, $2); }
          ;

block : TK_CURLY_BRACE_OPEN var_declarations TK_CURLY_BRACE_CLOSE { $$ = createBlockNode((ndVarDeclarations*)$2, NULL); }
      | TK_CURLY_BRACE_OPEN statement_list TK_CURLY_BRACE_CLOSE { $$ = createBlockNode(NULL, (ndStatements*)$2); }
      | TK_CURLY_BRACE_OPEN var_declarations statement_list TK_CURLY_BRACE_CLOSE { $$ = createBlockNode((ndVarDeclarations*)$2, (ndStatements*)$3); }
      | TK_CURLY_BRACE_OPEN TK_CURLY_BRACE_CLOSE { $$ = createBlockNode(NULL, NULL); }
      ;

var_declarations : type dec_variable { $$ = createVarDeclarations((tpType*)$1, (ndVariables*)$2); }
                 | type dec_variable var_declarations { $$ = $3; addVarDeclaration((ndVarDeclarations*)$$, (tpType*)$1, (ndVariables*)$2); }
                 ;

statement_list : statement { $$ = createStatements((ndStatement*)$1); }
               | statement statement_list { $$ = $2; addStatement((ndStatements*)$$, (ndStatement*)$1); }
               ;

statement : attribution TK_SEMICOLON { $$ = createStatement($1, STAT_ATTRIBUTION); }
          | function_call TK_SEMICOLON { $$ = createStatement($1, STAT_FUNCTION_CALL); }
          | return_call TK_SEMICOLON { $$ = createStatement($1, STAT_RETURN_CALL); }
          | block { $$ = createStatement($1, STAT_BLOCK); }
					| TK_WHILE TK_PARENTHESES_OPEN exp TK_PARENTHESES_CLOSE statement { $$ = createStatement(NULL, STAT_WHILE); /* INCOMPLETE */}
          | if_statement { $$ = createStatement(NULL, STAT_IF); /* INCOMPLETE */}
          ;

if_statement : if %prec aux
             | if TK_ELSE statement
             ;

if : TK_IF TK_PARENTHESES_OPEN exp TK_PARENTHESES_CLOSE statement
   ;

return_call : TK_RETURN { $$ = createReturnNode(NULL); }
            | TK_RETURN exp { $$ = createReturnNode((ndExpression*)$2); }
            ;

attribution : var TK_ONE_EQUAL exp { $$ = createAttributionNode((ndVar*)$1, (ndExpression*)$3); }
            ;

var : TK_ID { $$ = createVarNode($1); }
    | exp TK_SQUARE_BRACKET_OPEN exp TK_SQUARE_BRACKET_CLOSE { $$ = createArrayVarNode((ndExpression*)$1, (ndExpression*)$3); }
    ;

exp : NUMBER { $$ = createExpressionIntegerNode($1, EXP_NUMBER); }
    | HEXADECIMAL { $$ = createExpressionIntegerNode($1, EXP_HEXADECIMAL); }
    | CHAR { $$ = createExpressionIntegerNode($1[1], EXP_CHAR); }
		| FLOAT { $$ = createExpressionFloatNode($1); }
    | TEXT { $$ = createExpressionTextNode($1); }
    | var { $$ = createExpressionGenericNode($1, EXPND_VAR); }
    | TK_PARENTHESES_OPEN exp TK_PARENTHESES_CLOSE { $$ = $2; }
    | TK_NEW type TK_SQUARE_BRACKET_OPEN exp TK_SQUARE_BRACKET_CLOSE { void *pNewNode = createNewNode((tpType*)$2, (ndExpression*)$4); $$ = createExpressionGenericNode(pNewNode, EXPND_NEW); }
    | exp TK_DOUBLE_EQUAL exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_DOUBLE_EQUAL); }
    | exp TK_BANG_EQUAL exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_BANG_EQUAL); }
    | exp TK_GREATER_EQUAL exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_GREATER_EQUAL); }
    | exp TK_GREATER exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_GREATER); }
    | exp TK_LESS_EQUAL exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_LESS_EQUAL); }
    | exp TK_LESS exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_LESS); }
    | TK_EXCLAMATION_MARK exp
    | exp TK_AND exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_AND); }
    | exp TK_OR exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_OR); }
    | exp TK_PLUS exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_PLUS); }
    | exp TK_MINUS exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_MINUS); }
    | exp TK_ASTERISK exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_ASTERISK); }
    | exp TK_SLASH exp { $$ = createExpressionBinaryNode((ndExpression*)$1, (ndExpression*)$3, EXPBIN_SLASH); }
    | TK_MINUS exp %prec TK_ASTERISK %prec TK_SLASH
    ;

exp_list : exp { $$ = createExpListNode((ndVar*)$1); }
         | exp TK_COMMA exp_list { $$ = $3; addExpListNode((ndExpList*)$$, (ndExpression*)$1); }
         ;

function_call : TK_ID TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE { $$ = createFunctionCall((char*)$1, NULL); }
              | TK_ID TK_PARENTHESES_OPEN exp_list TK_PARENTHESES_CLOSE { $$ = createFunctionCall((char*)$1, (ndExpList*)$3); }
              ;
