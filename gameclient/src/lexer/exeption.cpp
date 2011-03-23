#include "exeption.hpp"
#include <string.h>
#include <stdio.h>

LexExeption:: LexExeption (const char * s, const Lex & l, int n)
	: lex (l)
{
	str = new char [ strlen (s) ];
	strcpy (str, s);
	num_str = n;
}

LexExeption:: LexExeption (const LexExeption & le)
	: lex (le.lex)
{
	str = new char [ strlen (le.str) ];
	strcpy (str, le.str);
	num_str = le.num_str;
}

LexExeption:: ~LexExeption ()
{
	delete [] str;
}

void LexExeption:: print () const
{
	printf ("Exeption: [%s], Lex:", str);
	lex.print ();
	printf (", num:%d.\n", num_str);
}
