#ifndef _GAME_HPP_
#define _GAME_HPP_


#include "cache.hpp"
#include "player.hpp"


class Game {
	struct Market {
		int raw_cost;
		int raw_count;
		int prod_cost;
		int prod_count;
	};

	Market mrk;
	Cache ch;
	ListPlayer *lp;
	char *nick;
	int room;
public:
	Game (char *ip, int port);

	int setnick (char *nick);
	int joinroom (int room);
	int create ();
	int waitplayers (int maxpl);
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

	~Game ();
};

#endif
