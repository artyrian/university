#include "player.hpp"

#include <stdio.h>
#include <stdlib.h>


Player::Player (char *n, int r, int pr, int m, int pl, int apl)
	: nick(n), raw(r), prod(pr), money(m), plants(pl), autoplants(apl)	
{
}

Player::~Player ()
{
	delete [] nick;
}




ListPlayer::ListPlayer ()
	: first (0), cnt (0)
{
}



ListPlayer::ListElem * ListPlayer:: newplayer (Player *pl)
{
	class ListElem *elem = new ListElem;

	elem->p = pl;  
	elem->next = 0;
	
	return elem;
}

ListPlayer::ListElem * ListPlayer::findlast () const
{
	class ListElem *cur = first;
	class ListElem *last = first;

	while ( cur != 0 ) {
		last = cur;
			cur = cur->next;
	}

	return last;
}



void ListPlayer::add (Player *pl)
{
	ListElem *elem = newplayer (pl);
	ListElem *last = findlast ();

	if ( first == 0 ) {
		first = elem;
	} else {
		last->next = elem;
	}
	cnt++;
}

void ListPlayer::search ()
{
}


void ListPlayer::remove ()
{
}


int ListPlayer::getplayercnt () const
{
	return cnt;
}



Player * ListPlayer::parse (char *str) const
{
	char *trash, *nick;
	int raw, prod, money, plant, autoplant;

	raw = prod = money = plant = autoplant = -1; 
	trash = new char [6];
	nick = new char [20];

	printf ("Start parse.\n");
	sscanf(	str, "%s%s%s%d%d%d%d%d", 
		trash, trash,  nick, &raw, &prod, &money, &plant, &autoplant);

	if ( 	(raw == -1) || (prod == -1) || 
		(money == -1) || (plant == -1) ||
		(autoplant == -1) )
	{
		perror ("Syntax error in parse '& INFO'.\n");
		exit (1);
	}

	delete [] trash;

	return new Player (nick, raw, prod, money, plant, autoplant);
}



ListPlayer::~ListPlayer() 
{
	while ( first != 0 ) {
		struct ListElem *cur = first;
		first = first->next;
		delete cur->p;
		delete cur;
	}
}
