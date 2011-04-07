#include "lexer.hpp"
#include "../exception/exception.hpp"

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
	int c = fgetc (fp);
	
	if ( c != EOF ) {
		return c;
	}
	else {
		return EOF;
	}
}

ReadFrom:: ~ReadFrom ()
{
	fclose (fp);
}


Lex:: Lex (int k, type_of_lex t, int v)
	: strnum (k), type (t), value (v)
{
}


void Lex:: print () const
{
	PrintTable pt;
	printf ("%s=(%d,%d);", pt.lexem [type], type, value);
}




LexList:: ListElem * 
LexList:: create_elem (const Lex & l)
{
	ListElem * t = new ListElem ;
	t->lex = l;
	t->next = 0;

	return t;
}


void LexList:: add_to_list (const Lex & l)
{
	ListElem * cur = first;

	if ( first == 0 ) {
		first = create_elem (l);
	}
	else {
		ListElem * prev;
		while ( cur != 0 ) {
			prev = cur;
			cur = cur->next;
		}
		cur = create_elem (l);	
		prev->next = cur;
	}
}



LexList:: LexList (const char * path)
	: rf (path)
{
	first = 0;
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



void LexList:: save_list ()
{
	Scanner la;
	Lex lex;
	int c;
	bool p = 0;

	while ( true ) {
		if ( (c = rf.get_symbol ()) == EOF ) {
			break;
		}
		
		p = la.feed_symbol (c);

		if ( p == true) {
			add_to_list (la.get_save_lex ());
		}
	}

	p = la.feed_symbol (' ');
	if ( p == true ) {
		add_to_list (la.get_save_lex ()); 
	}

	snd = first;
}


Lex * LexList:: get_lex_from_list ()
{
	ListElem * cur;

	if ( (snd != 0) && (snd->lex.type != 0) ) {
		cur = snd;
		snd = snd->next;
		return &(cur->lex);
	}
	else {
		return 0;
	}
}


void LexList:: print_list ()
{
	ListElem * cur = first;
	
	int cnt = 1;
	printf ("Print LexList:\n");
	while ( cur != 0 ) {
		if ( cnt != cur->lex.strnum ) {
			printf ("\n");
			cnt = cur->lex.strnum;
		}
		cur->lex.print ();	
		cur = cur->next;
	}
	printf ("\nEnd of Lex List.\n");

}




int Scanner:: isdelim (int c)
{
	const char smb [] = "+-*/%<>=&|!{}[]();,";
	int i = 0;

	while ( smb[i] != '\0' ) {
		if ( smb[i++] == c ) {
			return true;
		}
	}
	
	return false;	
}


Lex Scanner:: get_save_lex ()
{
	return save_lex;
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

	return false;	
}


Scanner:: Scanner ()
{
	table = new TableLexem;
	buffer = new Buffer;

	count_str = 1;
	CS = H;
}


bool Scanner:: feed_symbol (int c)
{
	Lex lex;
	save_c = 0;
	bool p;

	p = step(c);
	// now in save_lex ready lexem

	if ( c == '\n' ) {
		count_str++;
	}

	if ( save_c != 0 ) {
		step (save_c);
	}

	return p;
}


Scanner:: ~Scanner ()
{
	delete buffer;
	delete table;
}


bool Scanner:: step (int c)
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
		throw SymbolException ("Error in switch.", c,  count_str);
	}
}


bool Scanner:: state_H (int c)
{
	if ( c == '\n' || isspace (c) ) {
		//
		return false;	
	}
	else if ( isdigit (c) ) {
		digit = c - '0';
		CS = NUM;
		//
		return false;	
	}
	else if ( isalpha (c) ) {
		buffer->clear ();
		buffer->add (c);
		CS = KW;
		//
		return false;
	}
	else if ( c == '!' ) {
		buffer->clear ();
		buffer->add (c);
		CS = NEQ;
		//
		return false;
	}
	else if ( c == '?' ) {
		buffer->clear ();
		buffer->add (c);
		CS = FN;
		//
		return false; 
	} 
	else if ( c == '$' ) {
		buffer->clear ();
		buffer->add (c);
		CS = IDENT;
		//
		return false;
	}
	else if ( c == ':' ) {
		buffer->clear ();
		buffer->add (c);
		CS = ASSIGN;
		//
		return false;
	}
	else if ( c == '"') {
		buffer->clear ();
		CS = STR;
		//
		return false;
	} 
	else if ( isdelim (c) != 0 ) {
		buffer->clear ();
		buffer->add (c);
		CS = DELIM;
		//
		return false;
	}
	else if ( c == '\'' ) {
		buffer->clear ();
		CS = COMMENT;
		return false;
	}
	else if ( c == '@' ) {
		buffer->clear ();
		buffer->add (c);
		CS = LABEL;
		//
		return false; 
	}
	else {
		throw SymbolException ( "Undefined symbol. Can't set state. ", c, count_str);
	}
}


