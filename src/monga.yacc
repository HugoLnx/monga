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

block : TK_CURLY_BRACE_OPEN var_declarations TK_CURLY_BRACE_CLOSE { $$ = createBlockNode(); /* FIX */}
      | TK_CURLY_BRACE_OPEN statement_list TK_CURLY_BRACE_CLOSE { $$ = createBlockNode(); /* FIX */}
      | TK_CURLY_BRACE_OPEN var_declarations statement_list TK_CURLY_BRACE_CLOSE { $$ = createBlockNode(); /* FIX */}
      | TK_CURLY_BRACE_OPEN TK_CURLY_BRACE_CLOSE { $$ = createBlockNode(); }
      ;

var_declarations : type dec_variable 
                 | type dec_variable var_declarations
                 ;

statement_list : statement
               | statement statement_list
               ;

statement : attribution TK_SEMICOLON
          | function_call TK_SEMICOLON
          | return_call TK_SEMICOLON
          | block
					| TK_WHILE TK_PARENTHESES_OPEN exp TK_PARENTHESES_CLOSE statement
          | if_statement
          ;

if_statement : if %prec aux
             | if TK_ELSE statement
             ;

if : TK_IF TK_PARENTHESES_OPEN exp TK_PARENTHESES_CLOSE statement
   ;

return_call : TK_RETURN
            | TK_RETURN exp
            ;

attribution : var TK_ONE_EQUAL exp
            ;

var : TK_ID
    | exp TK_SQUARE_BRACKET_OPEN exp TK_SQUARE_BRACKET_CLOSE
    ;

exp : NUMBER
		| FLOAT
    | TEXT
    | HEXADECIMAL
    | CHAR
    | var
    | TK_PARENTHESES_OPEN exp TK_PARENTHESES_CLOSE
    | TK_NEW type TK_SQUARE_BRACKET_OPEN exp TK_SQUARE_BRACKET_CLOSE
    | exp TK_DOUBLE_EQUAL exp
    | exp TK_BANG_EQUAL exp
    | exp TK_GREATER_EQUAL exp
    | exp TK_GREATER exp
    | exp TK_LESS_EQUAL exp
    | exp TK_LESS exp
    | TK_EXCLAMATION_MARK exp
    | exp TK_AND exp
    | exp TK_OR exp
    | exp TK_PLUS exp
    | exp TK_MINUS exp
    | exp TK_ASTERISK exp
    | exp TK_SLASH exp
    | TK_MINUS exp %prec TK_ASTERISK %prec TK_SLASH
    ;

exp_list : exp
         | exp TK_COMMA exp_list
         ;

function_call : TK_ID TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE 
              | TK_ID TK_PARENTHESES_OPEN exp_list TK_PARENTHESES_CLOSE 
              ;
