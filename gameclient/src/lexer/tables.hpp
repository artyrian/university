#ifndef _TABLES_HPP_
#define _TABLES_HPP_

enum type_of_lex {
	LEX_NULL,				// 0
	LEX_AND,				// 1 
	LEX_BEGIN, 				// 2
	LEX_BOOL,				// 3
	LEX_DO,					// 4
	LEX_ELSE,				// 5
	LEX_END,				// 6
	LEX_IF,					// 7
	LEX_FALSE,				// 8
	LEX_INT,				// 9
	LEX_NOT,				// 10
	LEX_OR,					// 11
	LEX_PROGRAM,				// 12
	LEX_READ,				// 13
	LEX_THEN,				// 14
	LEX_TRUE,				// 15
	LEX_VAR,				// 16
	LEX_WHILE,				// 17
	LEX_WRITE,				// 18
	LEX_FIN,				// 19
	LEX_SEMICOLON, 				// 20
	LEX_COMMA, 
	LEX_COLON,
	LEX_ASSIGN,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_EQ,
	LEX_LSS,
	LEX_GTR,
	LEX_PLUS,
	LEX_MINUS,				// 30
	LEX_TIMES,
	LEX_SLASH, 
	LEX_LEQ,
	LEX_NEQ,
	LEX_GEQ,				// 35
	LEX_NUM,				// 36
	LEX_ID,					// 37
	POLIZ_LABEL,				// 38
	POLIZ_ADDRESS,				// 39
	POLIZ_GO,				// 40
	POLIZ_FGO				// 41
};

struct TableLexem {
	static const char *		word [];
	static const char * 		delim [];
	static type_of_lex		lex_word [];
	static type_of_lex 		lex_delim [];
};

#endif
