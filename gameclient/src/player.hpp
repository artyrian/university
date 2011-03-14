#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_


struct Player {
	char *nick;
	int raw;
	int prod;
	int money;
	int plants;
	int autoplants;
	int bought;
	int bought_price;
	int sold;
	int sold_price;
	int last_prod;

	int factive;

	Player (char *n, int r, int pr, int m, int pl, int apl, int l_prod);
	void operator= (Player *);
	~Player ();
};



class ListPlayer {
	struct ListElem {
		Player *p;
		ListElem *next;
	};

	ListElem *first;
	int cnt;

	ListElem * newplayer (Player *pl);
	ListElem * findlast () const;
public:
	ListPlayer ();

	void add (Player *pl);
	Player * find (char *nick);
	Player * parse (char *str) const;

	~ListPlayer ();
};



#endif
