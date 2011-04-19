#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer/parser.hpp"
#include "lexer/lexer.hpp"
#include "exec/executer.hpp"
#include "exception/exception.hpp"

#include "robot/game.hpp"

void login (Game &g, int argc, char *nick, int room, int maxpl);
void play (Game &g);
void ParseArguments (	int argc, char **argv, 
			char*& ip, int& port, 
			char*& nick, int& room, int& maxpl
			);



void login (Game &g, int argc, char *nick, int room, int maxpl)
{
	g.setnick (nick);

	if ( argc == 6 ) {
		g.create ();
		g.waitplayers (maxpl);
	} else {
		g.joinroom (room);
		g.waitstart ();
	}
}


void play (Game &g)
{
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
}



void ParseArguments (	int argc, char **argv, 
			char*& ip, int& port, 
			char*& nick, int& room, int& maxpl
			)
{
	if ( argc >= 5 ) {
		ip = argv[1];
		nick = argv[3];

		port = room = -1;
		strcpy (ip, argv[1]);

		sscanf (argv[2], "%d", &port);
		if ( port == -1 ) {
			perror ("Syntax error in parse port.\n");
			exit (1);
		}
		
		strcpy (nick, argv[3]);

		sscanf (argv[4], "%d", &room);
		if ( room == -1 ) {
			perror ("Syntax error in parse room.\n");
			exit (1);
		}

		if ( argc == 6 ) {
			maxpl = -1;
			sscanf (argv[5], "%d", &maxpl);
			if ( maxpl == -1 ) {
				perror ("Syntax error in parse max players to start.\n");
				exit (1);
			}
		}
	} else {
		perror ("Not enough parametrs.\n");
		exit (1);
	}
}

/* 
int main(int argc, char **argv)
{	
	char *ip, *nick;
	int port, room, maxpl;

	ParseArguments (argc, argv, ip, port, nick, room, maxpl);

	Game g (ip, port);

	login (g, argc, nick, room, maxpl);

	g.startinfo ();

	play (g);

	g.quit ();

	return 0;
}
*/

int main (int argc, char ** argv)
{
	if ( argc > 1 ) {
		try {
			LexList	ll;
			ll.fill (argv [1]);
			ll.print ();

			Parser pars (& ll);
			pars.analyze (); 
			pars.rpn.print ();

			Executer exec ( pars.rpn.get_pointer () );
			exec.executing ();
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
		catch (const char * str) {
			printf ("catch exception: %s.\n", str);
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
