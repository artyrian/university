#include <stdio.h>
#include "lexer/parser.hpp"
#include "exeption/exeption.hpp"

int main (int argc, char ** argv)
{
	if ( argc > 1 ) {
		try {
			Parser p (argv[1]);

			p.analyze (); 
		}
		catch (const LexExeption & le) {
			printf ("catch exeption.\n");
			le.print ();
			return 1;
		}
		catch (int i) {
			printf ("Exeption: %d.\n", i);
			return 1;
		}

	}
	else {
		perror ("Not enough parameters.\n");
	}

	return 0;
}
