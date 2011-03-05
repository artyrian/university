#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include "game.hpp"


/* */
void ParseArguments(	int n, char **argv, 
			char*& ip, int& port, char*& nick, int& room, int& maxpl)
{
	if ( n >= 5 ) {
		port = room = -1;

		ip = argv[1];

		sscanf (argv[2], "%d", &port);
		if ( port == -1 ) {
			perror ("Syntax error in parse port.\n");
		}
		
		nick = argv[3];

		sscanf (argv[4], "%d", &room);
		if ( room == -1 ) {
			perror ("Syntax error in parse room.\n");
		}

		if ( n == 6 ) {
			maxpl = -1;
			sscanf (argv[5], "%d", &maxpl);
			if ( maxpl == -1 ) {
				perror ("Syntax error in parse max players to start.\n");
			}
		}
	}
}



/* */
int main(int argc, char **argv)
{	
	printf("Start program.\n");

	char *ip = new char [16];
	int port;
	char *nick = new char [22];
	int room;
	int maxpl;

	ParseArguments (argc, argv, ip, port, nick, room, maxpl);

	Game g(ip, port);

	g.setnick (nick);

	if ( argc == 6 ) {
		g.create ();
		g.waitplayers (maxpl);
	} else {
		g.joinroom (room);
		g.waitstart ();
	}


	g.getinfo ();

	for (;;) {

		printf ("Here a loop where I send cmds every turn.\n");

		g.market ();
		g.buy (2, -1);
		g.sell (2, -1);
		g.prod (2);

		g.turn ();

		g.waitendturn ();

		g.queue ();
	}

	delete [] ip;
	delete [] nick;

	printf("End program.\n");
	return 0;
}
