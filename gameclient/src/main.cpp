#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include "game.hpp"


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
	do {	
		g.getinfo ();

		g.market ();

		g.buy (2, -1);
		g.checkok ();

		g.sell (2, -1);
		g.checkok ();

		g.prod (2);
		g.checkok ();

		g.turn ();
		g.waitendturn ();
		g.readqueue ();

		printf ("Active players = %d.\n", g._active_players ());
	} while ( g._checkactive (g._my_id ()) != 0 && g._active_players () != 1 );
}

/*
		printf ("==================  Check data: =============\n");
		printf ("my_id:%s.\nmonth:%d.\nplayers:%d.\nact_pl:%d.\nsullpy:%d.\nraw_price:%d.\ndemand:%d.\nprod_price:%d.\nmoney(n):%d.\nraw(n):%d\nprod(n):%d\n.factories(n):%d.\nauto_factories(n):%d.\nmanufactured(n):%d\n.sold(n):%d.\nsold_price(n)%d.\nbought(n)%d.\nbought_price(n)%d.\n", 
		g._my_id (), 
		g._month (), 
		g._players (),
		g._active_players (),
		g._supply (),
		g._raw_price (), 
		g._demand (), 
		g._production_price (), 
		g._money(g._my_id ()), 
		g._raw (g._my_id ()), 
		g._production (g._my_id ()), 
		g._factories (g._my_id ()), 
		g._auto_factories (g._my_id ()), 
		g._manufactured (g._my_id ()), 
		g._result_raw_sold (g._my_id ()), 
		g._result_raw_price (g._my_id ()), 
		g._result_prod_bought (g._my_id ()), 
		g._result_prod_price (g._my_id ()) 
		);
		printf ("=================     E N D     =============\n");
*/



/* */
void ParseArguments(	int n, char **argv, 
			char*& ip, int& port, char*& nick, int& room, int& maxpl)
{
	if ( n >= 5 ) {
		port = room = -1;
		strcpy (ip, argv[1]);

		sscanf (argv[2], "%d", &port);
		if ( port == -1 ) {
			perror ("Syntax error in parse port.\n");
		}
		
		strcpy (nick, argv[3]);

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
	} else {
		perror ("Not enough parametrs.\n");
	}
}



/* */
int main(int argc, char **argv)
{	
	printf("Start program.\n");

	char *ip = new char [strlen (argv[1])];
	int port;
	char *nick = new char [strlen (argv[3])];
	int room;
	int maxpl;

	ParseArguments (argc, argv, ip, port, nick, room, maxpl);

	Game g(ip, port);

	login (g, argc, nick, room, maxpl);

	g.startinfo ();

	play (g);

	printf ("Game is end for you ;-).\n");

	delete [] ip;
	delete [] nick;

	printf("End program.\n");

	return 0;
}
