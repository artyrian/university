#include "tables.hpp"
#include "string.h"
#include <stdio.h>

#define PART_SIZE_TABLE 4


const char * TableLexem:: word [] =
{
	""		// 0 dont use.
	"do", 		// 1
	"goto",		// 2
	"else",		// 3
	"if",		// 4
	"then",		// 5
	"while",	// 6
	"begin",
	"end",
	0
};


type_of_lex TableLexem:: lex_word [] =
{
	LEX_NULL,			// 0
	LEX_DO,				// 1
	LEX_GOTO,			// 2
	LEX_ELSE,			// 3
	LEX_IF,				// 4
	LEX_THEN,			// 5
	LEX_WHILE,			// 6
	LEX_BEGIN,
	LEX_END,
	LEX_NULL
};


const char * TableLexem:: delim [] =
{
	""		// 0 dont use.
	";",		// 1
	",",		// 2
	"(", 		// 3
	")",		// 4
	"=",		// 5
	"<",		// 6
	">",		// 7
	"+",		// 8
	"-",		// 9
	"*",		// 10
	"/",		// 11
	">=",		// 12
	"<=",		// 13
	"<>",		// 14
	"[",		// 15
	"]",		// 16
	//"{",		// 17
	//"}",		// 18
	"|",		// 19
	"&",		// 20
	"!",		// 21
	0
};


type_of_lex TableLexem:: lex_delim [] =
{
	LEX_NULL,		// 0 
	LEX_FIN,
	LEX_COMMA,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_EQ,
	LEX_LESS,
	LEX_GREATER,
	LEX_PLUS,
	LEX_MINUS,		// 9
	LEX_MULTIPLY,
	LEX_DIVISION, 
	LEX_LEQ,		// 12
	LEX_GEQ,		// 13
	LEX_LG,			// 14
	LEX_LBRACKET,		// 15
	LEX_RBRACKET,		//
//	LEX_BEGIN,
//	LEX_END,
	LEX_OR,			
	LEX_AND,		// 20
	LEX_NEQ,
	LEX_NULL
};


const char * TableLexem:: function [] =
{
	""
	"?buy",			// 1
	"?prod",		// 2
	"?sell",		// 3
	"?turn",		// 4
	"?raw",			// 5
	0
};


type_of_lex TableLexem:: lex_function [] =
{
	LEX_NULL,
	LEX_BUY,	// 1
	LEX_PROD,	// 2
	LEX_SELL,	// 3
	LEX_TURN,	// 4
	LEX_RAW,	// 5
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
	: size (PART_SIZE_TABLE)
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
	int new_size = 2 * size;	
	TableStorageTypeLex * new_t = new TableStorageTypeLex [ new_size ];

	for ( int i = 1; i < top; ++i ) {
		new_t[i] = t[i];
	}
	
	delete [] t;
	
	t = new_t;
	size = new_size;
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
