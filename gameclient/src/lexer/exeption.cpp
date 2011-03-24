#include "exeption.hpp"
#include <string.h>
#include <stdio.h>

LexExeption:: LexExeption (const char * s, const Lex & l)
	: lex (l)
{
	str = new char [ strlen (s) + 1 ];
	strcpy (str, s);
}

LexExeption:: LexExeption (const LexExeption & le)
	: lex (le.lex)
{
	str = new char [ strlen (le.str) + 1 ];
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
	printf (", str number:%d.\n", lex.get_str_n ());
}
