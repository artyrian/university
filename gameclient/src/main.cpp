#include <stdio.h>
#include "lexer/parser.hpp"

int main (int argc, char ** argv)
{
	if ( argc > 1 ) {
		Parser p (argv[1]);

		p.analyze (); 
	}
	else {
		perror ("Not enough parameters.\n");
	}

	return 0;
}
