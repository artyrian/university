#include "tables.hpp"
#include "string.h"


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
	"string",	// 13.5
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
	">=",		// 13
	"!",		// 14 
	"<=",		// 15
	0
};

const char * TableLexem:: function [] =
{
	""
	"buy",		// 1
	"prod",		// 2
	"sell",		// 3
	"turn",		// 4
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
	LEX_STR,
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
	LEX_LEQ,		// 13
	LEX_NEQ,		// 14
	LEX_GEQ,		// 15
	LEX_NULL
};

type_of_lex TableLexem:: lex_function [] =
{
	LEX_NULL,
	LEX_BUY,	// 1
	LEX_PROD,	// 2
	LEX_SELL,	// 3
	LEX_TURN,	// 4
	LEX_NULL
};


char * String:: get_name ()
{
	return name;
}

void String:: put_name (const char *str)
{
	name = new char [ strlen (str) + 1 ];
	strcpy (name, str);
}

type_of_lex String:: get_type ()
{
	return type;
}

void String::put_type (type_of_lex t)
{
	type = t;
}



void TableString:: extend_table ()
{
	int new_size = 2 * size;	
	String * news = new String [ new_size ];

	for ( int i = 1; i < top; i++ ) {
		news[i] = s[i];
	}
	
	delete [] s;
	
	s = news;
	size = new_size;

}

TableString:: TableString ()
	: size (4)
{
	s = new String [ size ];
	top = 1;
}

String & TableString:: operator[] (int k)
{
	return s[k];
}

int TableString:: put (const char * buf)
{
	if ( top == size - 1 ) {
		extend_table ();
	}

	for ( int i = 1; i < top; ++i ) {
		if ( strcmp (buf, s[i].get_name ()) == 0 ) {
			return i;	
		}
	}

	s[top].put_name (buf);
	top++;

	return (top - 1);
}

TableString:: ~TableString ()
{
	delete [] s;
}



void TableIdent:: extend_table ()
{
	int new_size = 2 * size;	
	Ident * newp = new Ident [ new_size ];

	for ( int i = 1; i < top; i++ ) {
		newp[i] = p[i];
	}
	
	delete [] p;
	
	p = newp;
	size = new_size;
}


TableIdent:: TableIdent ()
	: size (4)
{
	p = new Ident [size];
	top = 1;
}

Ident& TableIdent:: operator[] (int k)
{
	return p[k];
}

int TableIdent:: put (const char * buf)
{
	if ( top == size - 1) {
		extend_table ();
	}

	for ( int i = 1; i < top; ++i ) {
		if ( strcmp (buf, p[i].get_name ()) == 0 ) {
			return i;	
		}
	} 

	p[top].put_name (buf);
	top++;

	return (top - 1);
}

TableIdent:: ~TableIdent ()
{
	delete [] p;
}

TableLexem:: TableLexem ()
	: ident ()
{
}
