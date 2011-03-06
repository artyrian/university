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
public:
	Game (char *ip, int port);

	void bought (char *nick, int bought)
	{
		Player *pl = lp->find (nick);
		pl->bought = bought;
	}

	void sold (char *nick, int sold)
	{
		Player *pl = lp->find (nick);
		pl->sold = sold;
	}

	void bankrupt (char *nick)
	{
	}
	int setnick (char *nick);
	int joinroom (int room);
	int create () const;
	int waitplayers (int maxpl);
	int waitstart ();
	int getinfo ();
	int waitendturn ();	// now its const but no always.
	int queue ();

	void checkok ();

	void market ();
	void info () const;

	void buy (int count, int cost) const;
	void sell (int count, int cost) const;
	void prod (int count) const;
	void build () const;
	void turn () const;

//
	char* _my_id () const
	{
		return nick;
	}

	int _month () const
	{
		return month;
	}
	int _players () const
	{
		return inf.players;
	}
	int _active_players () const 
	{
		return inf.active_players;
	}
	int _supply () const
	{
		return mrk.raw_count;
	}
	int _raw_price () const
	{
		return mrk.raw_cost;
	}
	int _demand () const 
	{
		return mrk.prod_count;
	}
	int _production_price () const
	{
		return mrk.prod_cost;
	}
	int _money (char *nick) const
	{
		Player *pl = lp->find (nick);
		return pl->money;
	}
	int _raw (char *nick) const
	{
		Player *pl = lp->find (nick);
		return pl->raw;
	}
	int _production (char *nick) const
	{
		Player *pl = lp->find (nick);
		return pl->prod;	
	}
	int _factories (char *nick) const 
	{
		Player *pl = lp->find (nick);
		return pl->plants;
	}
	int _auto_factories (char *nick) const 
	{
		Player *pl = lp->find (nick);
		return pl->autoplants;
	}
	int _manufactured (char *nick) const
	{
		int manufactured;
		return manufactured;
	}
	int _result_raw_sold (char *nick) const
	{
		int result_raw_sold;
		return result_raw_sold;
	}
	int _result_raw_price (char *nick) const
	{
		int result_raw_price;
		return result_raw_price;
	}
	int _result_prod_bought (char *nick) const
	{
		int result_prod_bought;
		return result_prod_bought;
	}
	int _result_prod_price (char *nick) const
	{
		int result_prod_price;
		return result_prod_price;
	}
//
	~Game ();
};

#endif
