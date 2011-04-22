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
	char ** ip, int port_int, char ** nick, char ** mode, int num_int, 
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
				printf ("option %c with value '%s'\n", 
					c, optarg
				);
				(*ip) = optarg;
				break;
			case 'p' : 
				printf ("option %c with value '%s'\n", 
					c, optarg
				);
				port = optarg;
				break;
			case 'n' :
				printf ("option %c with value '%s'\n", 
					c, optarg
				);
				(*nick) = optarg;
				break;
			case 'm' :
				printf ("option %c with value '%s'\n", 
					c, optarg
				);
				(*mode) = optarg;
				break;

			case 'c' :
				printf ("option %c with value '%s'\n", 
					c, optarg
				);
				num = optarg;
				break;
			case 's' :
				printf ("option %c with value '%s'\n", 
					c, optarg
				);
				(*script) = optarg;
				break;
			default :
				printf ("?? getopt returned character code %c ??\n",
					c
				);
		} // end switch;
	} // end while 


	if ( optind >= argc ) {
		printf ("Expected argument after option.\n");
	}

	check_options (*ip, port, *nick, *mode, num, *script);

	port_int = atoi (port);
	num_int = atoi (num);
} // Parse paramets;



void login (Game & g, char * nick, char * mode, int num)
{
	g.setnick (nick);

	if ( strcpy (mode, "create") ) {
		g.create ();
		g.waitplayers (num);
	} 
	if ( strcpy (mode, "join") ) {
		g.joinroom (num);
		g.waitstart ();
	}
	else {
		printf ("Error in check mode.");	
		exit (EXIT_FAILURE);
	}
}


void play (Game &g)
{
	g.startinfo ();

	while ( g.checkactive (g.my_id ()) != 0 && 
		g.active_players () != 1 )	
	{
		g.readqueue ();
		g.getinfo ();

		g.market ();

		g.buy (2, -1); 
		g.sell (2, -1);

		g.prod (2);

		g.turn ();
		g.waitendturn ();

		g.getinfo ();		// ATT
	}

	g.quit ();
}

/* 
int main(int argc, char **argv)
{	
	char *ip, *nick;
	int port, room, maxpl;

	Game g (ip, port);
	login (g, argc, nick, room, maxpl);
	play (g);

	return 0;
}
*/

int main (int argc, char ** argv)
{
<<<<<<< HEAD
	char * ip = 0;
	int port = 0;
	char * nick = 0;
	char * mode = 0;
	int num = 0;
	char * script = 0;

	parse_arguments (argc, argv, &ip, port, &nick, &mode, num, &script);
	

	try {
		LexList	ll;
		ll.fill (script);
		ll.print ();

		Parser pars (& ll);
		pars.analyze (); 
		pars.rpn.print ();

		// here connect.

		Executer exec ( pars.rpn.get_pointer () );
		exec.executing ();

		// here disconnect.
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

// end play.

	return 0;
}
