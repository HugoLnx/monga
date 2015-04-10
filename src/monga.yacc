%{
#include <stdio.h>
#include <string.h>
%}
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

block : TK_CURLY_BRACE_OPEN type dec_variable TK_CURLY_BRACE_CLOSE
      ;
