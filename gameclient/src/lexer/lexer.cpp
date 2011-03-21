#include "lexer.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


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

Ident::~Ident ()
{
	delete [] name;
}

char * String:: get_name ()
{
	return name;
}

void put_name (const char *str)
{
	name = new char [ strlen (str) + 1 ];
	strcpy (name, str);
}

type_of_lex get_type ()
{
	return type;
}

void String::put_type (type_of_lex t)
{
	type = t;
}

void TableIdent:: extend_table ()
{
	perror ("Not enought place to next ident.\n");
	
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

TableString::TableString ()
	: size (1);
{
	s = new String [ size ];
}

String & TableString:: operator (int k)
{
	return s[k];
}

int TableString:: put (const char * buf)
{
	extend_table ();
	
	
	return ;
}

TableString::~TableString ()
{
	delete [] s;
}



int Scanner::look (const char * buf, const char ** list)
{
	int i = 0;

	while ( list [i] != 0 ) {
		if ( strcmp (buf, list [i]) == 0 ) {
			return (i + 1);
		}
		++i;
	}
	printf ("buf:[%s]\n", buf);
	return 0;
}


Scanner::Scanner ()
	: table_ident ()
{
	buffer = new Buffer;

	count_str = 1;
	CS = H;
}


Scanner:: ~Scanner ()
{
	delete buffer;
}

int Scanner:: isdelim (int c)
{
	const char smb [] = "+-*/%<>=&|!()[];,@";
	int i = 0;

	while ( smb[i] != '\0' ) {
		if ( smb[i++] == c ) {
			return 1;
		}
	}
	
	return 0;
}

Lex Scanner::feed_symbol (int c)
{
	int i = 0;

	switch ( CS ) {
	case H:
		if ( c == ' ' || c == '\n' || c == '\r' || c == '\t' ) {
			if ( c == '\n' ) {
				count_str++;
			}
			return Lex (LEX_NULL, 0);
		}
		else if ( isdigit (c) ) {
			digit = c - '0';
			CS = NUM;
			return Lex (LEX_NULL, 0);
		}
		else if ( isalpha (c) ) {
			buffer->clear ();
			buffer->add (c);
			CS = KW;
			return Lex (LEX_NULL, 0);
		}
		else if ( c == '!' ) {
			buffer->clear ();
			buffer->add (c);
			CS = NEQ;
			return Lex (LEX_NULL, 0);
		}
		else if ( c == '?' ) {
			buffer->clear ();
			buffer->add (c);
			CS = FN;
			return Lex (LEX_NULL, 0);
		} 
		else if ( c == '$' ) {
			buffer->clear ();
			buffer->add (c);
			CS = IDENT;
			return Lex (LEX_NULL, 0);
		}
		else if ( c == '=' ) {
			buffer->clear ();
			buffer->add (c);
			CS = ASSIGN;
			return Lex (LEX_NULL, 0);
		}
		else if ( c == '"') {
			buffer->clear ();
			CS = STR;
			return Lex (LEX_NULL, 0);
		} 
		else if ( isdelim (c) != 0 ) {
			buffer->clear ();
			buffer->add (c);
			CS = DELIM;
			return Lex (LEX_NULL, 0);
		}
		else {
			perror ("Undefined symbol so can't set state.\n");
		}
		break;

	case NUM:
		if ( isdigit (c) ) {
			digit = digit * 10 + ( c - '0');
			return Lex (LEX_NULL, 0);
		}
		else {
			CS = H;
			feed_symbol (c);
			return Lex (LEX_NUM, digit);
		}
		break;

	case IDENT:
		if ( isalpha (c) || isdigit (c) ) {
			buffer->add (c);
			return Lex (LEX_NULL, 0);
		}
		else {
			i = table_ident.put (buffer->get ());
			CS = H;
			feed_symbol (c);
			return Lex (LEX_ID, i );
		}
		break;

	case KW:
		if ( isalpha (c) || isdigit (c) ) {
			buffer->add (c);
			return Lex (LEX_NULL, 0);
		}
		else {
			if ( (i = look (buffer->get (), table.word)) != 0 ) {
				CS = H;
				feed_symbol (c);
				return Lex (table.lex_word [i], i);
			} 
			else {
				perror ("Not found keyword.\n");
			}
		}
		break;

	case ASSIGN:
		if ( (i = look (buffer->get (), table.delim)) != 0 )  {
			CS = H;
			feed_symbol (c);
			return Lex (table.lex_delim [i], i);
		}		
		break;

	case STR:
		if ( c != '"' ) {
			buffer->add (c);
			return Lex (LEX_NULL, 0);
		}
		else {
			// create strchar.
			feed_symbol (c);
			return Lex (LEX_NULL, 0);
		}
		break;

	case DELIM:
		if ( (i = look (buffer->get (), table.delim)) != 0 ) {
			CS = H;
			feed_symbol (c);
			return Lex (table.lex_delim[i], i);
		}
		else {
			perror ("Not found delim (state DELIM).\n");
		}
		break;

	case NEQ:
		if ( c == '=' ) {
			buffer->add (c);
			i = look (buffer->get (), table.delim);
			CS = H;
			feed_symbol (c);
			return Lex (LEX_NEQ, i);
		}
		else {
			perror ("Error in state NEG.\n");
		}
		break;

	case FN:
		if ( isalpha (c) || isdigit (c) ) {
			buffer->add (c);
			return Lex (LEX_NULL, 0);
		}
		else {
			feed_symbol (c);
			return Lex (LEX_NULL, 0);
			/*
			i = table_fn.put (buffer->get ());
			CS = H;
			return Lex (LEX_FN, i );
			*/
		}
		break;
	}
	
	CS = H;	
	printf ( "Error switch.c[%c]. Str number[%d]. Buf[%s]\n", 
			c, count_str + 1 , buffer->get ()
		);
	return Lex (LEX_NULL, 0);
}
