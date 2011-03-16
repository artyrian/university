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

	int _checkactive (char *);

	void market ();

	void buy (int count, int cost);
	void sell (int count, int cost);
	void prod (int count);
	void build () const;
	void turn () const;

	char* _my_id () const;
	int _month () const;
	int _players () const;
	int _active_players () const;
	int _supply () const;
	int _raw_price () const;
	int _demand () const;
	int _production_price () const;
	int _money (char *nick) const;
	int _raw (char *nick) const;
	int _production (char *nick) const;
	int _factories (char *nick) const;
	int _auto_factories (char *nick) const;
	int _manufactured (char *nick) const;
	int _result_raw_sold (char *nick) const;
	int _result_raw_price (char *nick) const;
	int _result_prod_bought (char *nick) const;
	int _result_prod_price (char *nick) const;

	~Game ();
};

#endif