bool Scanner:: state_NUM (int c)
{
	if ( isdigit (c) ) {
		digit = digit * 10 + ( c - '0');
		//
		return false;
	}
	else {
		CS = H;
		save_c = c;
		save_lex = Lex (count_str, LEX_NUM, digit);
		return true;
	}
}


bool Scanner:: state_IDENT (int c)
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		//
		return false;
	}
	else if ( isspace (c) ) {
		return false;
	}
	else if ( c == '[' ) {
		int i = table->array.put (buffer->get ());
		CS = H;
		save_c = c;
		save_lex = Lex (count_str, LEX_ARRAY, i);
		return true;
	} 
	else {
		int i = table->ident.put (buffer->get ());
		CS = H;
		buffer->add (c);
		save_c = c;
		save_lex = Lex (count_str, LEX_ID, i);
		return true;
	}
}


bool Scanner:: state_KW (int c)
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		//
		return false;
	}
	else {
		int i;
		if ( (i = look (buffer->get (), table->word)) != 0 ) {
			CS = H;
			save_c = c;
			save_lex = Lex (count_str, table->lex_word [i], i);
			return true;
		} 
		else if ( (i = look (buffer->get (), table->action)) != 0 ) {
			CS = H;
			save_c = c;
			save_lex = Lex (count_str, table->lex_action [i], i);
			return true;
		} 
		else {
			printf ("Now in buf: [%s]", buffer->get () );
			throw SymbolException ("Not found keyword", c, count_str);
		}
	}
}


bool Scanner:: state_ASSIGN (int c)
{
	if ( c == '=' ) { 
		CS = H;
		save_lex = Lex (count_str, LEX_ASSIGN, 1);
		return true;
	}		
	else {
		throw SymbolException ("Error in assign.", c, count_str);
	}
}


bool Scanner:: state_STR (int c)
{
	if ( c != '"' ) {
		buffer->add (c);
		//
		return false; 
	}
	else {
		int i = table->string.put (buffer->get ());
		CS = H;
		//
		save_lex = Lex (count_str, LEX_STR, i);
		return true;
	}
}


bool Scanner:: state_DELIM (int c)
{
	int i;

	if ( (i = look (buffer->get (), table->delim)) != 0 ) {
		CS = H;
		save_c = c;
		save_lex = Lex (count_str, table->lex_delim [i], i);
		return true;
	}
	else {
		printf ("%dbuf[%s]\n", i, buffer->get ());
		throw SymbolException ( "Not found delim (state DELIM).", c, count_str);
	}
}


bool Scanner:: state_NEQ (int c)
{
	CS = H;
	save_c = c;
	save_lex = Lex (count_str, LEX_NEQ, 0);
	return true;
}


bool Scanner:: state_FN (int c)
{
	if ( isalpha (c) || isdigit (c) || (c == '_') ) {
		buffer->add (c);
		//
		return false;
	}
	else {
		int i;
		if ( (i = look (buffer->get (), table->function)) != 0 ) {
			CS = H;
			save_c = c;

			save_lex = Lex (count_str, table->lex_function [i], i);
			return true;
		}
		else {
			perror ("Not found function.\n");
			throw SymbolException ("Not found function.", c, count_str);
		}
	}
}


bool Scanner:: state_COMMENT (int c)
{
	if ( c == '\'' ) {
		CS = H;
	}
	//
	return false;
}


bool Scanner:: state_LABEL (int c)
{
	if ( isalpha (c) || isdigit (c) || ( c == '_') ) {
		buffer->add (c);
		//
		return false;
	}
	else {
		int i = table->label.put (buffer->get ());
		CS = H;	
		
		save_lex = Lex (count_str, LEX_LABEL, i);
		return true;
	}

}
