#include "tables.hpp"
#include <string.h>
#include <stdio.h>


const char * PrintTable:: lexem [] =
{
	"",
	"LEX_GOTO",
	"LEX_IF",
	"LEX_FOR",
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
	"LEX_LEQ",
	"LEX_GEQ",
	"LEX_LG",
	"LEX_PLUS",
	"LEX_MINUS",
	"LEX_MULTIPLY",
	"LEX_DIVISION",
	"LEX_LBRACKET",
	"LEX_RBRACKET",
	"LEX_OR",
	"LEX_AND",
	"LEX_NEG",
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
	0
};


const char * TableLexem:: word [] =
{
	""		// 0 dont use.
	"goto",		// 2
	"if",		// 4
	"for",
	"while",	// 6
	0
};


type_of_lex TableLexem:: lex_word [] =
{
	LEX_NULL,			// 0
	LEX_GOTO,			// 2
	LEX_IF,				// 4
	LEX_FOR,
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
	LEX_PLUS,
	LEX_MINUS,		// 9
	LEX_MULTIPLY,
	LEX_DIVISION, 
	LEX_LBRACKET,		// 15
	LEX_RBRACKET,		//
	LEX_OR,			
	LEX_AND,		// 20
	LEX_NEG,
	LEX_BEGIN,
	LEX_END,
	LEX_NULL
};


const char * TableLexem:: compare [] =
{
	""		// 0 dont use.
	"=",		// 
	"<",		// 
	">",		// 
	"<=",
	">=",
	"<>",
	0
};

