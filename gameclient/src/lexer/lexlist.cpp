#include <stdlib.h>
#include "lexlist.hpp"
#include "exeption.hpp"


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

		if ( lex.get_type () != 0 ) {
			add (lex);
		}
	}

	lex = la.feed_symbol (' ');
	if ( lex.get_type () != 0 ) {
		add (lex); 
	}

	snd = first;
}

Lex LexList:: get_lex ()
{
	ListElem * cur;

	if ( (snd != 0) && (snd->lex.get_type () != 0) ) {
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
