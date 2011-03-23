#include "lexer.hpp"
#include "exeption.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


Lex:: Lex (int k = 0, type_of_lex t = LEX_NULL, int v = 0)
	: str_n (k), t_lex (t), v_lex (v), 
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

int Lex:: get_str_n ()
{
	return str_n;
}

void Lex:: print () const
{
	printf ("(%d,%d)", t_lex, v_lex);

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

	return 0;
}


Scanner:: Scanner ()
	: table ()
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
	const char smb [] = "+-*/%<>=&|![]();,@";
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
	switch ( CS ) {
	case H:
		return H ();
	case NUM:
		return NUM ();
	case IDENT:
		return IDENT ();
	case KW:
		return KW ();
	case ASSIGN:
		return ASSIGN ();
	case STR:
		return STR ();
	case DELIM:
		return DELIM ();
	case NEQ:
		return NEQ ();
	case FN:
		return FN ();
	case COMMENT:
		return COMMENT ();
	default:
		CS = H;	
		printf ( "c[%c]. Buf[%s]\n", c, buffer->get ());
		throw LexExeption ("Error in switch.", Lex (count_str));
	}
}



Lex Scanner:: H ()
{
	if ( isspace (c) ) {
		if ( c == '\n' ) {
			count_str++;
		}
		return Lex ();
	}
	else if ( isdigit (c) ) {
		digit = c - '0';
		CS = NUM;
		return Lex ();
	}
	else if ( isalpha (c) ) {
		buffer->clear ();
		buffer->add (c);
		CS = KW;
		return Lex ();
	}
	else if ( c == '!' ) {
		buffer->clear ();
		buffer->add (c);
		CS = NEQ;
		return Lex ();
	}
	else if ( c == '?' ) {
		buffer->clear ();
		buffer->add (c);
		CS = FN;
		return Lex ();
	} 
	else if ( c == '$' ) {
		buffer->clear ();
		buffer->add (c);
		CS = IDENT;
		return Lex ();
	}
	else if ( c == '=' ) {
		buffer->clear ();
		buffer->add (c);
		CS = ASSIGN;
		return Lex ();
	}
	else if ( c == '"') {
		buffer->clear ();
		CS = STR;
		return Lex ();
	} 
	else if ( isdelim (c) != 0 ) {
		buffer->clear ();
		buffer->add (c);
		CS = DELIM;
		return Lex ();
	}
	else if ( c == '\'' ) {
		buffer->clear ();
		CS = COMMENT;
		return Lex ();
	}
	else {
		throw LexExeption ( "Undefined symbol. Can't set state. ", Lex (count_str));
	}
}

Lex Scanner:: NUM ()
{
	if ( isdigit (c) ) {
		digit = digit * 10 + ( c - '0');
		return Lex ();
	}
	else {
		CS = H;
		feed_symbol (c);
		return Lex (count_str, LEX_NUM, digit);
	}
}

Lex Scanner:: IDENT ()
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		return Lex ();
	}
	else if ( c == '[' ) {
		int i = table.array.put (buffer->get ());
		CS = H;
		feed_symbol (c);
		return Lex (count_str, LEX_ARRAY, i);
	} 
	else {
		int i = table.ident.put (buffer->get ());
		CS = H;
		feed_symbol (c);
		return Lex (count_str, LEX_ID, i);
	}
}

Lex Scanner:: KW ()
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		return Lex ();
	}
	else {
		int i;
		if ( (i = look (buffer->get (), table.word)) != 0 ) {
			CS = H;
			feed_symbol (c);
			return Lex (count_str, table.lex_word [i], i);
		} 
		else {
			perror ("Not found keyword.\n");
			throw LexExeption ("Not found keyword", Lex (count_str));
		}
	}
}

Lex Scanner:: ASSIGN ()
{
	int i;
	if ( (i = look (buffer->get (), table.delim)) != 0 )  {
		CS = H;
		feed_symbol (c);
		return Lex (count_str, table.lex_delim [i], i);
	}		
}

Lex Scanner:: STR ()
{
	if ( c != '"' ) {
		buffer->add (c);
		return Lex ();
	}
	else {
		int i = table.string.put (buffer->get ());
		CS = H;
		return Lex (count_str, LEX_STR, i);
	}
}

Lex Scanner:: DELIM ()
{
	int i;
	if ( (i = look (buffer->get (), table.delim)) != 0 ) {
		CS = H;
		feed_symbol (c);
		return Lex (count_str, table.lex_delim [i], i);
	}
	else {
		throw LexExeption ( "Not found delim (state FDELIM).", Lex (count_str));
	}
}

Lex Scanner:: NEQ ()
{
	if ( c == '=' ) {
		buffer->add (c);
		int i = look (buffer->get (), table.delim);
		CS = H;
		feed_symbol (c);
		return Lex (count_str, LEX_NEQ, i);
	}
	else {
		throw LexExeption ("Error in state NEG.", Lex (count_str));
	}
}

Lex Scanner:: FN ()
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		return Lex ();
	}
	else {
		int i;
		if ( (i = look (buffer->get (), table.function)) != 0 ) {
			CS = H;
			feed_symbol (c);
			return Lex (count_str, table.lex_function [i], i);
		}
		else {
			perror ("Not found function.\n");
			throw LexExeption ("Not found function.", Lex (count_str));
		}
	}
}

Lex Scanner:: COMMENT ()
{
	if ( c == '\'' ) {
		CS = H;
	}

	return Lex ();
}