type_of_lex TableLexem:: lex_compare [] =
{
	LEX_NULL,		// 0 
	LEX_EQ,
	LEX_LESS,
	LEX_GREATER,
	LEX_LEQ,
	LEX_GEQ,
	LEX_LG,
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



int * StorageTypeLex:: get_address_value ()
{
	return & value;
}


void StorageTypeLex:: put_name (const char *str)
{
	name = new char [ strlen (str) + 1 ];
	strcpy (name, str);
}


StorageTypeLex:: ~StorageTypeLex ()
{
	delete [] name;
}


//----------------------------------------------------------------


TableStorageTypeLex:: TableStorageTypeLex ()
{
	storage_list = 0;
	size = 0;
}


TableStorageTypeLex:: Storage::
Storage ( StorageTypeLex * p_s, int p_num, Storage * p_next )
{
	s = p_s;
	num = p_num;
	next = p_next;
}



int TableStorageTypeLex:: put (const char * buf)
{
	Storage * cur = storage_list;
	int i = 1;

	while ( i <= size ) {
		if ( strcmp ( buf, cur->s->name ) == 0 ) {
			return cur->num;	
		}
		cur = cur->next;
		++ i;
	}


	Storage * old_storage_list = storage_list;
	storage_list = new Storage ( new StorageTypeLex, ++ size, old_storage_list );
	storage_list->s->put_name (buf);

	return size;
}

/*
StorageTypeLex * TableStorageTypeLex:: operator [] (int k) 
{
	Storage * cur = storage_list;
	int i = 1;

	while ( i <= size ) {
		if ( cur->num == k ) {
			return cur->s;
		}
		cur = cur->next;
		++ i;
	}
	
	perror ("Wrong parameter of index in TableStorageTypeLex.\n");
	return 0;
}
*/

StorageTypeLex * TableStorageTypeLex:: index (int idx)
{
	Storage * cur = storage_list;
	int i = 1;

	while ( i <= size ) {
		if ( cur->num == idx ) {
			return cur->s;
		}
		cur = cur->next;
		++ i;
	}
	
	throw ("Wrong parameter of index in TableStorageTypeLex.\n");
	return 0;
}


TableStorageTypeLex:: ~TableStorageTypeLex ()
{
	Storage * cur = storage_list;

	while ( cur != 0 ) {
		storage_list = storage_list->next;	
		delete cur->s;
		delete cur;
		cur = storage_list;
	}
}



void TableStorageTypeLex:: print () const
{
	Storage * cur = storage_list;
	int i = 1;

	printf ("Start print storage type lex.\n");

	while ( i <= size ) {
		printf ("%d) %d.\n", i, cur->s->value);
		cur = cur->next;
		++i;
	}
	
	printf (" end of print storage type lex.\n");
}

//----------------------------------------------------------------

TableArrayStorageTypeLex:: TableStorage:: 
TableStorage (TableStorageTypeLex * p_t, int p_num, TableStorage * p_next )
{
	t = p_t;
	num = p_num;
	next = p_next;
}
	

int TableArrayStorageTypeLex:: put (const char  * buf)
{
	TableStorage * cur = table_storage_list;
	int i = 1;

	while ( i <= size ) {
		if ( strcmp ( buf, cur->t->name ) == 0 ) {
			return cur->num;	
		}
		++ i;
		cur = cur->next;
	}

	TableStorage * old_table_storage_list = table_storage_list;
	
	TableStorageTypeLex * p_table = new TableStorageTypeLex ();

	table_storage_list = new TableStorage ( p_table, ++ size, old_table_storage_list);
	table_storage_list->t->put_name (buf);

	return size;
}



TableArrayStorageTypeLex:: TableArrayStorageTypeLex ()
{
	table_storage_list = 0;
	size = 0;
}


TableArrayStorageTypeLex:: ~TableArrayStorageTypeLex ()
{
	TableStorage * cur = table_storage_list;

	while ( cur != 0 ) {
		table_storage_list = table_storage_list->next;
		delete cur->t;
		delete cur;
		cur = table_storage_list;	
	}
}

/*
TableStorageTypeLex * 
TableArrayStorageTypeLex:: operator [] (int k)
{
	TableStorage * cur = table_storage_list;
	int i = 1;
	
	while ( i <= size ) {
		if ( cur->num  == k ) {
			return cur->t;
		}
		cur = cur->next;
		++ i;
	}

	perror ("Error index in TableArrayStorageTypeLex.\n");
	return 0;
}
*/

TableStorageTypeLex * TableArrayStorageTypeLex:: index (int idx)
{
	TableStorage * cur = table_storage_list;
	int i = 1;
	
	while ( i <= size ) {
		if ( cur->num  == idx ) {
			return cur->t;
		}
		cur = cur->next;
		++ i;
	}

	throw ("Error index in TableArrayStorageTypeLex.");
}


//---------------------------------------------------------


TableLabel:: TableLabel ()
{
	list = 0;
	size = 0;
}



TableLabel:: ~ TableLabel ()
{
	ListElem * cur = list;

	while ( cur != 0 ) {
		list = list->next;
		delete cur->item;
		delete cur;
		cur = list;
	}
}


TableLabel:: Item:: 
Item (int p_label, int p_place)
{
	label = p_label;
	place = p_place;
}


TableLabel:: ListElem::
ListElem ( Item * p_item, int p_num, ListElem * p_next )
{
	item = p_item;
	num = p_num;
	next = p_next;
}


int TableLabel:: put (int label, int place)
{
	ListElem * cur = list;
	int i = 1;

	while ( i <= size ) {
		if ( label == cur->item->label ) {
			return 0;	
		}
		++ i;
		cur = cur->next;
	}

	ListElem * old_list = list;
	list = new ListElem (new Item (label, place), ++ size, old_list);
	list->next = old_list;

	return size;
}


int TableLabel:: look (int label)
{
	ListElem * cur = list;
	int i = 1;

	while ( i <= size ) {
		if ( label == cur->item->label ) {
			return cur->item->place;
		}
		cur = cur->next;
		++ i;
	}

	return 0;
}

int TableLabel:: get_size ()
{
	return size;
}


TableLabel:: Item *
TableLabel:: index (int k)
{
	ListElem * cur = list;
	int i = 1;

	while ( i <= size ) {
		if ( cur->num == k ) {
			return cur->item;
		}
		++ i;
		cur = cur->next;
	}

	throw ("Dont know where go.");
}

void TableLabel:: print () const
{
	ListElem * cur = list;
	int i = 0;
	while ( cur != 0 ) {
		printf ("%d) label= %d, place=%d.\n",
			++i, cur->item->label, cur->item->place
		);	
		cur = cur->next;
	}
}
