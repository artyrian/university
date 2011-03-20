#include "lexer.hpp"
#include <stdio.h>
#include <string.h>


Lex:: Lex ( type_of_lex t = LEX_NULL, int v = 0)
	:	t_lex (t), v_lex (v)
{
}

type_of_lex Lex:: get_type ()
{
	return t_lex;
}

int Lex:: get_value ()
{
	return v_lex;
}

void Lex:: print ()
{
	printf ("(%d,%d)", t_lex, v_lex);

}


Ident:: Ident ()
	: declare (false), assign (false)
{
}

char * Ident:: get_name ()
{
	return name;
}

void Ident:: put_name (const char * str)
{
	name = new char [ strlen (str) + 1 ];
	strcpy (name, str);
}

bool Ident:: get_declare ()
{
	return declare;
}

void Ident:: put_declare ()
{
	declare = true;
}

type_of_lex Ident:: get_type ()
{
	return type;
}

void Ident:: put_type ( type_of_lex t)
{
	type = t;
}

bool Ident:: get_assign ()
{
	return assign;
}

void Ident:: put_assign ()
{
	assign = true;
}

int Ident:: get_value ()
{
	return value;
}

void Ident:: put_value (int v)
{
	value = v;
}



TableIdent:: TableIdent (int max_size)
{
	p = new Ident [ size = max_size ];
	top = 1;
}

Ident& TableIdent:: operator[] (int k)
{
	return p[k];
}

int TableIdent:: put (const char * buf)
{
	for ( int i = 1; i <= top; ++i ) {
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



int Scanner::look (const char * buf, char ** list)
{
	int i = 0;

	while ( list [i] != 0 ) {
		if ( strcmp (buf, list [i]) == 0 ) {
			return i;
		}
	}

	return 0;
}


Scanner::Scanner ()
	: buf ()
{
}

Lex Scanner::feed_symbol (int c)
{
	CS = H;
	do {
		switch ( CS ) {
		case H:

			break;
		case NUM:

			break;
		case IDENT:

			break;
		case KW:

			break;
		case AS:

			break;
		case STR:

			break;
		}
	} while ( true );
	
	return Lex();
}
