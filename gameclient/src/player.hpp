#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_


class Player {
	char *nick;
	int raw;
	int prod;
	int money;
	int plants;
	int autoplants;
public:
	Player (char *n, int r, int pr, int m, int pl, int apl);
	char * getnick ();
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
	int getplayercnt () const;	

	Player * parse (char *str) const;

	~ListPlayer ();
};



#endif
