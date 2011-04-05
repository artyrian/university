#include <stdio.h>
#include "lexer/parser.hpp"
#include "exception/exception.hpp"

int main (int argc, char ** argv)
{
	if ( argc > 1 ) {
		try {
			Parser p (argv[1]);

			p.analyze (); 
		}
		catch (const SymbolException & se) {
			printf ("catch exception.\n");
			se.print ();
			return 1;
		}
		catch (const LexException & le) {
			printf ("catch exception.\n");
			le.print ();
			return 1;
		}
		catch (...) {
			printf ("Unknown exception.\n");
			return 1;
		}

	}
	else {
		perror ("Not enough parameters.\n");
	}

	return 0;
}
