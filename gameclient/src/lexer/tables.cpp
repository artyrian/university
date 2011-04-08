#include "tables.hpp"
#include <string.h>
#include <stdio.h>

#define PART_SIZE_TABLE 4

const char * PrintTable:: lexem [] =
{
	"",
	"LEX_DO",
	"LEX_GOTO",
	"LEX_IF",
	"LEX_THEN",
	"LEX_WHILE",
	"LEX_BEGIN",
	"LEX_END",
	"LEX_FIN",
	"LEX_COMMA",
	"LEX_LPAREN",
	"LEX_RPAREN",
	"LEX_EQ",
	"LEX_LESS",
	"LEX_GREATER",
	"LEX_PLUS",
	"LEX_MINUS",
	"LEX_MULTIPLY",
	"LEX_DIVISION",
	"LEX_LBRACKET",
	"LEX_RBRACKET",
	"LEX_OR",
	"LEX_AND",
	"LEX_NEQ",
	"LEX_ASSIGN",
	"LEX_NUM",
	"LEX_ID",
	"LEX_ARRAY",
	"LEX_LABEL", 
	"LEX_STR",
	"LEX_BUY",
	"LEX_SELL",
	"LEX_PROD",
	"LEX_BUILD",
	"LEX_TURN",
	"LEX_PRINT",
	"LEX_CUR_MONTH",
	"LEX_PLAYERS",
	"LEX_ACTIVE_PLAYERS",
	"LEX_SUPPLY",
	"LEX_RAW_PRICE",
	"LEX_DEMAND",
	"LEX_PRODUCTION_PRICE",
	"LEX_MONEY",
	"LEX_RAW",
	"LEX_PRODUCTION",
	"LEX_FACTORIES",
	"LEX_AUTO_FACTORIES",
	"LEX_MANUFACTURED",
	"LEX_RESULT_RAW_SOLD",
	"LEX_RESULT_RAW_PRICE",
	"LEX_RESULT_PROD_BOUGHT",
	"LEX_RESULT_PROD_PRICE",
	"POLIZ_ADDRESS",
	"POLIZ_GO",
	"POLIZ_LABEL",
	"POLIZ_FGO",
	"POLIZ_NOP",
	0
};


const char * TableLexem:: word [] =
{
	""		// 0 dont use.
	"do", 		// 1
	"goto",		// 2
	"if",		// 4
	"then",		// 5
	"while",	// 6
	0
};


type_of_lex TableLexem:: lex_word [] =
{
	LEX_NULL,			// 0
	LEX_DO,				// 1
	LEX_GOTO,			// 2
	LEX_IF,				// 4
	LEX_THEN,			// 5
	LEX_WHILE,			// 6
	LEX_NULL
};


const char * TableLexem:: action [] =
{
	""		// 0 dont use.
	"buy",
	"sell",
	"prod",
	"build",
	"turn",
	"print",
	0
};


type_of_lex TableLexem:: lex_action [] =
{
	LEX_NULL,
	LEX_BUY,
	LEX_SELL,
	LEX_PROD,			// 
	LEX_BUILD,	
	LEX_TURN,	 
	LEX_PRINT,
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
	"[",		// 12
	"]",		// 13
	"|",		// 14
	"&",		// 15
	"!",		// 16
	"{",		// 17
	"}",	
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
	LEX_LBRACKET,		// 15
	LEX_RBRACKET,		//
	LEX_OR,			
	LEX_AND,		// 20
	LEX_NEQ,
	LEX_BEGIN,
	LEX_END,
	LEX_NULL
};


const char * TableLexem:: function [] =
{
	""
	"?cur_month",
	"?players",
	"?a_players",
	"?supply",
	"?raw_price",
	"?demand",
	"?production_price",
	"?money",		// 13
	"?raw",
	"?production",
	"?factories",
	"?a_factories",
	"?manufactured",
	"?result_raw_sold",
	"?result_raw_price",
	"?result_prod_bought",
	"?result_prod_price",
	0
};


type_of_lex TableLexem:: lex_function [] =
{
	LEX_NULL,
	LEX_CUR_MONTH,
	LEX_PLAYERS,
	LEX_ACTIVE_PLAYERS,
	LEX_SUPPLY,
	LEX_RAW_PRICE,
	LEX_DEMAND,
	LEX_PRODUCTION_PRICE,
	LEX_MONEY,
	LEX_RAW,
	LEX_PRODUCTION,
	LEX_FACTORIES,
	LEX_AUTO_FACTORIES,
	LEX_MANUFACTURED,
	LEX_RESULT_RAW_SOLD,
	LEX_RESULT_RAW_PRICE,
	LEX_RESULT_PROD_BOUGHT,
	LEX_RESULT_PROD_PRICE,
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
