#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include "cache.hpp"

class Player {
	char *nick;
	int raw;
	int prod;
	int money;
	int plants;
	int autoplants;
public:
	Player (char *n, int r, int pr, int m, int pl, int apl)
		: nick(n), raw(r), prod(pr), money(m), plants(pl), autoplants(apl)	
	{
	}
};


class ListPlayer {
	struct ListElem {
		Player *p;
		ListElem *next;
	};

	ListElem *first;
	int cnt;


	ListElem * newplayer (Player *pl) {
		class ListElem *elem = (ListElem*) malloc (sizeof(ListElem));

		elem->p = pl;  
		elem->next = 0;

		return elem;
	}
	
	ListElem * findlast () {
		class ListElem *cur = first;
		class ListElem *last= first;

		while ( cur != 0 ) {
				last = cur;
				cur = cur->next;
		}

		return last;
	}
public: 
	ListPlayer ()
		: first (0), cnt (0)
	{
	}

	void add (Player *pl) {
		ListElem *elem = newplayer (pl);
		ListElem *last = findlast ();

		last->next = elem;
		cnt++;
	}
	void search ();
	void remove ();
	
	int getplayercnt () {
		return cnt;
	}
	
	Player parse (char *str) {
		char *cmd, *nick;
		int raw, prod, money, plant, autoplant;

		raw = prod = money = plant = autoplant = -1;
		
		cmd = (char *) malloc (6);
		nick = (char *) malloc (20);

		sscanf(str, "%s%s%d%d%d%d%d", cmd, nick, &raw, &prod, &money, &plant, &autoplant);
		printf ("I parsed:\ncmd[%s]\nnick[%s]\nraw[%d]\nprod[%d]\nmoney[%d]\nplant[%d]\nautoplant[%d].\n",
			cmd, nick, raw, prod, money, plant, autoplant);
	
		if ( 	(raw != -1) && 
			(prod != -1) && 
			(money != -1) && 
			(plant = -1) && 
			(autoplant != -1) )
		{
			perror ("Syntax error in parse '& INFO'.\n");
			exit (1);
		}

		free (cmd);
		return Player(nick, raw, prod, money, plant, autoplant);
	}
};

/* */
class Game {
	struct Market {
		int raw_cost;
		int raw_count;
		int prod_cost;
		int prod_count;
	} mrk;

	Cache ch;
	ListPlayer *lp;
	char *nick;
	int room;
public:
	Game (char *ip, int port);
	int setnick (char *nick);
	int joinroom (int room);
	int waitstart ();
	int getinfo ();
	int waitendturn ();

	void market ();
	void info ();
	void buy (int count, int cost);
	void sell (int count, int cost);
	void prod (int count);
	void build ();
	void turn ();

};



/* */
Game::Game (char *ip, int port)
	: ch (ip, port)
{
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


int Game::waitstart ()
{
	char msg[80];
	do {
		strcpy (msg, ch.getmsg ());
	} while ( strncmp (msg, "& START", 7) != 0 );

	printf ("GAME START!\n");
	
	return 0;
}


int Game::getinfo ()
{
	char msg[80];
	do {
		strcpy (msg, ch.getmsg ());
		if ( strncpy (msg, "& INFO", 6) == 0 ) {
			struct Player *pl;
			pl = (Player *) malloc (sizeof(Player));
			pl = lp.parse (msg);
			lp.add (pl);
		}
	} while ( strncmp (msg, "& PLAYERS", 7) != 0 );

	return 0;
}


int Game::waitendturn ()
{
	char msg[80];
	do {
		strcpy (msg, ch.getmsg ());
		// HERE some analyze.
	} while ( strncmp (msg, "& ENDTURN", 9) != 0 );

	printf ("NEXT TURN.\n");
	
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

	char prs[4][10];
	int i = 0;
	for (int j = 0; j < 10; j++) {
		prs[0][j] = msg[i+10];
		prs[1][j] = msg[i+20];
		prs[2][j] = msg[i+30];
		prs[3][j] = msg[i+40];
		i++;
	}

	mrk.raw_count = atoi (prs[0]);
	mrk.raw_cost = atoi (prs[1]);
	mrk.prod_count = atoi (prs[2]);
	mrk.prod_cost = atoi (prs[3]);
}

void Game::info ()
{
	char str[10] = "info";

	ch.sendstr (str);

}

void Game::buy (int count, int cost)
{
	char *str = (char *) malloc (20);

//	
	cost = mrk.raw_cost;
//

	sprintf (str, "buy %d %d", count, cost);

	ch.sendstr (str);
}

void Game::sell (int count, int cost)
{
	char *str = (char *) malloc (20);

//	
	cost = mrk.prod_cost;
//

	sprintf (str, "sell %d %d", count, cost);

	ch.sendstr (str);
}

void Game::prod (int count)
{
	char *str = (char *)  malloc (20);
	sprintf (str, "prod %d", count);

	ch.sendstr (str);
}


void Game::build ()
{
	char str[10] = "build";

	ch.sendstr (str);

}

void Game::turn ()
{
	char str[10] = "turn";
	ch.sendstr (str);
}
	


/*
 * ===================   M A I N   ================ *
 */



/* */
void ParseArguments(	int n, char **argv, 
			char*& ip, int& port, char*& nick, int& room)
{
	if ( n != 5 ){
		printf ("WARNING!!! Enabled debug mode.\nStatic parametrs (ip, port, nick, room).\n");
		port = 4774; 
		room = 1; 
		strcpy (ip, "0"); 
		strcpy (nick, "Bot0"); 
	} else { 
		ip = argv[1];
		port = atoi(argv[2]);
		nick = argv[3];
		room = atoi(argv[4]);
	}
}



/* */
int main(int argc, char **argv)
{	
	printf("Start program.\n");
	
	char *ip = (char *) malloc(16);
	int port;
	char *nick = (char *) malloc(22);
	int room;

	ParseArguments (argc, argv, ip, port, nick, room);
	
	Game g(ip, port);
	
	g.setnick (nick);
	g.joinroom (room);
	g.waitstart ();
	g.getinfo ();

	for (;;) {
		printf ("Here a loop where I send cmds every turn.\n");
		
		g.market ();
		g.buy (2, -1);
		g.sell (2, -1);
		g.prod (2);

		g.turn ();

		g.waitendturn ();
	}

	free (ip);
	free (nick);

	printf("End program.\n");
	return 0;
}
