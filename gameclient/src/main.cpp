#include <stdio.h>
#include "lexer/lexlist.hpp"

int main (int argc, char ** argv)
{
	if ( argc > 1 ) {
		LexList ll (argv[1]);

		ll.analyze (); 

		ll.print ();
	}
	else {
		perror ("Not enough parameters.\n");
	}

	return 0;
}
