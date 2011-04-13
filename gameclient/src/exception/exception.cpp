#include "exception.hpp"
#include <string.h>
#include <stdio.h>


SymbolException:: SymbolException (const char * s, char ch, int n)
{
	c = ch;
	str = new char [ strlen (s) + 1];
	strcpy (str, s);
	num_str = n;
}


SymbolException:: SymbolException (const SymbolException & se)
{
	c = se.c;
	str = new char [ strlen (se.str) + 1];
	strcpy (str, se.str);
	num_str = se.num_str;
}



SymbolException:: ~SymbolException ()
{
	delete [] str;
}


void SymbolException:: print () const
{
	printf ("Exception: [%s], symbol: %d, str:%d.\n", str, c, num_str);
}


//----------------------------------------------------------


LexException:: LexException (const char * s, const Lex & l)
	: lex (l)
{
	str = new char [ strlen (s) + 1 ];
	strcpy (str, s);
}

LexException:: LexException (const LexException & le)
	: lex (le.lex)
{
	str = new char [ strlen (le.str) + 1 ];
	strcpy (str, le.str);
}

LexException:: ~LexException ()
{
	delete [] str;
}

void LexException:: print () const
{
	printf ("Exception: [%s], Lex:", str);
	lex.print ();
	printf (", str number:%d.\n", lex.strnum);
}


//----------------------------------------------------------

PolizExceptionNotInt:: PolizExceptionNotInt (PolizElem * op)
{
}


PolizExceptionNotLabel:: PolizExceptionNotLabel (PolizElem * op)
{
}

PolizExceptionNotNum:: PolizExceptionNotNum (PolizElem * op)
{
}

PolizExceptionNotAddress:: PolizExceptionNotAddress (PolizElem * op)
{
}
//----------------------------------------------------------

