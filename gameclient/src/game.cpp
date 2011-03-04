#include "game.hpp"

#include <string.h>
#include <stdio.h>


Game::Game (char *ip, int port)
	: ch (ip, port), month (1)
{
	lp = new ListPlayer;
}



int Game::setnick (char *n)
{
	nick = n;

	ch.sendstr (nick);

	return 0;
}


int Game::joinroom (int r)
{
	room = r;
	char str[32];
	
	sprintf (str, ".join %d", room);
	ch.sendstr (str);
	
	return 0;
}


int Game::create () const
{
	char str[16];

	sprintf (str, ".create");
	ch.sendstr (str);		

	return 0;
}


int Game::waitplayers (int maxpl)
{
	char str[16];
	char msg[1024];
	int i = 0;

	do {
		strcpy (msg, ch.getmsg ());
		if ( strncmp (msg, "@+", 2) == 0 ) {
			i++;
		}
		if ( strncmp (msg, "@-", 2) == 0 ) {
			i--;
		}
		printf ("Now join's players are %d.\n", i);
	} while (i != maxpl);

	
	sprintf (str, "start");
	ch.sendstr (str);

	return 0;
}


int Game::waitstart ()
{
	do {
	} while ( strncmp (ch.getmsg (), "& START", 7) != 0 );

	printf ("GAME START!\n");
	
	return 0;
}


int Game::getinfo ()
{
	char msg[80];

	info ();

	do {
		strcpy (msg, ch.getmsg ());
		if ( strncmp (msg, "& INFO", 6) == 0 ) {
			struct Player *pl;
			pl = lp->parse (msg);
			lp->add (pl);
		}
	} while ( strncmp (msg, "& PLAYERS", 9) != 0 );

	printf ("In game %d players.\n", lp->getplayercnt ());

	return 0;
}


int Game::waitendturn ()
{
	do {
		// HERE some analyze.
		// and add to str mb.
	} while ( strncmp (ch.getmsg(), "& ENDTURN", 9) != 0 );

	month++;

	printf ("NEXT TURN. Now will start %dth month.\n", month);
	
	return 0;
}


void Game::market ()
{
	char str[10] = "market";

	ch.sendstr (str);

	char msg[80];
	do {
		strcpy (msg, ch.getmsg ());
	} while ( strncmp (msg, "& MARKET", 8) != 0 );

	char *cmd = new char [8];
	
	mrk.raw_count = mrk.raw_cost = mrk.prod_count = mrk.prod_cost = -1;

	sscanf (msg, "%s%s%d%d%d%d", cmd, cmd, &mrk.raw_count, &mrk.raw_cost, &mrk.prod_count, &mrk.prod_cost); 
	
	if ( 	(mrk.raw_count == -1) || (mrk.raw_cost == -1) ||
		(mrk.prod_count == -1) || (mrk.prod_cost == -1) )
	{ 
		perror ("Syntax error in parse '& MARKET'.\n");
	}

	delete [] cmd;
}

void Game::info () const
{
	char str[10] = "info";

	ch.sendstr (str);

}

void Game::buy (int count, int cost) const
{
	char *str = new char [20];

//	
	cost = mrk.raw_cost;
//

	sprintf (str, "buy %d %d", count, cost);

	ch.sendstr (str);


	delete [] str;
}

void Game::sell (int count, int cost) const
{
	char *str = new char [20];

//	
	cost = mrk.prod_cost;
//

	sprintf (str, "sell %d %d", count, cost);

	ch.sendstr (str);

	delete [] str;
}

void Game::prod (int count) const
{
	char *str = new char [20];
	sprintf (str, "prod %d", count);

	ch.sendstr (str);

	delete [] str;
}


void Game::build () const
{
	char str[10] = "build";

	ch.sendstr (str);

}

void Game::turn () const
{
	char str[10] = "turn";
	ch.sendstr (str);
}
	
Game::~Game ()
{
	delete lp;
}
