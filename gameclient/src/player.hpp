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
	int sold;

	Player (char *n, int r, int pr, int m, int pl, int apl);
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
	void remove (char *);
	Player * find (char *nick);

	int getplayercnt () const;	

	Player * parse (char *str) const;

	~ListPlayer ();
};



#endif
