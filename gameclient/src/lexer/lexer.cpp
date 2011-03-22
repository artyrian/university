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
	const char smb [] = "+-*/%<>=&|!();,@";
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
		else if ( c == '[' ) {
			buffer->clear ();
			buffer->add (c);
			CS = LBRACKET;
			return Lex (LEX_NULL, 0);
		}
		else if ( c == ']' ) {
			buffer->clear ();
			buffer->add (c);
			CS = RBRACKET;
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
		if ( isalpha (c) || isdigit (c) || (c == '_') ) {
			buffer->add (c);
			return Lex (LEX_NULL, 0);
		}
		else if ( c == '[' ) {
			i = table.array.put (buffer->get ());
			CS = H;
			feed_symbol (c);
			return Lex (LEX_ARRAY, i);
		} 
		else {
			i = table.ident.put (buffer->get ());
			CS = H;
			feed_symbol (c);
			return Lex (LEX_ID, i);
		}
		break;

	case KW:
		if ( isalpha (c) || isdigit (c) || (c == '_') ) {
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
			i = table.string.put (buffer->get ());
			CS = H;
			return Lex (LEX_STR, i);
		}
		break;
	
	case DELIM:
		if ( (i = look (buffer->get (), table.delim)) != 0 ) {
			CS = H;
			feed_symbol (c);
			return Lex (table.lex_delim [i], i);
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
		if ( isalpha (c) || isdigit (c) || (c == '_') ) {
			buffer->add (c);
			return Lex (LEX_NULL, 0);
		}
		else {
			if ( (i = look (buffer->get (), table.function)) != 0 ) {
				CS = H;
				feed_symbol (c);
				return Lex (table.lex_function [i], i);
			}
			else {
				perror ("Not found function.\n");
			}
		}
		break;

	case LBRACKET:
		i = look (buffer->get (), table.delim);	
		CS = H;
		feed_symbol (c);
		return Lex (LEX_LBRACKET, i);

		break;

	case RBRACKET:
		i = look (buffer->get (), table.delim);	
		CS = H;
		feed_symbol (c);
		return Lex (LEX_RBRACKET, i);

		break;
	}

	CS = H;	
	printf ( "Error switch.c[%c]. Str number[%d]. Buf[%s]\n", 
			c, count_str + 1 , buffer->get ()
		);
	return Lex (LEX_NULL, 0);
}
