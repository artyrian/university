#include "game.hpp"

#include <string.h>
#include <stdio.h>


Game::Game (char *ip, int port)
	: q(ip, port), month (1)
{
	lp = new ListPlayer;
}

void Game::bought (char *nick, int bought, int price)
{
	Player *pl = lp->find (nick);
	pl->bought = bought;
	pl->bought_price = price;
}

void Game::sold (char *nick, int sold, int price)
{
	Player *pl = lp->find (nick);
	pl->sold = sold;
	pl->sold_price = price;
}

void Game::bankrupt (char *nick)
{
	lp->remove (nick);
}


int Game::setnick (char *n)
{
	nick = n;

	q.sendstr (nick);

	return 0;
}


int Game::joinroom (int r)
{
	room = r;
	char str[32];
	
	sprintf (str, ".join %d", room);
	q.sendstr (str);
	
	return 0;
}


int Game::create () const
{
	char str[16];

	sprintf (str, ".create");
	q.sendstr (str);		

	return 0;
}


int Game::waitplayers (int maxpl)
{
	char str[16];
	char msg[1024];
	int i = 0;

	do {
		strcpy (msg, q.gettype ('&'));
		if ( strncmp (msg, "@+", 2) == 0 ) {
			i++;
		}
		if ( strncmp (msg, "@-", 2) == 0 ) {
			i--;
		}
		printf ("Now join's players are %d.\n", i);
	} while (i != maxpl);

	
	sprintf (str, "start");
	q.sendstr (str);

	return 0;
}


int Game::waitstart ()
{
	do {
	} while ( strncmp (q.gettype ('&'), "& START", 7) != 0 );

	printf ("GAME START!\n");
	
	return 0;
}


int Game::getinfo ()
{
	char msg[80];

	info ();

	do {
		strcpy (msg, q.gettype('&'));
		if ( strncmp (msg, "& INFO", 6) == 0 ) {
			struct Player *parsed_pl, *pl;
			parsed_pl = lp->parse (msg);
			pl = lp->find (parsed_pl->nick); 
			if ( pl == 0 ) {
				lp->add (parsed_pl);
			} else {
				pl->raw = parsed_pl->raw;
				pl->prod = parsed_pl->prod;
				pl->money = parsed_pl->money;
				pl->plants = parsed_pl->plants;
				pl->autoplants = parsed_pl->autoplants;
				pl->last_prod = pl->prod - pl->last_prod;
				delete parsed_pl;
			}
		}
	} while ( strncmp (msg, "& PLAYERS", 9) != 0 );
	
	char trash[10];
	inf.players = inf.active_players = -1;
	sscanf (msg, "%s%s%d%s%d", trash, trash, 
		&inf.players, trash, &inf.active_players);
	if ( inf.players == -1 || inf.active_players == -1 ) {
		perror ("Syntax error in info about count of players.\n");
	}

	return 0;
}


int Game::waitendturn ()
{
	char *msg;

	do {
		msg = q.gettype ('&');

		if ( strncmp (msg, "& BOUGHT", 8) == 0) {
			char trash[10];
			char nick[20];
			int amount = -1;
			int price = -1;
			sscanf (msg, "%s%s%s%d%d", trash, trash, nick, &amount, &price);
			if ( amount == - 1 || price == -1 ) {
				perror ("Syntax error in BOUGHT.\n");
			}
			bought (nick, amount, price);
		} else if ( strncmp (msg, "& SOLD", 6) == 0) {
			char trash[10];
			char nick[20];
			int amount = -1;
			int price = -1;
			sscanf (msg, "%s%s%s%d%d", trash, trash, nick, &amount, &price);
			if ( amount == - 1 || price == -1 ) {
				perror ("Syntax error in SOLD.\n");
			}
			sold (nick, amount, price);
		} else if ( strncmp (msg, "& BANKRUPT", 10) == 0) {
			char trash[10];
			char nick[20];
			sscanf (msg, "%s%s%s", trash, trash, nick);
			bankrupt (nick);
		}
		
	} while ( strncmp (msg, "& ENDTURN", 9) != 0 );

	month++;

	printf ("NEXT TURN. Now will start %dth month.\n", month);

	return 0;
}


int Game::queue ()
{
	char *msgq;

	while ( q.getcount () != 0 ) {
		msgq = q.readqueue ();	

		char str1[30] = "", str2[30] = "", str3[30] = "";
		printf ("I read from queue:[%s].\n", msgq);	
		sscanf (msgq, "%s%s%s", str1, str2, str3);
		if ( strcpy (str1, "@-") == 0 && strcpy (str2, "LEFT") ) {
			lp->remove (str3);
		}
	}

	return 0;
}


void Game::market ()
{
	char str[10] = "market";

	q.sendstr (str);

	char msg[80];
	do {
		strcpy (msg, q.gettype ('&'));
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

	q.sendstr (str);

}

void Game::buy (int count, int cost) const
{
	char *str = new char [20];
//	
	cost = mrk.raw_cost;
//
	sprintf (str, "buy %d %d", count, cost);

	q.sendstr (str);
	delete [] str;
}


void Game::sell (int count, int cost) const
{
	char *str = new char [20];

//	
	cost = mrk.prod_cost;
//

	sprintf (str, "sell %d %d", count, cost);

	q.sendstr (str);

	delete [] str;
}

void Game::prod (int count) const
{
	char *str = new char [20];
	sprintf (str, "prod %d", count);

	q.sendstr (str);

	delete [] str;
}


void Game::build () const
{
	char str[10] = "build";

	q.sendstr (str);

}

void Game::turn () const
{
	char str[10] = "turn";
	q.sendstr (str);
}


void Game::checkok ()
{
	char *str;

	do {
		str = q.gettype ('&');
		if ( strncpy (str, "&-", 2) == 0 ) {
			perror ("THROW. Buy wrong.\n");	
			break;
		}
	} while ( strncpy (str, "& OK", 4) == 0 );
}


	
Game::~Game ()
{
	delete lp;
}
