#include "game.hpp"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int prod_creating_cost = 2000;


Game:: Game (char *ip, int port)
	: q(ip, port), month (1)
{
	lp = new ListPlayer;
}

void Game:: bought (char *nick, int amount, int price)
{
	Player * pl = lp->find (nick);

	pl->bought = amount;
	pl->bought_price = price;
	
	pl->money -= amount * price;
	pl->raw += amount;
}

void Game:: sold (char *nick, int amount, int price)
{
	Player *pl = lp->find (nick);

	pl->sold = amount;
	pl->sold_price = price;

	pl->money += amount * price;
	pl->prod -= amount;
}

void Game:: bankrupt (char *nick)
{
	Player *pl = lp->find (nick);
	pl->factive = 0;
}

int Game:: checkactive (char *nick)
{
	Player *pl = lp->find (nick);
	return pl->factive;
}


void Game:: login (char * nick, char * mode, int num)
{
	setnick (nick);

	if ( strcmp (mode, "create") == 0) {
		create ();
		waitplayers (num);
	} 
	else if ( strcmp (mode, "join") == 0) {
		joinroom (num);
		waitstart ();
	}
	else {
		printf ("Error in check mode. [ %s ].\n",
			mode
		);	
		exit (EXIT_FAILURE);
	}

	startinfo ();
	market ();
}

void Game:: setnick (char *n)
{
	nick = n;

	q.sendstr (nick);

	char msg[100];
	strcpy (msg, q.gettype ('%'));
	if ( msg[1] == '-' ) {
		perror ("Bad nick.\n");
		exit (1);
	}
}


void Game:: joinroom (int r)
{
	room = r;
	char str[32];
	
	sprintf (str, ".join %d", room);
	q.sendstr (str);
	
	char msg[100];
	strcpy (msg, q.gettype ());
	if ( msg[0] == '%' ) {
		perror ("Bad room.\n");
		exit (1);
	}
}


void Game:: create () const
{
	char str[16];

	sprintf (str, ".create");
	q.sendstr (str);		
}


