#include "tables.hpp"
#include "string.h"
#include <stdio.h>

#define PART_SIZE_TABLE 4


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
	"[",
	"]",		// 17
	0
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
	LEX_LBRACKET,
	LEX_RBRACKET,
	LEX_NULL
};


const char * TableLexem:: function [] =
{
	""
	"?buy",			// 1
	"?prod",		// 2
	"?sell",		// 3
	"?turn",		// 4
	0
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



char * StorageTypeLex:: get_name ()
{
	return name;
}


void StorageTypeLex:: put_name (const char *str)
{
	name = new char [ strlen (str) + 1 ];
	strcpy (name, str);
}


type_of_lex StorageTypeLex:: get_type ()
{
	return type;
}


void StorageTypeLex::put_type (type_of_lex t)
{
	type = t;
}


StorageTypeLex:: ~StorageTypeLex ()
{
	delete [] name;
}


void StorageTypeLex:: operator= (const StorageTypeLex & str)
{
	name = new char [ strlen (str.name) ];
	strcpy (name, str.name);

	type = str.type;
	value = str.value;
}



void TableStorageTypeLex:: extend_table ()
{
	int new_size = 2 * size;	
	StorageTypeLex * news = new StorageTypeLex [ new_size ];

	for ( int i = 1; i < top; ++i ) {
		news[i] = s[i];
	}
	
	delete [] s;
	
	s = news;
	size = new_size;

}


TableStorageTypeLex:: TableStorageTypeLex ()
	: size (4)
{
	s = new StorageTypeLex [ size ];
	top = 1;
}


StorageTypeLex & TableStorageTypeLex:: operator[] (int k)
{
	return s[k];
}


int TableStorageTypeLex:: put (const char * buf)
{
	if ( top == size - 1 ) {
		extend_table ();
	}

	for ( int i = 1; i < top; i++ ) {
		if ( strcmp (buf, s[i].get_name ()) == 0 ) {
			return i;	
		}
	}

	s[top].put_name (buf);
	top++;

	return (top - 1);
}

void TableStorageTypeLex:: operator= (const TableStorageTypeLex & t)
{
	printf ("start copy.\n");

	name = new char [ strlen (t.name) ];
	strcpy (name, t.name);

	type = t.type;
	value = t.value;


	s = new StorageTypeLex [ top ];

	for ( int i = 1; i < top; ++i ) {
		
		s[i] = t.s[i];
	}

	size = t.size;
	top = t.top;
}
	

void TableArrayStorageTypeLex:: extend_table ()
{
	printf ("Extend table array.\n");
	int new_size = 2 * size;	
	TableStorageTypeLex * new_t = new TableStorageTypeLex [ new_size ];

	for ( int i = 1; i < top; ++i ) {
		new_t[i] = t[i];
	}
	
	delete [] t;
	
	t = new_t;
	size = new_size;
	
	printf ("End of ext table array.\n");
}

TableStorageTypeLex:: ~TableStorageTypeLex ()
{
	delete [] s;
}

int TableArrayStorageTypeLex:: put (const char  * buf)
{
	if ( top == size - 1 ) {
		extend_table ();
	}
	
	for ( int i = 1; i < top; i++ ) {
		if ( strcmp (buf, t[i].get_name ()) == 0 ) {
			return i;	
		}
	}
	
	t[top].put_name (buf);
	top++;

	printf ("Create new array.\n");

	return (top - 1);
}

TableArrayStorageTypeLex:: TableArrayStorageTypeLex ()
	: size (PART_SIZE_TABLE)
{
	t = new TableStorageTypeLex [size];
	top = 1;	
}


TableArrayStorageTypeLex:: ~TableArrayStorageTypeLex ()
{
	delete [] t;
}

TableStorageTypeLex & 
TableArrayStorageTypeLex:: operator [] (int k)
{
	return t[k];
}
