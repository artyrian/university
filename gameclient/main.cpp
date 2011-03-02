#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include "cache.hpp"

struct Market {
	int row_cost;
	int row_count;
	int prod_cost;
	int prod_count;
};

/* */
class Game {
	Cache ch;
	Market mrk;
	char *nick;
	int room;
public:
	Game (char *ip, int port);
	int setnick (char *nick);
	int joinroom (int room);
	int waitstart ();
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


int Game::waitendturn ()
{
	char msg[80];
	do {
		strcpy (msg, ch.getmsg ());
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

	mrk.row_count = atoi (prs[0]);
	mrk.row_cost = atoi (prs[1]);
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