void Game:: waitplayers (int maxpl)
{
	char str[16];
	char msg[1024];
	int i = 0;

	do {
		strcpy (msg, q.gettype ('@'));
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
}


void Game:: waitstart ()
{
	do {
	} while ( strncmp (q.gettype ('&'), "& START", 7) != 0 );

	printf ("Game is starting.\n");
}


void Game:: quit ()
{
	char str [10];
	sprintf (str, ".quit");
	q.sendstr (str);
}


void Game:: startinfo ()
{
	char msg[80];

	info ();

	do {
		strcpy (msg, q.gettype('&'));

		if ( strncmp (msg, "& INFO", 6) == 0 ) {
			struct Player * player = lp->parse (msg);
			lp->add (player);
			lp->enable (player);
		}
	} while ( strncmp (msg, "& PLAYERS", 9) != 0 );
	
	char trash[10];
	int watchers;

	inf.players = inf.active_players = watchers = -1;

	sscanf (msg, "%s%s%d%s%d", trash, trash, 
		&inf.active_players, trash, &watchers);
	if ( inf.active_players == -1 || watchers == -1 ) {
		perror ("Syntax error in info about count of players.\n");
	}

	inf.players = watchers + inf.active_players;
}


void Game:: getinfo ()
{
	char msg [80];

	info ();

	lp->disable_all ();
	do {
		strcpy (msg, q.gettype('&'));

		if ( strncmp (msg, "& INFO", 6) == 0 ) {
			struct Player * parsed_pl = lp->parse (msg);
			struct Player * player = lp->find (parsed_pl->nick); 
			lp->enable (player);
			player->copy (parsed_pl);
			delete parsed_pl;
		}
	} while ( strncmp (msg, "& PLAYERS", 9) != 0 );
	
	char trash [10];
	int watchers;

	inf.players = inf.active_players = watchers = -1;

	sscanf (msg, "%s%s%d%s%d", trash, trash, 
		&inf.active_players, trash, &watchers);
	if ( inf.active_players == -1 || watchers == -1 ) {
		perror ("Syntax error in info about count of players.\n");
	}

	inf.players = watchers + inf.active_players;
}


void Game:: waitendturn ()
{
	char msg [1024];

	char trash [10];
	char nick [20];
	int amount = -1;
	int price = -1;

	do {
		strcpy (msg, q.gettype ('&'));

		if ( strncmp (msg, "& BOUGHT", 8) == 0) {
			sscanf ( msg, "%s%s%s%d%d", trash, trash, 
					nick, &amount, &price);
			if ( amount == - 1 || price == -1 ) {
				perror ("Syntax error in BOUGHT.\n");
			}
			bought (nick, amount, price);
		} else if ( strncmp (msg, "& SOLD", 6) == 0) {
			sscanf ( msg, "%s%s%s%d%d", trash, trash, 
					nick, &amount, &price);
			if ( amount == - 1 || price == -1 ) {
				perror ("Syntax error in SOLD.\n");
			}
			sold (nick, amount, price);
		} else if ( strncmp (msg, "& BANKRUPT", 10) == 0) {
			sscanf (msg, "%s%s%s", trash, trash, nick);
			bankrupt (nick);
		}
	} while ( strncmp (msg, "& ENDTURN", 9) != 0 );

	lp->month_expense ();

	++ month;

//	printf ("NEXT TURN. Now will start %dth month.\n", month);
}


void Game:: readqueue ()
{
	char msgq[1024];

	while ( q.getcount () != 0 ) {
//		printf ("Now in queue %d message.\n", q.getcount ());

		strcpy (msgq, q.getmsgq ());	

		char str1[30] = "", str2[30] = "", str3[30] = "";

		sscanf (msgq, "%s%s%s", str1, str2, str3);
		if ( 	strcpy (str1, "@-") == 0 &&
			strcpy (str2, "LEFT") == 0 &&
			strcpy (str3, "") == 0)
		{
			Player *pl = lp->find (str3);
			pl->factive = 0;
		}
	}
}


void Game:: market ()
{
	char str[10] = "market";

	q.sendstr (str);

	char msg[80];
	do {
		strcpy (msg, q.gettype ('&'));
		if ( strncmp (msg, "&-", 2) == 0 ) {
			perror ("Clearly, end of game.\n");
			break;
		}
	} while ( strncmp (msg, "& MARKET", 8) != 0 );

	char trash [10];
	
	mrk.raw_count = mrk.raw_cost = mrk.prod_count = mrk.prod_cost = -1;

	sscanf ( msg, "%s%s%d%d%d%d", trash, trash, 
			&mrk.raw_count, &mrk.raw_cost,
			&mrk.prod_count, &mrk.prod_cost
		); 
	
	if ( 	(mrk.raw_count == -1) || (mrk.raw_cost == -1) ||
		(mrk.prod_count == -1) || (mrk.prod_cost == -1) )
	{ 
		perror ("Syntax error in parse '& MARKET'.\n");
	}
}

void Game:: info () const
{
	char str [10] = "info";

	q.sendstr (str);
}

void Game:: buy (int count, int cost)
{
	char str [80];

	sprintf (str, "buy %d %d", count, cost);

	q.sendstr (str);

	checkok ();
}


void Game:: sell (int count, int cost)
{
	char str [80];

	sprintf (str, "sell %d %d", count, cost);

	q.sendstr (str);

	checkok ();
}

void Game:: prod (int count)
{
	char str[20];

	sprintf (str, "prod %d", count);

	q.sendstr (str);
	
	checkok ();

	Player * player = lp->find (nick);
	player->money -= count * prod_creating_cost;
	player->raw -= count;
	player->prod += count;
}


void Game:: build () const
{
	char str [10] = "build";
	q.sendstr (str);
}

void Game:: turn ()
{
	char str [10] = "turn";

	q.sendstr (str);

	waitendturn ();


	// new month
	readqueue ();
	getinfo ();
	if ( active_players () >= 2 ) {
		market ();
	}
}


void Game:: checkok ()
{
	char str[80];

	do {
		strcpy (str, q.gettype ('&'));
		if ( strncpy (str, "&-", 2) == 0 ) {
			perror ("Error in reading OK.\n");	
			break;
		}
	} while ( strncpy (str, "& OK", 4) == 0 );
}


char * Game:: my_id () const
{
	return nick;
}

int Game:: cur_month () const
{
	return month;
}

int Game:: players () const
{
	return inf.players;
}

int Game:: active_players () const 
{
	return inf.active_players;
}

int Game:: supply () const
{
	return mrk.raw_count;
}

int Game:: raw_price () const
{
	return mrk.raw_cost;
}

int Game:: demand () const 
{
	return mrk.prod_count;
}

int Game:: production_price () const
{
	return mrk.prod_cost;
}

int Game:: active (const char * nick) const
{
	Player *pl = lp->find (nick);
	return pl->factive;
}

int Game:: money (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->money;
}

int Game:: raw (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->raw;
}

int Game:: production (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->prod;	
}

int Game:: factories (const char *nick) const 
{
	Player *pl = lp->find (nick);
	return pl->plants;
}

int Game:: auto_factories (const char *nick) const 
{
	Player *pl = lp->find (nick);
	return pl->autoplants;
}

int Game:: manufactured (const char *nick) const
{
	Player *pl = lp->find (nick);
	int manufactured = pl->prod - pl->last_prod + pl->sold;
	return manufactured;
}

int Game:: result_raw_sold (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->bought;
}

int Game:: result_raw_price (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->bought_price;
}

int Game:: result_prod_bought (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->sold;
}

int Game:: result_prod_price (const char *nick) const
{
	Player *pl = lp->find (nick);
	return pl->sold_price;
}


char * Game:: convert_to_char (int player_num) const
{
	Player * pl = lp->find (player_num);
	return pl->nick;
}

int Game:: convert_to_int (const char * str) const
{
	Player * pl = lp->find (str);
	return pl->number;
}
	
Game:: ~Game ()
{
	delete lp;
}
