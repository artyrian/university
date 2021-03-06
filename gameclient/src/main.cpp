#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "lexer/parser.hpp"
#include "lexer/lexer.hpp"
#include "exec/executer.hpp"
#include "exception/exception.hpp"
#include "robot/game.hpp"


void check_options ( char * ip, char * port, 
	char * nick, char * mode, char * script);

void parse_arguments (int argc, char ** argv,
	char ** ip, int port, char ** nick, char ** mode, int num, 
	char ** script);

void login (Game &g, char * nick, char * mode, int num);
void play (Game &g);



void check_options (char * ip, char * port, 
	char * nick, char * mode, char * num, char * script)
{
	if ( ip == 0 ) {
		printf ( "Ip not enabled.\n" );
		exit (EXIT_FAILURE);
	}
	if ( port == 0 || atoi (port) == 0) {
		printf ( "Port not enabled or error in number.\n" );
		exit (EXIT_FAILURE);
	}
	if ( nick == 0 ) {
		printf ( "Nick not enabled.\n" );
		exit (EXIT_FAILURE);
	}
	if ( mode == 0 ) {
		printf ( "Mode to sctipt not enabled.\n" );
		exit (EXIT_FAILURE);
	}
	if ( num == 0 ||  atoi (num) == 0 ) {
		printf ( "Count [-c] not enabled or error in number.\n" );
		exit (EXIT_FAILURE);
	}
	if ( script == 0 ) {
		printf ( "Path to sctipt not enabled.\n" );
		exit (EXIT_FAILURE);
	}
}

// Parse arguments
void parse_arguments (int argc, char ** argv,
	char ** ip, int * port_int, char ** nick, char ** mode, int * num_int, 
	char ** script)
{
	int c;
	char * port;
	char * num;

	while ( true ) {
		c = getopt (argc, argv, "i:p:n:m:c:s:");

		if ( c == -1 ) {
			break;
		}

		switch (c) {
			case 'i' :
				(*ip) = optarg;
				break;
			case 'p' : 
				port = optarg;
				break;
			case 'n' :
				(*nick) = optarg;
				break;
			case 'm' :
				(*mode) = optarg;
				break;

			case 'c' :
				num = optarg;
				break;
			case 's' :
				(*script) = optarg;
				break;
			default :
				printf ("?? getopt returned character code %c ??\n",
					c
				);
		} // end switch;
	} // end while 


	check_options (*ip, port, *nick, *mode, num, *script);

	(*port_int) = atoi (port);
	(*num_int) = atoi (num);
} // Parse paramets;



int main (int argc, char ** argv)
{
	char * ip = 0;
	int port = 0;
	char * nick = 0;
	char * mode = 0;
	int num = 0;
	char * script = 0;

	parse_arguments (argc, argv, &ip, &port, &nick, &mode, &num, &script);

	printf ("Parsed paramets: ip = %s, port = %d, nick = %s, mode = %s, num = %d, script = %s.\n",
		ip, port, nick, mode, num, script
	);


	try {
		LexList	ll;
		ll.fill (script);

		Game g (ip, port);

		Parser pars (& ll, & g);
		pars.analyze (); 

		g.login (nick, mode, num);

		// ready to play

		Executer exec ( pars.rpn.get_pointer () );
		exec.executing ();

		/* Nee still execute while ...
			while ( g.checkactive (g.my_id ()) != 0 && 
				g.active_players () != 1 )	
			{
		*/

		g.quit ();
	}
	catch (const SymbolException & se) {
		printf ("catch exception.\n");
		se.print ();
		exit (EXIT_FAILURE);
	}
	catch (const LexException & le) {
		printf ("catch exception.\n");
		le.print ();
		exit (EXIT_FAILURE);
	}
	catch (const char * str) {
		printf ("catch exception: %s.\n", str);
		exit (EXIT_FAILURE);
	}
	catch (...) {
		printf ("Unknown exception.\n");
		exit (EXIT_FAILURE);
	}

	return 0;
}
