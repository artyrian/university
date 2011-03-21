#include <stdio.h>
#include "lexer/lexer.hpp"
#include <stdlib.h>


int main (int argc, char ** argv)
{
	Scanner la;
	FILE * fp;
	
	if ( (fp = fopen (argv[1], "r")) == 0 ) {
		perror ("Couldn'n open file.\n");
		exit (1);
	}

	int c = 0;
	Lex lex = Lex (LEX_NULL, 0);

	while ( true ) {
		if ( (c = fgetc (fp)) == -1 ) {
			break;
		}

		lex = la.feed_symbol (c);

		if ( lex.get_type () != 0 ) {
			lex.print ();
			lex = Lex (LEX_NULL, 0);	
		}
		
	}

	lex = la.feed_symbol (' ');
	if ( lex.get_type () != 0 ) {
		lex.print ();
		lex = Lex (LEX_NULL, 0);	
	}

	fclose (fp);

	return 0;
}
