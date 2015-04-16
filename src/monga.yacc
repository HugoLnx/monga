%{
#include <stdio.h>
#include <string.h>
%}
%left TK_AND TK_OR
%left TK_BANG_EQUAL TK_DOUBLE_EQUAL TK_LESS_EQUAL TK_LESS TK_GREATER_EQUAL TK_GREATER TK_EXCLAMATION_MARK
%left TK_MINUS TK_PLUS
%left TK_ASTERISK TK_SLASH
%nonassoc TK_SQUARE_BRACKET_OPEN
%token
	END INVALID
	NUMBER HEXADECIMAL FLOAT TEXT CHAR COMMENT
	TK_ID
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
%%
program : type declaration
        | TK_VOID dec_function
				;

declaration : dec_variable
        | dec_function
				;

dec_variable : names_list TK_SEMICOLON
             ;

names_list : TK_ID
           | TK_ID TK_COMMA names_list
					 ;

type : base_type
     | type TK_SQUARE_BRACKET_OPEN TK_SQUARE_BRACKET_CLOSE
		 ;
base_type : TK_INT
          | TK_CHAR
					| TK_FLOAT
					;

dec_function : TK_ID TK_PARENTHESES_OPEN parameters TK_PARENTHESES_CLOSE block
						 | TK_ID TK_PARENTHESES_OPEN TK_PARENTHESES_CLOSE block
						 ;
parameters : parameter
           | parameter TK_COMMA parameters
					 ;
parameter : type TK_ID
          ;

block : TK_CURLY_BRACE_OPEN var_declarations TK_CURLY_BRACE_CLOSE
      | TK_CURLY_BRACE_OPEN statement_list TK_CURLY_BRACE_CLOSE
      | TK_CURLY_BRACE_OPEN var_declarations statement_list TK_CURLY_BRACE_CLOSE
      | TK_CURLY_BRACE_OPEN TK_CURLY_BRACE_CLOSE
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
