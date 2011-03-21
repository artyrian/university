#include "tables.hpp"


const char * TableLexem:: word [] =
{
	""		// 0 dont use.
	"and",		// 1
	"begin",	// 2 
	"bool",		// 3
	"do", 		// 4
	"else",		// 5
	"end", 		// 6
	"if",		// 7
	"false",	// 8
	"int",		// 9
	"not",		// 10
	"or",		// 11
	"program",	// 12
	"read",		// 13
	"then",		// 14
	"true",		// 15
	"var",		// 16
	"while",	// 17
	"write", 	// 18	
	0
};

const char * TableLexem:: delim [] =
{
	""		// 0 dont use.
	";",		// 1
	"@",		// 2 
	",",		// 3
	"(", 		// 4
	")",		// 5
	"=",		// 6
	"<",		// 7
	">",		// 8
	"+",		// 9
	"-",		// 10
	"*",		// 11
	"/",		// 12
	"!",		// 13 
	":=",		// 14
	0
};

type_of_lex TableLexem:: lex_word [] =
{
	LEX_NULL,			// 0
	LEX_AND,			// 1
	LEX_BEGIN,			// 2
	LEX_BOOL,			// 3
	LEX_DO,				// 4
	LEX_ELSE,			// 5
	LEX_END,			// 6
	LEX_IF,				// 7
	LEX_FALSE,			// 8
	LEX_INT,			// 9
	LEX_NOT,			// 10
	LEX_OR,				// 11
	LEX_PROGRAM,			// 12
	LEX_READ,			// 13
	LEX_THEN,			// 14
	LEX_TRUE,			// 15
	LEX_VAR,			// 16
	LEX_WHILE,			// 17
	LEX_WRITE,			// 18
	LEX_NULL			// 19
};

type_of_lex TableLexem:: lex_delim [] =
{
	LEX_NULL,		// 0 
	LEX_FIN,
	LEX_SEMICOLON,
	LEX_COMMA,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_EQ,
	LEX_LSS,
	LEX_GTR,
	LEX_PLUS,
	LEX_MINUS,		// 10
	LEX_TIMES,
	LEX_SLASH, 
	LEX_NEQ,		// 13
	LEX_ASSIGN,		// 14
	LEX_NULL		// 15
};
