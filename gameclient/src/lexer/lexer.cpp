#include "lexer.hpp"
#include "../exeption/exeption.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


ReadFrom:: ReadFrom (const char * str)
{
	if ( (fp = fopen (str, "r")) == 0 ) {
		perror ("Couldn'n open file.\n");
		exit (1);
	}
}


int ReadFrom:: get_symbol ()
{
	return fgetc (fp);
}

ReadFrom:: ~ReadFrom ()
{
	fclose (fp);
}




Lex:: Lex ()
	: type (LEX_NULL), value (0), strnum (0)
{
}

Lex:: Lex (int n)
	: type (LEX_NULL), value (0), strnum (n)
{
}

Lex:: Lex (int k, type_of_lex t, int v)
{
	strnum = k; 
	type = t;
	value = v; 
}

void Lex:: print () const
{
	PrintTable pt;
	printf ("%s=(%d,%d);", pt.lexem [type], type, value);

}




LexList:: ListElem:: ListElem ()
	: lex (0, LEX_NULL, 0)
{
}


LexList:: LexList (const char * path)
	: rf (path)
{
	first = 0;
}


LexList:: ListElem * 
LexList:: create (const Lex & l)
{
	ListElem * t = new ListElem ;
	t->lex = l;
	t->next = 0;

	return t;
}


void LexList:: add (const Lex & l)
{
	ListElem * cur = first;

	if ( first == 0 ) {
		first = create (l);
	}
	else {
		ListElem * prev;
		while ( cur != 0 ) {
			prev = cur;
			cur = cur->next;
		}
		cur = create (l);	
		prev->next = cur;
	}
}

void LexList:: save ()
{
	Scanner la;
	Lex lex (0, LEX_NULL, 0);
	int c;

	while ( true ) {
		if ( (c = rf.get_symbol ()) == -1 ) {
			break;
		}
		
		lex = la.feed_symbol (c);

		if ( lex.type != 0 ) {
			add (lex);
		}
	}

	lex = la.feed_symbol (' ');
	if ( lex.type != 0 ) {
		add (lex); 
	}

	snd = first;
}


Lex LexList:: get_lex_from_list ()
{
	ListElem * cur;

	if ( (snd != 0) && (snd->lex.type != 0) ) {
		cur = snd;
		snd = snd->next;
		return cur->lex;
	}
	else {
		return Lex (0, LEX_NULL, 0);
	}
}


void LexList:: print ()
{
	ListElem * cur = first;
	
	printf ("Print LexList:\n");
	while ( cur != 0 ) {
		cur->lex.print ();	
		cur = cur->next;
	}
	printf ("\nEnd of Lex List.\n");

}


LexList:: ~LexList ()
{
	ListElem * cur = first;

	while ( cur != 0 ) {
		first = cur;
		cur = cur->next;	
		delete first;
	}
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



int Scanner::look (const char * buf, const char ** list)
{
	int i = 0;

	while ( list [i] != 0 ) {
		if ( strcmp (buf, list [i]) == 0 ) {
			printf ("find №i=%d.\n", i);
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

Lex Scanner:: feed_symbol (int c)
{
	Lex lex;
	save_c = 0;

	lex = step(c);

	if ( c == '\n' ) {
		count_str++;
	}

	if ( save_c != 0 ) {
		step (save_c);
	}

	return lex;
}


Scanner:: ~Scanner ()
{
	delete buffer;
}


Lex Scanner:: step (int c)
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
	if ( c == '\n' || isspace (c) ) {
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
		save_c = c;
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
		save_c = c;
		return Lex (count_str, LEX_ARRAY, i);
	} 
	else {
		int i = table.ident.put (buffer->get ());
		CS = H;
		buffer->add (c);
		save_c = c;
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
			save_c = c;
			return Lex (count_str, table.lex_word [i], i);
		} 
		else {
			throw LexExeption ("Not found keyword", Lex (count_str));
		}
	}
}

Lex Scanner:: state_ASSIGN (int c)
{
	if ( c == '=' ) { 
		CS = H;
		//save = c;
		return Lex (count_str, LEX_ASSIGN, 1);
	}		
	else {
		throw LexExeption ("Error in assign.", Lex (count_str));
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
		save_c = c;
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
	save_c = c;
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
			printf ("in buf[%s], i=%d.\n", buffer->get (), i);
			CS = H;
			save_c = c;
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

