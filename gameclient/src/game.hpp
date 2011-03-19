#ifndef _GAME_HPP_
#define _GAME_HPP_


#include "queuemsg.hpp"
#include "player.hpp"


class Game {
	struct Market {
		int raw_cost;
		int raw_count;
		int prod_cost;
		int prod_count;
	} mrk;

	struct Info {
		int players;
		int active_players;
	} inf;

	QueueMsg q;
	ListPlayer *lp;
	char *nick;
	int room;
	int month;

	void info () const;
	void bought (char *nick, int bought, int price);
	void sold (char *nick, int sold, int price);
	void bankrupt (char *nick);

	void checkok ();
public:
	Game (char *ip, int port);

	void setnick (char *nick);
	void joinroom (int room);
	void create () const;
	void waitplayers (int maxpl);
	void waitstart ();
	void startinfo ();
	void getinfo ();
	void waitendturn ();
	void readqueue ();
	void quit ();

	int checkactive (char *);

	void market ();

	void buy (int count, int cost);
	void sell (int count, int cost);
	void prod (int count);
	void build () const;
	void turn () const;

	char* my_id () const;
	int curmonth () const;
	int players () const;
	int active_players () const;
	int supply () const;
	int raw_price () const;
	int demand () const;
	int production_price () const;
	int money (const char *nick) const;
	int raw (const char *nick) const;
	int production (const char *nick) const;
	int factories (const char *nick) const;
	int auto_factories (const char *nick) const;
	int manufactured (const char *nick) const;
	int result_raw_sold (const char *nick) const;
	int result_raw_price (const char *nick) const;
	int result_prod_bought (const char *nick) const;
	int result_prod_price (const char *nick) const;

	~Game ();
};

#endif
