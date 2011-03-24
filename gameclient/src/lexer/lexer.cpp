#include "lexer.hpp"
#include "exeption.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

Lex:: Lex ()
	: t_lex (LEX_NULL), v_lex (0), str_n (0)
{
}

Lex:: Lex (int n)
	: t_lex (LEX_NULL), v_lex (0), str_n (n)
{
}

Lex:: Lex (int k, type_of_lex t, int v)
	//: str_n (k)
{
	str_n = k; 
	t_lex = t;
	v_lex = v; 
}

type_of_lex Lex:: get_type () const
{
	return t_lex;
}

int Lex:: get_value () const
{
	return v_lex;
}

int Lex:: get_str_n () const
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
	const char smb [] = "+-*/%<>=&|!{}[]();,@";
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
		return state_H (c);
	case NUM:
		return state_NUM (c);
	case IDENT:
		return state_IDENT (c);
	case KW:
		return state_KW (c);
	case ASSIGN:
		return state_ASSIGN (c);
	case STR:
		return state_STR (c);
	case DELIM:
		return state_DELIM (c);
	case NEQ:
		return state_NEQ (c);
	case FN:
		return state_FN (c);
	case COMMENT:
		return state_COMMENT (c);
	case LABEL:
		return state_LABEL (c);
	default:
		CS = H;	
		printf ( "c[%c]. Buf[%s]\n", c, buffer->get ());
		throw LexExeption ("Error in switch.", Lex (count_str));
	}
}



Lex Scanner:: state_H (int c)
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
	else if ( c == ':' ) {
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
	else if ( c == '@' ) {
		buffer->clear ();
		buffer->add (c);
		CS = LABEL;
		return Lex ();
	}
	else {
		throw LexExeption ( "Undefined symbol. Can't set state. ", Lex (count_str));
	}
}

Lex Scanner:: state_NUM (int c)
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

Lex Scanner:: state_IDENT (int c)
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		return Lex ();
	}
	else if ( c == '[' ) {
		int i = table.array.put (buffer->get ());
		CS = H;
		buffer->add (c);
		feed_symbol (c);
		return Lex (count_str, LEX_ARRAY, i);
	} 
	else {
		int i = table.ident.put (buffer->get ());
		CS = H;
		buffer->add (c);
		feed_symbol (c);
		return Lex (count_str, LEX_ID, i);
	}
}

Lex Scanner:: state_KW (int c)
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

Lex Scanner:: state_ASSIGN (int c)
{
	if ( c == '=' ) { 
		CS = H;
		return Lex (count_str, LEX_ASSIGN, 1);
	}		
	else {
		throw LexExeption ("Error in assign.", Lex (count_str));
/*
		CS = H;
		i = look (buffer->get (), table.delim);
		feed_symbol (c);
		return Lex (count_str, LEX_ASSIGN, i);
*/
	}
}

Lex Scanner:: state_STR (int c)
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

Lex Scanner:: state_DELIM (int c)
{
	int i;
	if ( (i = look (buffer->get (), table.delim)) != 0 ) {
		CS = H;
		feed_symbol (c);
		return Lex (count_str, table.lex_delim [i], i);
	}
	else {
		printf ("%dbuf[%s]\n", i, buffer->get ());
		throw LexExeption ( "Not found delim (state DELIM).", Lex (count_str));
	}
}

Lex Scanner:: state_NEQ (int c)
{
	CS = H;
	feed_symbol (c);
	return Lex (count_str, LEX_NEQ, 0);
}

Lex Scanner:: state_FN (int c)
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

Lex Scanner:: state_COMMENT (int c)
{
	if ( c == '\'' ) {
		CS = H;
	}

	return Lex ();
}

Lex Scanner:: state_LABEL (int c)
{
	if ( isalpha (c) || isdigit (c) || ( c == '_') ) {
		buffer->add (c);
		return Lex ();
	}
	else {
		int i = table.label.put (buffer->get ());
		CS = H;	
		return Lex (count_str, LEX_LABEL, i);
	}

}
